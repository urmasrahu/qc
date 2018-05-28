#include <gtest\gtest.h>
#include <iostream>

#include "cmatrix.h"

using namespace testing;

class cmatrixTest : public Test
{
public:
	cmatrixTest() = default;

	template <typename T> void PrintMatrixToConsole(const complex_matrix<T> & matrix, const char * title=nullptr)
	{
		if (title)
			std::cout << title << "\n";

		std::vector<std::vector<std::string>> dbg = matrix.ToStringListList();
		for (auto row : dbg)
		{
			for (auto value : row)
			{
				std::cout << value << " ";
			}
			std::cout << "\n";
		}
	}

	template <typename T> void PrintVectorToConsole(const complex_vector<T> & vector, const char * title = nullptr) // TBD: duplicate code with cvectorTest, put elsewhere
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


namespace
{
	std::vector<std::vector<std::string>> MATRIX_DATA_2_2_3 // from exercise 2.2.3
	(
		{ { "1-i", "2+2i" },{ "3", "4+i" } }
	);

	std::vector<std::vector<std::string>> MATRIX_DATA_2_2_5 // from exercise 2.2.5
	(
		{ { "6-3i", "0", "1" },{ "2+12i", "5+2.1i", "2+5i" },{ "-19i", "17", "3-4.5i" } }
	);
	std::vector<std::vector<std::string>> MATRIX_DATA_2_2_5_CONJUGATED
	(
		{ { "6+3i", "0", "1" },{ "2-12i", "5-2.1i", "2-5i" },{ "19i", "17", "3+4.5i" } }
	);
	std::vector<std::vector<std::string>> MATRIX_DATA_2_2_5_TRANSPOSED
	(
		{ { "6-3i", "2+12i", "-19i" },{ "0", "5+2.1i", "17" },{ "1", "2+5i", "3-4.5i" } }
	);
	std::vector<std::vector<std::string>> MATRIX_DATA_2_2_5_ADJOINTED
	(
		{ { "6+3i", "2-12i", "19i" },{ "0", "5-2.1i", "17" },{ "1", "2-5i", "3+4.5i" } }
	);

	std::vector<std::vector<std::string>> MATRIX_DATA_2_2_5_SHORTENED // from exercise 2.2.5, but removed one vector so we have 2x3 matrix; also it has integers only
	(
		{ { "6-3i", "0", "1" },{ "2+12i", "5+2i", "2+5i" } }
	);

