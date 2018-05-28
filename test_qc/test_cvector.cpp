#include <gtest\gtest.h>
#include <iostream>

#include "cvector.h"

#define _USE_MATH_DEFINES
#include <math.h>  


using namespace testing;

class cvectorTest : public Test
{
public:
	cvectorTest() = default;

	template <typename T> void PrintVectorToConsole(const complex_vector<T> & vector, const char * title = nullptr)
	{
		if (title)
			std::cout << title << "\n";

		std::vector<std::string> dbg = vector.ToStringList();
		for (auto value : dbg)
			{
				std::cout << value << " ";
			}
		std::cout << "\n";
	}
};


TEST_F(cvectorTest, CtrFromStringList)
{
	cdouble_vector v({ "2.7-6.9i" });

	ASSERT_EQ(1, v.size());
	EXPECT_EQ(cdouble("2.7-6.9i"), v[0]);

	v.FromStringList({ "1", "2" });
	EXPECT_EQ(2, v.size());
}

TEST_F(cvectorTest, CtrInitWithSameValue)
{
	cdouble_vector v6(6, { "3-5i" }); // you can also construct a complex with string like this
	ASSERT_EQ(6, v6.size());
	for (auto value : v6)
	{
		EXPECT_EQ(cdouble("3-5i"), value);
	}

	cint_vector zeroVector(4);
	ASSERT_EQ(4, zeroVector.size());
	for (auto value : zeroVector)
	{
		EXPECT_EQ(cint(), value);
		EXPECT_EQ(cint("0"), value);
	}
}

TEST_F(cvectorTest, FromStringList)
{
	cint_vector v;
	EXPECT_EQ(0, v.size());

	v.FromStringList({ "6-4i", "7+3i" });

	ASSERT_EQ(2, v.size());
	EXPECT_EQ(cint("6-4i"), v[0]);
	EXPECT_EQ(cint("7+3i"), v[1]);
}

TEST_F(cvectorTest, Add_exception)
{
	cint_vector v1({ std::string("6-4i"), "7+3i" });
	cint_vector v2({ "6-4i" });

	try
	{
		v1.Add(v2);
		EXPECT_TRUE(false); // above line must throw exception, if it does not we fail the test here
	}
	catch (std::out_of_range &)
	{
	}
}

TEST_F(cvectorTest, Add_Subtract)
{
	cdouble_vector v1({ "5+13i", "6+2i", "0.53-6i", "12" }); // exercise 2.1.1
	cdouble_vector v2({ "7-8i", "4i", "2", "9.4+3i" });

	cdouble_vector sum = v1 + v2;
	EXPECT_EQ(cdouble_vector({ "12+5i", "6+6i", "2.53-6i", "21.4+3i" }), sum);

	EXPECT_EQ(v1 + v2, v2 + v1);

	cdouble_vector zero(4);
	EXPECT_EQ(v1, v1 + zero);
	EXPECT_EQ(v1, zero + v1);

	EXPECT_EQ(v1, sum - v2);
	EXPECT_EQ(zero, v2 - v2);


	//for (auto str : sum.ToStringList())
	//{
	//	std::cout << str << "\n";
	//}
}

TEST_F(cvectorTest, Inverse)
{
	cdouble_vector v1({"6-4i", "7+3i", "4.2-8.1i", "-3i"});
	auto v2 = v1.Inverse();

	auto zeroVector = cdouble_vector::CreateZeroVector(4);

	EXPECT_EQ(zeroVector, v1 + v2);

	auto v3 = -v1;
	EXPECT_EQ(zeroVector, v1 + v3);
}

TEST_F(cvectorTest, Multiply)
{
	cdouble_vector v({ "16+2.3i", "-7i", "6", "5-4i" }); // exercise 2.1.3
	cdouble scalar("8-2i");

	cdouble_vector product({ "132.6-13.6i", "-14-56i", "48-12i", "32-42i" });

	EXPECT_EQ(product, v * scalar);
	EXPECT_EQ(product, scalar * v);
}

TEST_F(cvectorTest, InnerProduct)
{
	cint_vector a({"5", "3", "-7"});
	cint_vector b({ "6", "2", "0" });

	EXPECT_EQ(cint(36), a * b);

	EXPECT_ANY_THROW(a * cint_vector({std::string("1"), "2"}));
}

