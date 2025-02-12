This is an example of a very dangerous (and difficult to detect)
error.  It is related to the relaxed One Definition Rule (odr) when
dealing with template functions.

The one definition rule says that I can have in a program only one
definition for an object. For ordinary (not template, nor automatic
return) functions, it means that if during the linking phase the
compiler finds the definition of two identical functions it gives an
error. This error may happen if you, by mistake, have defined the
function in a header file, and not in the source file.

However, the mechanism of instantiation of function templates makes it
inevitable that all instances of template functions with the same
arguments are compiled in all translation units that instantiate them,
(thats why function template definitions must go in the header file).

Example:

file myfun.hpp
template <class T>
double myfun(const T & x){...}

file f1.cpp
#include "myfun.hpp"
...
double c=myfun(5.0) -> will instantiate myfun(const double &)
                       the compiler will insert the corresponding machine code
                       in f1.o

file f2.cpp
#include "myfun.hpp"
...
double c=myfun(7.0) -> will instantiate myfun(const double &)
                       the compiler will insert the corresponding
                       machine code in f2.o

g++ main.o f1.o f2.o -o main -> the linker finds two definitions of
                                myfun(const double&) in the object files.
                                It is not a problem, it just chooses one.

The linker in this case will insert in the executable only one copy of
the compiled code for myfun(const double). In normal cases, like the
one just shown, this is perfectly fine since all instances are
identical. But unfortunately *the linker does not check if they are identical*!

In this example we show what can happen if one creates two function
templates with the same name and parameters but with a different
semantic (*something you should never do!*).

In three.hpp I have the definition of a function template, called
three, that just multiplies the input by 3.0.  In newthree.hpp I have
the definition of a function template with the same (full qualified)
name, same parameters, but this time the function divides by three!

Now let's see how this can be VERY dangerous. In fun.hpp and fun2.hpp
I declare two functions, the first uses the first version of three,
the second is meant to use the second version. However, since the function
three() is not referred in the function declarations I can avoid including
three.hpp and newthree.hpp in those files.  Indeed, they are included
only in the corrresponding source files, fun.cpp and fun2.cpp, where
function three is actually used and thus instantiated with the correct
type (the same in both cases).

After compilation (which goes perfectly well) if you do
nm --demangle fun.o | grep three
you get

00000000 W double three<double>(double const&)

The W tells that that symbol is indeed resolved (defined), but it is
the instance of a function template: the function three() istantiated
with T=double.

nm --demangle fun2.o | grep three will give the same result, since
also fun2 instantiates three with T=double. Perfectly fine. But if we
have a main that uses both fun and fun2 we have something nasty.

The linker find the function defined twice, but it is not an error
since we have two instances of the same template function, *or at
least this is what the linker believes*.

So, eventually in the main we have the code corresponding to only one
of the two versions of three(double const). Which one?  It depends on
the order the object files are given in the linking process!!

Run make, see what it does, and then run main and main2 and see what
happens!!

HOW TO SOLVE

First of all let's that this very nasty and difficult to detect error
in principle should not happen. The standard says that you can have
more than one identical instances of function templates, but they must
be identical. Unfortunately the linker has no way to check it.

Secondly, you should never write two function tempaltes with the same
name and parameters. The use of namespaces may also help to avoid name
clashes.

Thirdly, a good rule is that you include the definition of the
template in the header file where you declare a function or a class
that you know makes use of a template function, even when it is not
strictly necessary.Indeed, try to uncomment the #include statements in
fun.hpp and fun2.hpp and see what happens. Now, the main does not even
compile!  So you know that you have a problem to sort out: change the
name of one of the two functions (or use namespaces to separate them,
what matters is the full qualified name).
