#include "quantum_gates.h"

#define _USE_MATH_DEFINES
#include <math.h>  

cdouble_matrix QuantumGates::SquareRootOfNot()
{
  cdouble_matrix result;
  
  result.FromStringListList({
    { "1", "-1" },
    { "1", "1"  },
    }
  );

  return result * M_SQRT1_2;
}
