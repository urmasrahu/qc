#pragma once

#include "cmatrix.h"

namespace QC_Algorithms
{
	cdouble_vector BraFromKet(const cdouble_vector & ket);
	cdouble TransitionAmplitude(const cdouble_vector & oldState, const cdouble_vector & newState);

	double ObservationProbability(const cdouble_vector & state, size_t position);

  cdouble_matrix AveragerMatrix(size_t size);

	cdouble_matrix FillWithBinaryVectorsInOrder(size_t n);
}
