#include "qc_algorithms.h"
#include "matrix_constants.h"

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

static bool parity(size_t n) // from https://www.geeksforgeeks.org/write-a-c-program-to-find-the-parity-of-an-unsigned-integer/
{
	bool parity = 0;

	while (n)
	{
		parity = !parity;
		n = n & (n - 1);
	}
	return parity;
}

cdouble_matrix QC_Algorithms::HadamardMatrix(size_t order)
{
	if (order == 0)
		throw std::out_of_range("Hadamard matrix order must be at least 1");

	size_t size = 1 << order;

	cdouble_matrix result(size, size);

	double coefficient = 1 / sqrt(1 << (order));

	for (size_t i=0; i<size; i++)
		for (size_t j = 0; j < size; j++)
		{
			double multiplier = parity(i & j) ? -1 : 1; // see pages 181-184

			result[i][j] = coefficient * multiplier;
		}

	return result;
}

cdouble_matrix QC_Algorithms::AveragerMatrix(size_t size)
{
	return cdouble_matrix(size, size, cdouble(1.0 / size));
}

cdouble_vector QC_Algorithms::InverseAboutMean(const cdouble_vector & vector)
{
	cdouble_matrix A = AveragerMatrix(vector.size());

	return -vector + 2 * A * vector;
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

cint_vector QC_Algorithms::PowersOfModulo(int a, int N, size_t count)
{
	cint_vector result;

	for (size_t x = 0; x < count; x++)
	{
		result.push_back(std::llround(pow(a, x)) % N);
	}

	return result;
}
