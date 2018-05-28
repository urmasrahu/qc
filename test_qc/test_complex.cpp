#include <gtest\gtest.h>

#define _USE_MATH_DEFINES
#include <math.h>  

#include "complex.h"

using namespace testing;

class complexTest : public Test
{
public:
	complexTest() = default;
};

TEST_F(complexTest, CtrDefault)
{
	cint ci;
	EXPECT_EQ(0, ci.Real());
	EXPECT_EQ(0, ci.Imag());

	cdouble cd;
	EXPECT_EQ(0.0, cd.Real());
	EXPECT_EQ(0.0, cd.Imag());
}

TEST_F(complexTest, CtrInit)
{
	cint ci(3, 14);
	EXPECT_EQ(3, ci.Real());
	EXPECT_EQ(14, ci.Imag());

	cdouble cd(3.14, 2.72);
	EXPECT_EQ(3.14, cd.Real());
	EXPECT_EQ(2.72, cd.Imag());

	cint real(15);
	EXPECT_EQ(cint(15, 0), real);
}

TEST_F(complexTest, CtrStringInit_int)
{
	cint ci("3+14i");
	EXPECT_EQ(3, ci.Real());
	EXPECT_EQ(14, ci.Imag());

	cint ci0("0");
	EXPECT_EQ(0, ci0.Real());
	EXPECT_EQ(0, ci0.Imag());

	cint ci1("-1");
	EXPECT_EQ(-1, ci1.Real());
	EXPECT_EQ(0, ci1.Imag());

	cint ci2("2-17i");
	EXPECT_EQ(2, ci2.Real());
	EXPECT_EQ(-17, ci2.Imag());

	cint ci3("2 - 17i");
	EXPECT_EQ(2, ci3.Real());
	EXPECT_EQ(-17, ci3.Imag());

	cint ci4("-34 - 17i");
	EXPECT_EQ(-34, ci4.Real());
	EXPECT_EQ(-17, ci4.Imag());

	cint ci5("3-i");
	EXPECT_EQ(3, ci5.Real());
	EXPECT_EQ(-1, ci5.Imag());

	cint ci6("i");
	EXPECT_EQ(0, ci6.Real());
	EXPECT_EQ(1, ci6.Imag());

	cint ci7("-i");
	EXPECT_EQ(0, ci7.Real());
	EXPECT_EQ(-1, ci7.Imag());

	cint ci8("47");
	EXPECT_EQ(47, ci8.Real());
	EXPECT_EQ(0, ci8.Imag());

	cint ci9("3i");
	EXPECT_EQ(0, ci9.Real());
	EXPECT_EQ(3, ci9.Imag());
}

TEST_F(complexTest, CtrStringInit_double)
{
	cdouble cd("3+14i");
	EXPECT_EQ(3, cd.Real());
	EXPECT_EQ(14, cd.Imag());

	cdouble cd2("3 - 14i");
	EXPECT_EQ(3, cd2.Real());
	EXPECT_EQ(-14, cd2.Imag());

	cdouble cd3("-3.2 + 14.7i");
	EXPECT_EQ(-3.2, cd3.Real());
	EXPECT_EQ(14.7, cd3.Imag());
}

TEST_F(complexTest, Operator_T_cast)
{
	EXPECT_EQ(3, static_cast<int>(cint("3")));
}

TEST_F(complexTest, Operator_T_cast_exception)
{
	try
	{
		static_cast<int>(cint("3+2i"));
		EXPECT_TRUE(false); // above line must throw exception, if it does not we fail the test here
	}
	catch (std::out_of_range &)
	{
	}
}


TEST_F(complexTest, ToString)
{
	EXPECT_EQ("3+14i", cint("3+14i").ToString());
	EXPECT_EQ("0", cint("0").ToString());
	EXPECT_EQ("-1", cint("-1").ToString());
	EXPECT_EQ("2-17i", cint("2-17i").ToString());
	EXPECT_EQ("-34-17i", cint("-34-17i").ToString());
	EXPECT_EQ("3-i", cint("3-i").ToString());
	EXPECT_EQ("i", cint("i").ToString());
	EXPECT_EQ("-i", cint("-i").ToString());
	EXPECT_EQ("47", cint("47").ToString());
	EXPECT_EQ("3i", cint("3i").ToString());
	EXPECT_EQ("1", cint("1").ToString());

	// not testing cdouble as the precision of outputs is not well defined
}

