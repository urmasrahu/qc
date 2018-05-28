#include "cvector.h"

template <class T> complex_vector<T>::complex_vector(size_t size, T initValue /*= T()*/)
{
	for (size_t i = 0; i < size; i++)
		this->push_back(initValue);
}

template <class T> void complex_vector<T>::FromStringList(const std::vector<std::string> & list)
{
	this->clear();

	for (auto str : list)
		this->push_back(T(str));
}

template <class T> std::vector<std::string> complex_vector<T>::ToStringList() const
{
	std::vector<std::string> result;

	for (auto & value : *this)
		result.push_back(value.ToString());

	return result;
}

template <class T> void complex_vector<T>::FromValueList(const std::vector<T> & list)
{
	this->clear();

	for (auto value : list)
		this->push_back(value);
}

template <class T> bool complex_vector<T>::NearEquals(const complex_vector<T> & other, double epsilon) const
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
	}

	return result;
}

template <class T> complex_vector<T> complex_vector<T>::Add(const complex_vector & other) const
{
	return AddOrSubtract(other, &T::Add);
}

template <class T> complex_vector<T> complex_vector<T>::Subtract(const complex_vector & other) const
{
	return AddOrSubtract(other, &T::Subtract);
}

template <class T> complex_vector<T> complex_vector<T>::AddOrSubtract(const complex_vector & other, T(T::*fnPtr)(const T &) const) const
{
	size_t n = this->size();

	if (n != other.size())
		throw std::out_of_range("Cannot add or subtract vectors of different sizes");

	complex_vector<T> result;

	for (size_t i = 0; i < n; i++)
		result.push_back(((*this)[i].*fnPtr)(other[i]));

	return result;
}

template <class T> complex_vector<T> complex_vector<T>::Conjugate() const
{
	complex_vector<T> result;

	for (auto & value : *this)
		result.push_back(value.Conjugate());

	return result;
}

template <class T> complex_vector<T> complex_vector<T>::Inverse() const
{
	complex_vector<T> result;

	for (auto & value : *this)
		result.push_back(-value);

	return result;
}

template <class T> complex_vector<T> complex_vector<T>::Multiply(const T & scalar) const
{
	complex_vector<T> result;

	for (auto & value : *this)
		result.push_back(value * scalar);

	return result;
}

template <class T> T complex_vector<T>::InnerProduct(const complex_vector<T> & other) const
{
	size_t n = this->size();

	if (n != other.size())
		throw std::out_of_range("Cannot compute inner product of vectors of different sizes");

	T result;

	for (size_t i = 0; i < n; i++)
		result += (*this)[i].Conjugate() * other[i];

	return result;
}

template <class T> complex_vector<T> complex_vector<T>::TensorProduct(const complex_vector<T> & other) const
{
	size_t m = this->size(), n = other.size();
	size_t length = m * n;

	complex_vector<T> result (length);

	for (size_t i = 0; i < length; i++)
		result[i] = (*this)[i / n] * other[i % n];

	return result;
}

template <class T> T complex_vector<T>::Norm() const
{
	return T::FromReal(sqrt(NormSquare().Real()));
}

template <class T> T complex_vector<T>::NormSquare() const
{
	return T::FromReal(InnerProduct(*this).Real());
}

template <class T> complex_vector<T> complex_vector<T>::Normalize() const
{
	size_t n = this->size();

	complex_vector<T> result(n);

	T length = this->Lenght();

	for (size_t i = 0; i < n; i++)
		result[i] = (*this)[i] / length;

	return result;
}

template <class T> T complex_vector<T>::Distance(const complex_vector & other) const
{
	return Subtract(other).Norm();
}

template <class T> T complex_vector<T>::Sum() const
{
	T result;

	for (auto value : *this)
		result += value;

	return result;
}

template <class T> complex_vector<T> complex_vector<T>::CreateZeroVector(size_t size)
{
	complex_vector<T> result(size);
	return result;
}


namespace
{
	void dummy() // we must create dummy objects and call methods of template classes to avoid linker errors, see https://www.codeproject.com/Articles/48575/How-to-define-a-template-class-in-a-h-file-and-imp
	{
		cint_vector vi;
		cdouble_vector vd;

		vi.FromStringList({});
		vd.FromStringList({});
		vi.ToStringList();
		vd.ToStringList();

		cint_vector({ 1 });
		cdouble_vector({ 1 });

		vi.FromValueList({});
		vd.FromValueList({});

		vi.NearEquals(vi, 0);
		vd.NearEquals(vd, 0);

		vi.Add(cint_vector());
		vd.Add(cdouble_vector());
		vi.Subtract(cint_vector());
		vd.Subtract(cdouble_vector());

		vi.Conjugate();
		vd.Conjugate();

		vi.Inverse();
		vd.Inverse();

		vi.Multiply({});
		vd.Multiply({});

		vi.InnerProduct(vi);
		vd.InnerProduct(vd);

		vi.TensorProduct(vi);
		vd.TensorProduct(vd);

		vi.Norm();
		vd.Norm();
		vi.NormSquare();
		vd.NormSquare();
		vi.Normalize();
		vd.Normalize();

		vi.Distance(vi);
		vd.Distance(vd);
		vi.Sum();
		vd.Sum();

		cint_vector viInit(0);
		cdouble_vector vdInit(0);

		vi.CreateZeroVector(0);
		vd.CreateZeroVector(0);
	}
}
