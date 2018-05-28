#include "qc_algorithms.h"

cdouble_vector QC_Algorithms::BraFromKet(const cdouble_vector & ket)
{
	return ket.Conjugate();
}

cdouble QC_Algorithms::TransitionAmplitude(const cdouble_vector & oldState, const cdouble_vector & newState)
{
	return newState.Normalize() * oldState.Normalize();
}

double QC_Algorithms::ObservationProbability(const cdouble_vector & state, size_t position)
{
	return state[position].ModulusSquared() / state.NormSquare().Real();
}
