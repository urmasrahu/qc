#pragma once

#include "cmatrix.h"

#define _USE_MATH_DEFINES
#include <math.h>  

namespace MatrixConstants
{
  const cdouble_matrix HADAMARD(
    {
      { cdouble(M_SQRT1_2), cdouble(M_SQRT1_2) },
      { cdouble(M_SQRT1_2), cdouble(-M_SQRT1_2) }
    });

	const cdouble_matrix CNOT( // controlled-NOT gate, see pages 153-154
		{
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 1},
			{0, 0, 1, 0}
		});

	const cdouble_matrix SQRT_NOT( // square root of NOT gate, see page 159
		{
			{ cdouble(M_SQRT1_2), cdouble(-M_SQRT1_2) },
			{ cdouble(M_SQRT1_2), cdouble(M_SQRT1_2) }
		}
	);
}
