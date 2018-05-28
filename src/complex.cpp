#include "complex.h"

#include <cctype>
#include <cfloat>
#include <limits>

template <> static int complex<int>::ValueFromString(const std::string & valueStr, size_t * index)
{
	try
	{
		return std::stoi(valueStr, index);
	}
	catch (...)
	{
		return 0;
	}
}

template <> static double complex<double>::ValueFromString(const std::string & valueStr, size_t * index)
{
	try
	{
		return std::stod(valueStr, index);
	}
	catch (...)
	{
		return 0;
	}
}

template <class T> complex<T>::complex(const std::string & valueStr)
{
	size_t index = 0;
	m_real = ValueFromString(valueStr, &index);

	m_imag = 0;
	bool negativeImag = false;

	for (size_t startIndex = index; index < valueStr.size(); index++)
	{
		const char c = valueStr[index];

		if (c == '-')
			negativeImag = true;

		else if (std::isdigit(c))
		{
			m_imag = ValueFromString(valueStr.substr(index));
			if (negativeImag)
				m_imag = -m_imag;

			break;
		}

		else if (c == 'i')
		{
			if (index != startIndex)
				m_imag = negativeImag ? -1 : 1;
			else
			{
				m_imag = index != 0 ? m_real : 1;
				m_real = 0;
			}
			break;
		}
	}
}

template <class T> complex<T>::operator T() const
{
	if (m_imag != 0)
		throw std::out_of_range("Cannot convert complex number with nonzero imaginary part to real number");

	return m_real;
}

template <> cint complex<int>::FromReal(int real)
{
	return cint(real, 0);
}

template <> cdouble complex<double>::FromReal(int real)
{
	return cdouble(real, 0);
}

template <> cint complex<int>::FromReal(double real)
{
	return cint(static_cast<int>(floor(real)), 0);
}

template <> cdouble complex<double>::FromReal(double real)
{
	return cdouble(real, 0);
}

template <class T> std::string complex<T>::ToString() const
{
	std::string realStr;
	if (m_real != 0 || m_imag == 0)
		realStr = std::to_string(m_real);

	std::string imagStr;
	if (m_imag != 0)
	{
		T absImag = abs(m_imag);

		if (realStr.size() && m_imag > 0)
			realStr += "+";
		else if (m_imag < 0)
			realStr += "-";

		if (absImag != 1)
			imagStr = std::to_string(absImag);

		imagStr += "i";
	}

	return realStr + imagStr;
}

template <class T> bool complex<T>::Equals(const complex & other) const
{
	if (std::numeric_limits<T>::is_exact)
		return m_real == other.Real() && m_imag == other.Imag();

	return NearEquals(other);
}

template <class T> bool complex<T>::NearEquals(const complex & other) const
{
	return NearEquals(other, static_cast<T>(DBL_EPSILON * 10)); // for now we allow a 10x epsilon difference
}

template <class T> bool complex<T>::NearEquals(const complex & other, double epsilon) const
{
	T realDiff = abs(m_real - other.Real());
	T imagDiff = abs(m_imag - other.Imag());

	return realDiff < epsilon && imagDiff < epsilon;
}

template <class T> complex<T> complex<T>::Add(const complex & other) const
{
	return complex<T>(m_real + other.Real(), m_imag + other.Imag());
}

template <class T> complex<T> & complex<T>::AddTo(const complex & other)
{
	*this = Add(other);
	return *this;
}

template <class T> complex<T> complex<T>::Subtract(const complex & other) const
{
	return complex<T>(m_real - other.Real(), m_imag - other.Imag());
}

template <class T> complex<T> & complex<T>::SubtractFrom(const complex & other)
{
	*this = Subtract(other);
	return *this;
}

template <class T> complex<T> complex<T>::Multiply(const complex & other) const
{
	const T otherReal = other.Real();
	const T otherImag = other.Imag();

	const T real = m_real * otherReal - m_imag * otherImag;
	const T imag = m_real * otherImag + otherReal * m_imag;

	return complex<T>(real, imag);
}

template <class T> complex<T> & complex<T>::MultiplyWith(const complex & other)
{
	*this = Multiply(other);
	return *this;
}

template <class T> complex<T> complex<T>::Divide(const complex & other) const
{
	const T otherReal = other.Real();
	const T otherImag = other.Imag();

	const T otherModulusSquared = other.ModulusSquared();

	const T real = (m_real * otherReal + m_imag * otherImag) / otherModulusSquared;
	const T imag = (otherReal * m_imag - m_real *  otherImag) / otherModulusSquared;

	return complex<T>(real, imag);
}

template <class T> complex<T> & complex<T>::DivideWith(const complex & other)
{
	*this = Divide(other);
	return *this;
}

template <class T> void complex<T>::FromPolar(T modulus, double angle)
{
	m_real = static_cast<T>(modulus * cos(angle));
	m_imag = static_cast<T>(modulus * sin(angle));
}

template <class T> std::pair<T, double> complex<T>::ToPolar() const
{
	return std::make_pair(Modulus(), atan(m_imag / m_real));
}

namespace {
	void dummy() // we must create dummy objects and call methods of template classes to avoid linker errors, see https://www.codeproject.com/Articles/48575/How-to-define-a-template-class-in-a-h-file-and-imp
	{
		cint ci("");
		cdouble cd("");

		cint::FromReal(0);
		cint::FromReal(0.0);
		cdouble::FromReal(0);
		cdouble::FromReal(0.0);

		ci.ToString();
		cd.ToString();

		ci.Equals(cint());
		cd.Equals(cdouble());

		ci.Add(cint());
		cd.Add(cdouble());
		ci.AddTo(cint());
		cd.AddTo(cdouble());

		ci.Subtract(cint());
		cd.Subtract(cdouble());
		ci.SubtractFrom(cint());
		cd.SubtractFrom(cdouble());

		ci.Multiply(cint());
		cd.Multiply(cdouble());
		ci.MultiplyWith(cint());
		cd.MultiplyWith(cdouble());

		ci.Divide(cint());
		cd.Divide(cdouble());
		ci.DivideWith(cint());
		cd.DivideWith(cdouble());

		(int)ci;
		(double)cd;

		static_cast<int>(ci);
		static_cast<double>(cd);

		ci.FromPolar(0, 0);
		cd.FromPolar(0, 0);

		ci.ToPolar();
		cd.ToPolar();
	}
}
