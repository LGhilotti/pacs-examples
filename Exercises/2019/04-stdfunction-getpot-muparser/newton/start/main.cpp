#include <iostream>
#include "newton.h"

int main(){
  //this define a callable object (defined the operator () ), and instanciate an object of that class (don't know the name, in fact auto)
  auto fun=[](double x){return (((x*x)+5)*x)+3; }; //this is an object, otherwise to use it in the solver,
  auto der=[](double x){return 3*x*x +5;};  //I would pass the function, in the solver it would be a pointer to function,
  //if you want to define a classic function (above the main)

  newton_solver solver(fun,der);
  solver.solve(1.0);
  double res=solver.get_result();
  std::cout<<"the root is: "<<res<<std::endl;
  return 0;
}
//third version(Ancora Getpot): leggo nome del file da riga di comando, di default data.txt
// qualunque oggetto che ha definito l'operator () può essere usato come std::function, quindi passato al solver.
