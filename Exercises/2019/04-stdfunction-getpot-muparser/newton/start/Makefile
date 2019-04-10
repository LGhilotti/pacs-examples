CXX       = g++
CXXFLAGS  ?= -std=c++14
CPPFLAGS  ?= -I. -Wno-conversion-null -Wno-deprecated-declarations
LDFLAGS   ?=
LIBS      ?=

OBJECTS=main.o
SOURCES=main.cpp
HEADERS=newton.h
EXEC=newton_example

all : $(EXEC)

%.o: %.cpp $(HEADERS)
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $<

$(EXEC) : $(OBJECTS)
	$(CXX) -o $@ $^


clean :
	$(RM) *.o

distclean : clean
	$(RM) $(EXEC) $(EXECMP)
