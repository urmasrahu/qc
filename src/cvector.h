#pragma once

#include <vector>
#include "complex.h"

template <class T> class complex_vector : public std::vector<T>
{
public:
	complex_vector() = default;
	complex_vector(const std::vector<std::string> & list) { FromStringList(list); }
	complex_vector(const std::vector<T> & list) { FromValueList(list); }
	complex_vector(size_t size, T initValue=T());

	void FromStringList(const std::vector<std::string> & list);
	std::vector<std::string> ToStringList() const;

	void FromValueList(const std::vector<T> & list);

	bool NearEquals(const complex_vector & other, double epsilon) const;

	complex_vector Add(const complex_vector & other) const;
	complex_vector operator + (const complex_vector & other) const { return Add(other); }
	complex_vector Subtract(const complex_vector & other) const;
	complex_vector operator - (const complex_vector & other) const { return Subtract(other); }

	complex_vector Conjugate() const;

	complex_vector Inverse() const;
	complex_vector operator - () const { return Inverse(); }

	complex_vector Multiply(const T & scalar) const;
	complex_vector operator * (const T & scalar) const { return Multiply(scalar); }
	complex_vector operator *= (const T & scalar) { *this = Multiply(scalar); return *this; }

	T InnerProduct(const complex_vector & other) const;
	T operator * (const complex_vector & other) const { return InnerProduct(other); }

	complex_vector TensorProduct(const complex_vector & other) const;

	T Norm() const; // note that this result is always a real unumber
	T Lenght() const { return Norm(); } // alias
	T NormSquare() const;
	T LengthSquare() const { return NormSquare(); } // alias

	complex_vector Normalize() const;

	T Distance(const complex_vector & other) const;

	T Sum() const;

	static complex_vector CreateZeroVector(size_t size);

protected:
	complex_vector AddOrSubtract(const complex_vector & other, T (T::*fnPtr)(const T &) const) const;
};


template <typename T> complex_vector<T> operator * (const T & scalar, const complex_vector<T> & vector)
{
	return vector * scalar;
}


typedef complex_vector<cint> cint_vector;
typedef complex_vector<cdouble> cdouble_vector;

