#include "cmatrix.h"

template <class T> complex_matrix<T>::complex_matrix(size_t m, size_t n, T initValue)
{
	for (size_t i = 0; i < m; i++)
		this->push_back(complex_vector<T>(n, initValue));
}

template <class T> void complex_matrix<T>::FromVector(const complex_vector<T> & vector)
{
	this->clear();
	this->push_back(vector);
	*this = Transpose();
}

template <class T> complex_vector<T> complex_matrix<T>::ToVector() const
{
	complex_vector<T> result;

	for (const auto & row : *this)
		result.push_back(row[0]);

	return result;
}

template <class T> void complex_matrix<T>::FromStringListList(const std::vector<std::vector<std::string>> & list)
{
	this->clear();

	for (auto & row : list)
		this->push_back(complex_vector<T>(row));
}

template <class T> std::vector<std::vector<std::string>> complex_matrix<T>::ToStringListList() const
{
	std::vector<std::vector<std::string>> result;

	for (const auto & row : *this)
	{
		result.push_back(row.ToStringList());
	}

	return result;
}

template <class T> void complex_matrix<T>::FromValueListList(const std::vector<std::vector<T>> & list)
{
	this->clear();

	for (auto & row : list)
		this->push_back(complex_vector<T>(row));
}

template <class T> bool complex_matrix<T>::NearEquals(const complex_matrix & other, double epsilon) const
{
	bool result = false;

	size_t n = this->size();
	if (n == other.size())
	{
		result = true;

		for (size_t i = 0; i < n; i++)
		{
			if (!(*this)[i].NearEquals(other[i], epsilon))
			{
				result = false;
				break;
			}
		}

		return result;
	}
}

template <class T> complex_matrix<T> complex_matrix<T>::AddOrSubtract(const complex_matrix & other, complex_vector<T>(complex_vector<T>::*fnPtr)(const complex_vector<T> &) const) const
{
	size_t n = this->size();

	if (n != other.size())
		throw std::out_of_range("Cannot add or subtract matrixes of different sizes");

	complex_matrix<T> result;

	for (size_t i = 0; i < n; i++)
		result.push_back(((*this)[i].*fnPtr)(other[i]));

	return result;
}

template <class T> complex_matrix<T> complex_matrix<T>::Conjugate() const
{
	complex_matrix<T> result;

	for (auto & value : *this)
		result.push_back(value.Conjugate());

	return result;
}

template <class T> complex_matrix<T> complex_matrix<T>::Inverse() const
{
	complex_matrix<T> result;

	for (auto & value : *this)
		result.push_back(-value);

	return result;
}

template <class T> complex_matrix<T> complex_matrix<T>::Multiply(const T & scalar) const
{
	complex_matrix<T> result;

	for (auto & vector : *this)
		result.push_back(vector * scalar);

	return result;
}

template <class T> complex_matrix<T> complex_matrix<T>::Multiply(const complex_matrix<T> & other) const
{
	size_t m = Rows(), n=Cols(), p = other.Cols();

	if (n != other.Rows())
		throw std::out_of_range("Incompatible matrix sizes for multiplication");

	complex_matrix<T> result(m, p);

	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < p; j++)
		{
			T sum;
			for (size_t k = 0; k < n; k++)
			{
				sum += (*this)[i][k] * other[k][j];
			}

			result[i][j] = sum;
		}
	}

	return result;
}

template <class T> complex_vector<T> complex_matrix<T>::Multiply(const complex_vector<T> & other) const
{
	complex_matrix<T> otherMatrix;
	otherMatrix.FromVector(other);

	return Multiply(otherMatrix).ToVector();
}

template <class T> complex_matrix<T> complex_matrix<T>::Power(size_t k) const
{
	complex_matrix<T> result = CreateIdentityMatrix(Rows());

	for (size_t i = 0; i < k; i++)
		result *= *this;

	return result;
}

template <class T> complex_matrix<T> complex_matrix<T>::Transpose() const
{ // for simplicity, this method has undefined behavior if the current matrix is not valid, i.e. has vectors of different sizes
	complex_matrix<T> result;

	size_t n = this->size();
	if (n)
	{
		size_t m = (*this)[0].size();

		for (size_t i = 0; i < m; i++)
		{
			complex_vector<T> vector;
			for (size_t j = 0; j < n; j++)
			{
				vector.push_back((*this)[j][i]);
			}

			result.push_back(vector);
		}
	}

	return result;
}