TEST_F(complexTest, Equals)
{
	EXPECT_TRUE(cint(3, 45).Equals(cint("3+45i")));
	EXPECT_TRUE(cint(3, 45) != cint("-3+45i"));
	EXPECT_TRUE(cdouble(12.3, -45.6) == cdouble("12.3-45.6i"));
}

TEST_F(complexTest, Add)
{
	cint c1(3, 4);
	cint c2(5, 6);

	cint sum = c1.Add(c2);
	EXPECT_EQ(8, sum.Real());
	EXPECT_EQ(10, sum.Imag());

	cint sum2 = c1 + c2;
	EXPECT_EQ(8, sum2.Real());
	EXPECT_EQ(10, sum2.Imag());

	EXPECT_EQ(cint("4+3i"), cint("3-i") + cint("1+4i")); // example 1.1.2
	EXPECT_EQ(cint("-1-3i"), cint("-3+i") + cint("2-4i")); // exercise 1.1.3

	EXPECT_EQ(sum += c1, sum2 + c1);
}

TEST_F(complexTest, Subtract)
{
	cint diff = cint(9, 8) - cint(12, 3);
	EXPECT_EQ(-3, diff.Real());
	EXPECT_EQ(5, diff.Imag());

	cint other = diff;
	EXPECT_EQ(diff -= cint(9, 8), other - cint(9, 8));
}

TEST_F(complexTest, Multiply)
{
	EXPECT_EQ(cint("7+11i"), cint("3-i") * cint("1+4i")); // example 1.1.2
	EXPECT_EQ(cint("-2+14i"), cint("-3+i") * cint("2-4i")); // exercise 1.1.3

	const cdouble a("32+12i"), b("8-6i");
	cdouble c = a;
	EXPECT_EQ(c *= b, c * b);
}

TEST_F(complexTest, Divide)
{
	EXPECT_EQ(cint("i"), cint("-2+i") / cint("1+2i")); // example 1.2.2
	EXPECT_EQ(cdouble("-1.5-1.5i"), cdouble("3i") / cdouble("-1-i")); // exercise 1.2.3

	const cdouble a("32+12i"), b("8-6i");
	cdouble c = a;
	EXPECT_EQ(c /= b, c / b);
}

TEST_F(complexTest, Modulus)
{
	EXPECT_EQ(sqrt(2), cdouble("1-i").Modulus()); // example 1.2.3
}

TEST_F(complexTest, Conjugate)
{
	EXPECT_EQ(cint("2-3i"), cint("2+3i").Conjugate());
}

TEST_F(complexTest, Inverse)
{
	EXPECT_EQ(cint("2+3i"), cint("-2-3i").Inverse());
	EXPECT_EQ(cint("-2-3i"), -cint("2+3i"));
}

TEST_F(complexTest, relationship_modulus_squared_and_conjugate) // see expression (1.50)
{
	cint c("3+2i");
	cint product = c * c.Conjugate();
	int real = product.Real();
	int imag = product.Imag();

	EXPECT_EQ(13, real);
	EXPECT_EQ(0, imag);

	EXPECT_EQ(real, c.ModulusSquared());

	// below, we test the same thing, but more concisely using the typecast operator
	EXPECT_EQ(static_cast<int>(product), c.ModulusSquared());
}

TEST_F(complexTest, FromPolar_ToPolar)
{
	cdouble c("1+i"); // example 1.3.2
	std::pair<double, double> polar = c.ToPolar();
	EXPECT_DOUBLE_EQ(M_SQRT2, polar.first);
	EXPECT_DOUBLE_EQ(M_PI_4, polar.second);

	c.FromPolar(3, M_PI / 3); // exercise 1.3.3

	EXPECT_DOUBLE_EQ(1.5, c.Real());
	EXPECT_NEAR(2.598, c.Imag(), 0.001);

	polar = c.ToPolar(); // convert the same back
	EXPECT_DOUBLE_EQ(3, polar.first);
	EXPECT_DOUBLE_EQ(M_PI / 3, polar.second);
}