
#ifndef HAVE_CONCRETE2_H
#define HAVE_CONCRETE2_H

#include "abstract.h"

class
concrete2 : public abstract
{

public :

  double eval (double x); //override
  ~concrete2 ();
};

#endif
