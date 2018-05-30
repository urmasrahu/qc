#pragma once

#include "cvector.h"

template <class T> class complex_matrix : public std::vector<complex_vector<T>>
{
public:
	complex_matrix() = default;
	complex_matrix(const std::vector<std::vector<std::string>> & list) { FromStringListList(list); }
	complex_matrix(const std::vector<std::vector<T>> & list) { FromValueListList(list); }
	complex_matrix(size_t m, size_t n, T initValue = T());

	size_t Rows() const { return this->size(); }
	size_t Cols() const { return (*this)[0].size(); }

	void FromVector(const complex_vector<T> & vector); // not made a constuctor as this would create ambiguity with other constructors
	complex_vector<T> ToVector() const;

	void FromStringListList(const std::vector<std::vector<std::string>> & list);
	std::vector<std::vector<std::string>> ToStringListList() const;

	void FromValueListList(const std::vector<std::vector<T>> & list);

	bool NearEquals(const complex_matrix & other, double epsilon) const;

	complex_matrix Add(const complex_matrix & other) const { return AddOrSubtract(other, &complex_vector<T>::Add); }
	complex_matrix operator + (const complex_matrix & other) const { return Add(other); }
	complex_matrix Subtract(const complex_matrix & other) const { return AddOrSubtract(other, &complex_vector<T>::Subtract); }
	complex_matrix operator - (const complex_matrix & other) const { return Subtract(other); }

	complex_matrix Conjugate() const;

	complex_matrix Inverse() const;
	complex_matrix operator - () const { return Inverse(); }

	complex_matrix Multiply(const T & scalar) const;
	complex_matrix operator * (const T & scalar) const { return Multiply(scalar); }
	complex_matrix operator *= (const T & scalar) { *this * Multiply(scalar); return *this; }

	complex_matrix Multiply(const complex_matrix & other) const;
	complex_matrix operator * (const complex_matrix & other) const { return Multiply(other); }
	complex_matrix operator *= (const complex_matrix & other) { *this = Multiply(other); return *this; }

	complex_vector<T> Multiply(const complex_vector<T> & other) const;
	complex_vector<T> operator * (const complex_vector<T> & other) const { return Multiply(other); }

	complex_matrix Power(size_t k) const;
	complex_matrix operator ^ (size_t k) const { return Power(k); }

	complex_matrix Transpose() const;

	complex_matrix Adjoint() const { return Transpose().Conjugate(); }
	complex_matrix Dagger() const { return Adjoint(); } // alias

	T Trace() const;
	T InnerProduct(const complex_matrix & other) const;

	complex_matrix TensorProduct(const complex_matrix & other) const;

	T Norm() const; // note that this result is always a real unumber
	T Lenght() const { return Norm(); } // alias

	T RowSum(size_t row) const;
	T ColSum(size_t col) const;

	bool IsDoublyStochastic() const;
	bool IsUnitary() const;

	static complex_matrix CreateFromVector(const complex_vector<T> & vector) { complex_matrix M; M.FromVector(vector); return M; }
	static complex_matrix CreateZeroMatrix(size_t m, size_t n);
	static complex_matrix CreateIdentityMatrix(size_t size);

protected:
	complex_matrix AddOrSubtract(const complex_matrix & other, complex_vector<T> (complex_vector<T>::*fnPtr)(const complex_vector<T> &) const) const;
};


template <typename T> complex_matrix<T> operator * (const T & scalar, const complex_matrix<T> & matrix)
{
	return matrix * scalar;
}


typedef complex_matrix<cint> cint_matrix;
typedef complex_matrix<cdouble> cdouble_matrix;

// convenience operators for multiplying matrixes with scalars
inline cint_matrix operator * (int scalar, const cint_matrix & matrix)
{
	return matrix * cint(scalar);
}

inline cdouble_matrix operator * (double scalar, const cdouble_matrix & matrix)
{
	return matrix * cdouble(scalar);
}
