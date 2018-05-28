#pragma once

#include <string>

template <class T> class complex
{
public:
	complex() = default;
	complex(T real, T imag) : m_real(real), m_imag(imag) {}
	complex(T real) : m_real(real), m_imag(0) {}
	complex(const std::string & valueStr);

	T Real() const { return m_real; }
	T Imag() const { return m_imag; }
	explicit operator T() const;

	static complex FromReal(int real); // to all constructing from another type of real number
	static complex FromReal(double real);

	std::string ToString() const;

	bool Equals(const complex & other) const;
	bool operator == (const complex & other) const { return Equals(other); }
	bool operator != (const complex & other) const { return !Equals(other); }

	bool NearEquals(const complex & other) const; // you should not use this with cint
	bool NearEquals(const complex & other, double epsilon) const; // or this

	complex Add(const complex & other) const;
	complex operator + (const complex & other) const { return Add(other); }
	complex & AddTo(const complex & other);
	complex & operator += (const complex & other) { return AddTo(other); }

	complex Subtract(const complex & other) const;
	complex operator - (const complex & other) const { return Subtract(other); }
	complex & SubtractFrom(const complex & other);
	complex & operator -= (const complex & other) { return SubtractFrom(other); }

	complex Multiply(const complex & other) const;
	complex operator * (const complex & other) const { return Multiply(other); }
	complex & MultiplyWith(const complex & other);
	complex & operator *= (const complex & other) { return MultiplyWith(other); }

	complex Divide(const complex & other) const;
	complex operator / (const complex & other) const { return Divide(other); }
	complex & DivideWith(const complex & other);
	complex & operator /= (const complex & other) { return DivideWith(other); }

	T ModulusSquared() const { return m_real * m_real + m_imag * m_imag; }
	T Modulus() const { return static_cast<T>(sqrt(ModulusSquared())); }

	complex Conjugate() const { return complex<T>(m_real, -m_imag); }

	complex Inverse() const { return complex<T>(-m_real, -m_imag); }
	complex operator - () const { return Inverse(); }

	void FromPolar(T modulus, double angle);
	std::pair<T, double> ToPolar() const;


protected:
	static T ValueFromString(const std::string & valueStr, size_t * index=nullptr);

	T m_real = 0;
	T m_imag = 0;
};


typedef complex<int> cint;
typedef complex<double> cdouble;
