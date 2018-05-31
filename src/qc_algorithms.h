#pragma once

#include "cmatrix.h"

namespace QC_Algorithms
{
	cdouble_vector BraFromKet(const cdouble_vector & ket);
	cdouble TransitionAmplitude(const cdouble_vector & oldState, const cdouble_vector & newState);

	double ObservationProbability(const cdouble_vector & state, size_t position);

	cdouble_matrix HadamardMatrix(size_t order);
	cdouble_matrix AveragerMatrix(size_t size);

	cdouble_vector InverseAboutMean(const cdouble_vector & vector);

	cdouble_matrix FillWithBinaryVectorsInOrder(size_t n);
	cdouble_vector CreateQubitStateVectorAndInitializeToZero(size_t numberOfQubits);

	size_t Measure(const cdouble_vector & state); // returns the index of the state that got measured (random operation)
	std::vector<double> MeasurementProbabilitiesVector(const cdouble_vector & state);
	double RandomNumber(); // between 0 and 1

	cint_vector PowersOfModulo(int a, int N, size_t count); // 6.5, page 206
}
