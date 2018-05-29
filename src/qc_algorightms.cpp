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

cdouble_matrix QC_Algorithms::AveragerMatrix(size_t size)
{
	return cdouble_matrix(size, size, cdouble(1.0 / size));
}

cdouble_matrix QC_Algorithms::FillWithBinaryVectorsInOrder(size_t n) // eg. "000", "001", "010", "011", "100" etc if n == 3
{
	cdouble_matrix result;

	size_t count = 1 << n; // 2 to power of n

	for (size_t i = 0; i < count; i++)
	{
		cdouble_vector v;

		for (int j = n - 1; j >= 0; j--)
		{
			v.push_back((i / (1 << j)) % 2); // "1 << j" is "2 to power of j"
		}

		result.push_back(v);
	}

	return result;
}