	std::vector<std::vector<std::string>> MATRIX_DATA_MULTIPLY_A // from 2.33
	(
		{ { "3+2i", "0", "5-6i" },{ "1", "4+2i", "i" },{ "4-i", "0", "4" } }
	);
	std::vector<std::vector<std::string>> MATRIX_DATA_MULTIPLY_B
	(
		{ { "5", "2-i", "6-4i" },{ "0", "4+5i", "2" },{ "7-4i", "2+7i", "0" } }
	);
	std::vector<std::vector<std::string>> MATRIX_DATA_MULTIPLY_AB
	(
		{ { "26-52i", "60+24i", "26" },{ "9+7i", "1+29i", "14" },{ "48-21i", "15+22i", "20-22i" } }
	);
}


TEST_F(cmatrixTest, CtrFromVector)
{
	cint_vector V({ "2+i", "3-4i", "23" });
	cint_matrix M = cint_matrix::CreateFromVector(V);

	EXPECT_EQ(3, M.Rows());
	EXPECT_EQ(1, M.Cols());
	EXPECT_EQ(cint("2+i"), M[0][0]);
	EXPECT_EQ(cint("3-4i"), M[1][0]);
	EXPECT_EQ(cint("23"), M[2][0]);
}

TEST_F(cmatrixTest, CtrFromStringListList)
{
	cdouble_matrix m(MATRIX_DATA_2_2_5);

	ASSERT_EQ(3, m.size());

	ASSERT_EQ(3, m[0].size());
	ASSERT_EQ(3, m[1].size());
	ASSERT_EQ(3, m[2].size());

	EXPECT_EQ(cdouble("6-3i"), m[0][0]);
	EXPECT_EQ(cdouble("0"), m[0][1]);
	EXPECT_EQ(cdouble("1"), m[0][2]);

	EXPECT_EQ(cdouble("2+12i"), m[1][0]);
	EXPECT_EQ(cdouble("5+2.1i"), m[1][1]);
	EXPECT_EQ(cdouble("2+5i"), m[1][2]);

	EXPECT_EQ(cdouble("-19i"), m[2][0]);
	EXPECT_EQ(cdouble("17"), m[2][1]);
	EXPECT_EQ(cdouble("3-4.5i"), m[2][2]);

	m.FromStringListList(MATRIX_DATA_2_2_5);
	EXPECT_EQ(3, m.size());

	cint_matrix small({
		{"1"}
		});
	EXPECT_EQ(1, small.Rows());
	EXPECT_EQ(1, small.Cols());

	cint_matrix m_1x3({
		{ "1", "1", "1" }
		});
	EXPECT_EQ(1, m_1x3.Rows());
	EXPECT_EQ(3, m_1x3.Cols());

	cint_matrix m_3x1({
		{ "1" },
		{ "1" },
		{ "1" }
		});
	EXPECT_EQ(3, m_3x1.Rows());
	EXPECT_EQ(1, m_3x1.Cols());

	// IMPORTANT: be wary of creating lists of two items, as the compiler will create a vector of raw pointers instead of string list, so explicit construction is needed
	cint_matrix m_1x2(std::vector<std::vector<std::string>>({
		{ "1", "1" }
		}));
	EXPECT_EQ(1, m_1x2.Rows());
	EXPECT_EQ(2, m_1x2.Cols());
}

TEST_F(cmatrixTest, Add_Subtract)
{
	cdouble_matrix m1(MATRIX_DATA_2_2_5);
	cdouble_matrix m2(MATRIX_DATA_2_2_5);

	cdouble_matrix sum = m1 + m2;

	ASSERT_EQ(3, sum.size());

	ASSERT_EQ(3, sum[0].size());
	ASSERT_EQ(3, sum[1].size());
	ASSERT_EQ(3, sum[2].size());

	EXPECT_EQ(cdouble("12-6i"), sum[0][0]);
	EXPECT_EQ(cdouble("0"), sum[0][1]);
	EXPECT_EQ(cdouble("2"), sum[0][2]);

	EXPECT_EQ(cdouble("4+24i"), sum[1][0]);
	EXPECT_EQ(cdouble("10+4.2i"), sum[1][1]);
	EXPECT_EQ(cdouble("4+10i"), sum[1][2]);

	EXPECT_EQ(cdouble("-38i"), sum[2][0]);
	EXPECT_EQ(cdouble("34"), sum[2][1]);
	EXPECT_EQ(cdouble("6-9i"), sum[2][2]);

	EXPECT_EQ(m1, sum - m2);
}

TEST_F(cmatrixTest, Multiply_matrix)
{
	cint_matrix A(MATRIX_DATA_MULTIPLY_A), B(MATRIX_DATA_MULTIPLY_B);
	cint_matrix C = A * B;

	EXPECT_EQ(cint_matrix(MATRIX_DATA_MULTIPLY_AB), C);

	// now testing multiplication properties

	EXPECT_NE(A * B, B * A);

	EXPECT_EQ((A * B) * C, A * (B * C)); // i, associative

	cint_matrix I = cint_matrix::CreateIdentityMatrix(3); // ii, identity unit
	EXPECT_EQ(I * A, A);
	EXPECT_EQ(A * I, A);

	EXPECT_EQ(A * (B + C), (A*B) + (A* C)); // iii, distribution over addition

	cint c("2+12i");
	EXPECT_EQ(c * (A * B), (c * A) * B); // iv, respects scalar multiplication

	EXPECT_EQ((A * B).Transpose(), B.Transpose() * A.Transpose()); // v, relates to transpose

	EXPECT_EQ((A * B).Conjugate(), A.Conjugate() * B.Conjugate()); // vi, relates to conjugate

	EXPECT_EQ((A * B).Adjoint(), B.Adjoint() * A.Adjoint()); // vii, relates to adjoint
}

TEST_F(cmatrixTest, Multiply_matrix_invalid)
{
	cint_matrix M1({
		{ std::string("1"), "1" },
		{ "1", "1" }
		});

	cint_matrix M2({
		{ "1", "1", "1" },
		{ "1", "1", "1" },
		{ "1", "1", "1" }
		});

	EXPECT_ANY_THROW(M1 * M2);
}

TEST_F(cmatrixTest, Power)
{
	cint_matrix M({ // 3.4
		{ "0", "0", "0" , "0" , "0" , "0" },
		{ "0", "0", "0" , "0" , "0" , "0" },
		{ "0", "1", "0" , "0" , "0" , "1" },
		{ "0", "0", "0" , "1" , "0" , "0" },
		{ "0", "0", "1" , "0" , "0" , "0" },
		{ "1", "0", "0" , "0" , "1" , "0" }
		});

	cint_matrix X({ {"6", "2", "1" , "5" , "3" , "10"} });

	// first just test multiply here as in 3.4, so this essentially tests matrix-to-vector multiplication

	cint_matrix Y = M * X.Transpose();

	EXPECT_EQ(cint_matrix({ { "0", "0", "12" , "5" , "1" , "9" } }).Transpose(), Y);

	// exercise 3.1.2

	cint_matrix M_power6_expected({
		{ "0", "0", "0" , "0" , "0" , "0" },
		{ "0", "0", "0" , "0" , "0" , "0" },
		{ "0", "0", "1" , "0" , "0" , "0" },
		{ "0", "0", "0" , "1" , "0" , "0" },
		{ "1", "0", "0" , "0" , "1" , "0" },
		{ "0", "1", "0" , "0" , "0" , "1" }
		});

	EXPECT_EQ(M_power6_expected, M.Power(6));
	//PrintMatrixToConsole(M.Power(6));

	// additional tests, special cases of 0 and 1 powers
	EXPECT_EQ(cint_matrix::CreateIdentityMatrix(6), M.Power(0));
	EXPECT_EQ(M, M.Power(1));
}

TEST_F(cmatrixTest, Conjugate) // exercise 2.2.5
{
	cdouble_matrix m(MATRIX_DATA_2_2_5);
	EXPECT_EQ(cdouble_matrix(MATRIX_DATA_2_2_5_CONJUGATED), m.Conjugate());
}

TEST_F(cmatrixTest, Inverse_and_CreateZeroMatrix)
{
	cdouble_matrix zeroMatrix = cdouble_matrix::CreateZeroMatrix(3, 3);
	ASSERT_EQ(3, zeroMatrix.size());
	for (auto vector : zeroMatrix)
	{
		ASSERT_EQ(3, vector.size());
		for (auto value : vector)
			EXPECT_EQ(cdouble(), value);
	}

	cdouble_matrix m(MATRIX_DATA_2_2_5);
	cdouble_matrix inverted = -m;
	EXPECT_EQ(zeroMatrix, m + inverted);
}

TEST_F(cmatrixTest, exercise_2_4_3) // tests Trace and InnerProduct; modified a bit to use some complex numbers
{
	cint_matrix A({ {std::string("1"), "2+3i"}, {"0", "1-i"} });
	cint_matrix B({ { std::string("0"), "-1"},{"-1", "0"} });
	cint_matrix C({ { std::string("2"), "1"},{"1", "3"} });

	EXPECT_EQ((A + B).InnerProduct(C), A.InnerProduct(C) + B.InnerProduct(C)); // 2.101
	EXPECT_EQ(A.InnerProduct(B + C), A.InnerProduct(B) + A.InnerProduct(C)); // 2.102

	EXPECT_EQ(cint(2, -1), A.Trace());
	EXPECT_EQ(cint(0), B.Trace());
	EXPECT_EQ(cint(5), C.Trace());
}

TEST_F(cmatrixTest, TensorProduct) // exercise 2.7.3
{
	cint_matrix A({ {"3+2i", "5-i", "2i"}, {"0", "12", "6-3i"}, {"2", "4+4i", "9+3i"} });
	cint_matrix B({ {"1", "3+4i", "5-7i" },{ "10+2i", "6", "2+5i" },{ "0", "1", "2+9i" } });

	cint_matrix C({
		{ "3+2i", "1+18i", "29-11i", "5-i", "19+17i", "18-40i", "2i", "-8+6i", "14+10i"},
		{ "26+26i", "18+12i", "-4+19i", "52", "30-6i", "15+23i", "-4+20i", "12i", "-10+4i" },
		{ "0", "3+2i", "-12+31i", "0", "5-i", "19+43i", "0", "2i", "-18+4i" },
		{ "0", "0", "0", "12", "36+48i", "60-84i", "6-3i", "30+15i", "9-57i" },
		{ "0", "0", "0", "120+24i", "72", "24+60i", "66-18i", "36-18i", "27+24i" },
		{ "0", "0", "0", "0", "12", "24+108i", "0", "6-3i", "39+48i" },
		{ "2", "6+8i", "10-14i", "4+4i", "-4+28i", "48-8i", "9+3i", "15+45i", "66-48i" },
		{ "20+4i", "12", "4+10i", "32+48i", "24+24i", "-12+28i", "84+48i", "54+18i", "3+51i" },
		{ "0", "2", "4+18i", "0", "4+4i", "-28+44i", "0", "9+3i", "-9+87i" }
		});

	//cint_matrix P = A.TensorProduct(B);

	//for (size_t i=0; i<C.Rows(); i++)
	//	for (size_t j = 0; j < C.Cols(); j++)
	//	{
	//		if (C[i][j] != P[i][j])
	//		{
	//			std::cout << "**************** " << i << "," << j << "\n";
	//		}
	//	}

	EXPECT_EQ(C, A.TensorProduct(B));
}


TEST_F(cmatrixTest, exercise_2_4_6) // tests Norm
{
	cdouble_matrix A({ {std::string("3"), "5"}, {"2", "3"} });

	EXPECT_EQ(cdouble(sqrt(47)), A.Norm());
}

TEST_F(cmatrixTest, CreateIdentityMatrix)
{
	const size_t size = 47;
	cint_matrix m = cint_matrix::CreateIdentityMatrix(size);

	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			EXPECT_EQ(i == j ? cint(1, 0) : cint(0, 0), m[i][j]);
}

TEST_F(cmatrixTest, exercise_2_2_3)
{
	cint_matrix m(MATRIX_DATA_2_2_3);
	cint c1("2i"), c2("1+2i");

	EXPECT_EQ(c1 * (c2 * m), (c1 * c2) * m); // vi
	EXPECT_EQ((c1 + c2) * m, c1 * m + c2 * m); // viii
}

TEST_F(cmatrixTest, Transpose) // exercise 2.2.5
{
	cdouble_matrix m(MATRIX_DATA_2_2_5);
	EXPECT_EQ(cdouble_matrix(MATRIX_DATA_2_2_5_TRANSPOSED), m.Transpose());
}

TEST_F(cmatrixTest, Adjoint) // exercise 2.2.5
{
	cdouble_matrix m(MATRIX_DATA_2_2_5);
	EXPECT_EQ(cdouble_matrix(MATRIX_DATA_2_2_5_ADJOINTED), m.Adjoint());

}

TEST_F(cmatrixTest, exercise_2_2_6)
{
	cint_matrix m(MATRIX_DATA_2_2_5_SHORTENED);
	cint c("1+2i");

	EXPECT_EQ((c * m).Conjugate(), c.Conjugate() * m.Conjugate());
}

TEST_F(cmatrixTest, exercise_2_2_7)
{
	cint_matrix m(MATRIX_DATA_2_2_5_SHORTENED);
	cint_matrix n = m + m; // just to create some matrix that is not identical with m
	cint c("345-4982i");

	EXPECT_EQ(m, m.Adjoint().Adjoint()); // vii
	EXPECT_EQ((m + n).Adjoint(), m.Adjoint() + n.Adjoint()); // viii
	EXPECT_EQ((c * m).Adjoint(), c.Conjugate() * m.Adjoint()); // ix
}

TEST_F(cmatrixTest, probabilistic_systems) // 3.2
{
	cdouble_matrix W(1, 3); // 3.16
	W[0][0] = 1.0 / 3;
	W[0][1] = 0;
	W[0][2] = 2.0 / 3;

	cdouble_matrix M(3, 3);
	M[0][0] = 0;
	M[0][1] = 1.0/6;
	M[0][2] = 5.0/6;
	M[1][0] = 1.0/3;
	M[1][1] = 1.0/2;
	M[1][2] = 1.0/6;
	M[2][0] = 2.0/3;
	M[2][1] = 1.0/3;
	M[2][2] = 0;

	cdouble_matrix Z = W * M;

	EXPECT_EQ(Z.Transpose(), M.Transpose() * W.Transpose());
}

TEST_F(cmatrixTest, IsDoublyStochastic)
{
	cdouble_matrix M (3, 3);
	M[0][0] = 0;
	M[0][1] = 1.0 / 6;
	M[0][2] = 5.0 / 6;
	M[1][0] = 1.0 / 3;
	M[1][1] = 1.0 / 2;
	M[1][2] = 1.0 / 6;
	M[2][0] = 2.0 / 3;
	M[2][1] = 1.0 / 3;
	M[2][2] = 0;

	EXPECT_TRUE(M.IsDoublyStochastic());

	M[0][0] = 1.0 / 6;
	M[0][1] = 0;

	EXPECT_FALSE(M.IsDoublyStochastic());
}

TEST_F(cmatrixTest, exercise_3_2_6)
{
	cdouble_matrix M({
		{ "0.1", "0.7", "0.2" },
		{ "0.6", "0.2", "0.2" },
		{ "0.3", "0.1", "0.6" }
		});

	EXPECT_TRUE(M.IsDoublyStochastic());

	cdouble_matrix math = cdouble_matrix::CreateFromVector(cdouble_vector({ "1", "0", "0" }));
	cdouble_matrix physics = cdouble_matrix::CreateFromVector(cdouble_vector({ "0", "1", "0" }));
	cdouble_matrix compsci = cdouble_matrix::CreateFromVector(cdouble_vector({ "0", "0", "1" }));

	cdouble_matrix M2 = M.Power(2);
	cdouble_matrix M4 = M2.Power(2);
	cdouble_matrix M8 = M4.Power(2);

	//PrintMatrixToConsole(M2, "M2");
	//PrintMatrixToConsole(M4, "M4");
	//PrintMatrixToConsole(M8, "M8");

	// visual inspection as answers were not given

	PrintMatrixToConsole(M2 * math, "math students after 2 years:");
	PrintMatrixToConsole(M2 * physics, "physics students after 2 years:");
	PrintMatrixToConsole(M2 * compsci, "computer science students after 2 years:");

	PrintMatrixToConsole(M4 * math, "math students after 4 years:");
	PrintMatrixToConsole(M4 * physics, "physics students after 4 years:");
	PrintMatrixToConsole(M4 * compsci, "computer science students after 4 years:");

	PrintMatrixToConsole(M8 * math, "math students after 8 years:");
	PrintMatrixToConsole(M8 * physics, "physics students after 8 years:");
	PrintMatrixToConsole(M8 * compsci, "computer science students after 8 years:");

	for (auto result : { M8*math, M8*physics, M8*compsci }) // sanity check, after 8 years all students are still in the system :)
	{
		EXPECT_EQ(cdouble(1, 0), result.ColSum(0));
	}
}

TEST_F(cmatrixTest, exercise_3_4_3)
{
	cdouble_matrix M, N, RESULT;
	
	M.FromValueListList({
		{ 1.0 / 3, 2.0 / 3 },
		{ 2.0 / 3, 1.0 / 3 }
		});

	N.FromValueListList({
		{ 1.0 / 2, 1.0 / 2 },
		{ 1.0 / 2, 1.0 / 2 }
		});

	RESULT.FromValueListList({
		{ 1.0 / 6, 1.0 / 6, 2.0 / 6, 2.0 / 6 },
		{ 1.0 / 6, 1.0 / 6, 2.0 / 6, 2.0 / 6 },
		{ 2.0 / 6, 2.0 / 6, 1.0 / 6, 1.0 / 6 },
		{ 2.0 / 6, 2.0 / 6, 1.0 / 6, 1.0 / 6 }
		});

	EXPECT_EQ(RESULT, M.TensorProduct(N));
}

TEST_F(cmatrixTest, exercise_4_4_1) // tests IsUnitary
{
	cint_matrix U1;
	U1.FromValueListList({
		{ 0, 1 },
		{ 1, 0 }
		});

	EXPECT_TRUE(U1.IsUnitary());

	cdouble_matrix U2;

	double value = sqrt(2) / 2;
	U2.FromValueListList({
		{ value, value },
		{ value, -value }
		});

	EXPECT_TRUE(U2.IsUnitary());
}

TEST_F(cmatrixTest, exercise_4_4_2)
{
	cdouble_matrix U;

	cdouble valueReal(1/sqrt(2), 0);
	cdouble valueNegReal(-1 / sqrt(2), 0);
	cdouble valueImag(0, 1 / sqrt(2));

	U.FromValueListList({
		{ 0, valueReal, valueReal, 0 },
		{ valueImag, 0, 0, valueReal },
		{ valueReal, 0, 0, valueImag },
		{ 0, valueReal, valueNegReal, 0 }
		});

	//cdouble_matrix test = U * U.Conjugate().Transpose();
	//PrintMatrixToConsole(test);

	ASSERT_TRUE(U.IsUnitary());

	cdouble_matrix state[4];
	state[0].FromVector(cdouble_vector({ "1", "0", "0" , "0" }));

	for (size_t i = 0; i < 3; i++)
	{
		state[i + 1] = U * state[i];
		std::cout << "State after step " << i+1 << ":\n";
		PrintMatrixToConsole(state[i + 1]);
	}

	cdouble_vector endStateNormalized = state[3].ToVector().Normalize();
	PrintVectorToConsole(endStateNormalized, "Normalized end state:");

}