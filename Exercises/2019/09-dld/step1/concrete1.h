
#ifndef HAVE_CONCRETE1_H
#define HAVE_CONCRETE1_H

#include "abstract.h"

class
concrete1 : public abstract
{

public :

  double eval (double x); //override
  ~concrete1 ();
};

#endif
