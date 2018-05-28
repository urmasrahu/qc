#pragma once

#include "cmatrix.h"

#define _USE_MATH_DEFINES
#include <math.h>  

namespace MatrixConstants
{
  const cdouble_matrix HADAMARD_2(
    {
      { cdouble(M_SQRT1_2), cdouble(M_SQRT1_2) },
      { cdouble(M_SQRT1_2), cdouble(-M_SQRT1_2) }
    });
}