template <class T> T complex_matrix<T>::Trace() const
{
	size_t n = this->size();

	T result;

	for (size_t i = 0; i < n; i++)
		result += (*this)[i][i];

	return result;
}

template <class T> T complex_matrix<T>::InnerProduct(const complex_matrix & other) const
{
	return (Adjoint() * other).Trace();
}

template <class T> complex_matrix<T> complex_matrix<T>::TensorProduct(const complex_matrix & other) const
{
	size_t m = Rows(), n = other.Rows();

	//TBD: throw if not square matrixes

	size_t resultSize = m * n;

	complex_matrix<T> result(resultSize, resultSize);

	for (size_t j=0; j<resultSize; j++)
		for (size_t k = 0; k < resultSize; k++)
		{
			result[j][k] = (*this)[j / n][k / m] * other[j % n][k % m];
		}

	return result;
}

template <> cint complex_matrix<cint>::Norm() const
{
	return static_cast<int>(floor(sqrt(InnerProduct(*this).Real())));
}

template <> cdouble complex_matrix<cdouble>::Norm() const
{
	return sqrt(InnerProduct(*this).Real());
}

template <class T> T complex_matrix<T>::RowSum(size_t row) const
{
	return (*this)[row].Sum();
}

template <class T> T complex_matrix<T>::ColSum(size_t col) const
{
	T result;

	for (const auto & row : *this)
		result += row[col];

	return result;
}

template <class T> bool complex_matrix<T>::IsDoublyStochastic() const
{
	size_t n = Rows(); // assumed square matrix, exception not handled here

	for (size_t i = 0; i < n; i++)
	{
		if (T(1, 0) != RowSum(i))
			return false;

		if (T(1, 0) != ColSum(i))
			return false;
	}

	return true;
}

template <class T> bool complex_matrix<T>::IsUnitary() const
{
	size_t n = Rows(); // assumed square matrix, exception not handled here

	complex_matrix<T> identityMatrix = CreateIdentityMatrix(n);

	return (*this) * Conjugate().Transpose() == identityMatrix;
}

template <class T> complex_matrix<T> complex_matrix<T>::CreateZeroMatrix(size_t m, size_t n)
{
	complex_matrix<T> result(m, n);
	return result;
}

template <class T> complex_matrix<T> complex_matrix<T>::CreateIdentityMatrix(size_t size)
{
	complex_matrix<T> result(size, size);

	for (size_t i = 0; i < size; i++)
		result[i][i] = 1;

	return result;
}


namespace
{
	void dummy() // we must create dummy objects and call methods of template classes to avoid linker errors, see https://www.codeproject.com/Articles/48575/How-to-define-a-template-class-in-a-h-file-and-imp
	{
		cint_matrix mi;
		cdouble_matrix md;

		cint_matrix miInit(0, 0);
		cdouble_matrix mdInit(0, 0);

		mi.FromVector(cint_vector());
		md.FromVector(cdouble_vector());
		mi.ToVector();
		md.ToVector();

		mi.FromStringListList({});
		md.FromStringListList({});

		mi.ToStringListList();
		md.ToStringListList();

		cint_matrix({ {1} });
		cdouble_matrix({ {1} });

		mi.NearEquals(mi, 0);
		md.NearEquals(md, 0);

		mi.Add(mi);
		md.Add(md);

		mi.Conjugate();
		md.Conjugate();

		mi.Inverse();
		md.Inverse();

		mi.Multiply(cint());
		md.Multiply(cdouble());

		mi.Multiply(cint_matrix({}));
		md.Multiply(cdouble_matrix({}));

		mi.Multiply(cint_vector({}));
		md.Multiply(cdouble_vector({}));

		mi.Power(0);
		md.Power(0);

		mi.Transpose();
		md.Transpose();

		mi.Trace();
		md.Trace();
		mi.InnerProduct(mi);
		md.InnerProduct(md);

		mi.TensorProduct(mi);
		md.TensorProduct(md);

		mi.Norm();
		md.Norm();

		mi.IsDoublyStochastic();
		md.IsDoublyStochastic();

		mi.IsUnitary();
		md.IsUnitary();

		mi.CreateZeroMatrix(0, 0);
		md.CreateZeroMatrix(0, 0);
		mi.CreateIdentityMatrix(0);
		md.CreateIdentityMatrix(0);
	}
}