TEST_F(cvectorTest, InnerProduct_exercise_2_4_1)
{
	cint_vector v1({ "2", "1", "3" });
	cint_vector v2({ "6", "2", "4" });
	cint_vector v3({ "0", "-1", "2" });
	cint c("23-33i");

	EXPECT_EQ((v1 + v2) * v3, v1 * v3 + v2 * v3); // 2.101
	EXPECT_EQ(v1 * (v2 + v3), v1 * v2 + v1 * v3); // 2.102

	EXPECT_EQ((c * v1) * v2, c.Conjugate() * (v1 * v2)); // 2.103
	EXPECT_EQ(v1 * (c * v2), c * (v1 * v2)); // 2.104

	EXPECT_EQ(v1 * v2, (v2 * v1).Conjugate()); // 2.105
}

TEST_F(cvectorTest, TensorProduct)
{
	cint_vector v1({ std::string("2"), "3" });
	cint_vector v2({ "4", "6", "3" });

	EXPECT_EQ(cint_vector({ "8", "12", "6", "12", "18", "9" }), v1.TensorProduct(v2));

	// 2.171
	v1 = cint_vector({ std::string("1"), "0" }).TensorProduct(cint_vector({ "8", "0", "0" }));
	v2 = cint_vector({ std::string("0"), "6" }).TensorProduct(cint_vector({ "0", "0", "3" }));
	EXPECT_EQ(cint_vector({ "8", "0", "0", "0", "0", "18" }), v1 + v2);

	//exercise 2.7.1
	v1 = cint_vector({ "3", "4", "7" });
	v2 = cint_vector({ std::string("-1"), "2" });

	EXPECT_EQ(cint_vector({ "-3", "6", "-4", "8", "-7", "14" }), v1.TensorProduct(v2));
}

TEST_F(cvectorTest, Norm)
{
	cint_vector v({ "3", "-6", "2" });

	EXPECT_EQ(cint(7), v.Norm());
	EXPECT_EQ(7, static_cast<int>(v.Norm()));

	cdouble_vector v2({ "4+3i", "6-4i", "12-7i", "13i" });
	EXPECT_EQ(cdouble(sqrt(439)), v2.Norm()); // exercise 2.4.5
}

TEST_F(cvectorTest, exercise_2_4_7) // tests Distance
{
	cdouble_vector v1({ "3", "1", "2" });
	cdouble_vector v2({ "2", "2", "-1" });

	EXPECT_EQ(cdouble(sqrt(11)), v1.Distance(v2));
}

TEST_F(cvectorTest, exercise_2_4_8)
{
	cdouble_vector V({ "3", "-1", "0" });
	cdouble_vector Vprime({ "2", "-2", "1" });

	double cos_angle = ((V * Vprime) / (V.Norm() * Vprime.Norm())).Real();
	double answer = acos(cos_angle) / M_PI * 180; // rad to degrees

	EXPECT_NEAR(32.512517, answer, 0.000001);
}

TEST_F(cvectorTest, CreateZeroVector)
{
	cint_vector zeroVector = cint_vector::CreateZeroVector(4);

	ASSERT_EQ(4, zeroVector.size());
	for (auto value : zeroVector)
	{
		EXPECT_EQ(cint(), value);
	}
}

TEST_F(cvectorTest, exercise_4_1_1)
{
	cdouble_vector state({"2-i", "2i", "1-i", "1", "-2i", "2"});
	EXPECT_NEAR(1 / pow(4.4721,2), cdouble("1").ModulusSquared() / state.NormSquare().Real(), 0.00001);
}

TEST_F(cvectorTest, exercise_4_1_3)
{
	cdouble_vector v1({ std::string("1+i"), "2-i" });
	cdouble_vector v2({ std::string("2+2i"), "1-2i" });

	cdouble c0 = v2[0] / v1[0];
	cdouble c1 = v2[1] / v1[1];

	EXPECT_NE(c0, c1);
}

TEST_F(cvectorTest, Normalize) // example 4.1.3
{
	cdouble_vector v;
	v.FromStringList({ "2-3i", "1+2i" });
	cdouble_vector normalized;
	normalized.FromStringList({ "0.471405-0.707107i", "0.235702+0.471405i" });

	EXPECT_TRUE(normalized.NearEquals(v.Normalize(), 0.00001));
}

TEST_F(cvectorTest, example_4_1_4)
{
	cdouble_vector ket;
	ket.FromStringList({ "3-4i", "7+2i" });

	cdouble lengthSquare = ket.LengthSquare();

	double probabilityUp = cdouble("3-4i").ModulusSquared() / lengthSquare.Real();
	double probabilityDn = cdouble("7+2i").ModulusSquared() / lengthSquare.Real();

	EXPECT_EQ(25.0 / 78, probabilityUp);
	EXPECT_EQ(53.0 / 78, probabilityDn);
}

TEST_F(cvectorTest, exercise_4_1_6)
{

}