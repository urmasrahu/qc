#include <gtest\gtest.h>
#include <iostream>

#include "qc_algorithms.h"
#include "matrix_constants.h"
#include "print_util.h"

using namespace QC_Algorithms;
using namespace testing;

class QC_Algorithms_Test : public Test
{
public:
	QC_Algorithms_Test() = default;
};


TEST_F(QC_Algorithms_Test, example_4_1_5)
{
	cdouble_vector ket;
	ket.FromStringList({"3", "1-2i"});

	cdouble_vector bra;
	bra.FromStringList({"3", "1+2i"});

	EXPECT_EQ(bra, BraFromKet(ket));
}

TEST_F(QC_Algorithms_Test, example_4_1_6)
{
	cdouble_vector state1;
	state1.FromStringList({"1", "i"});

	cdouble_vector state2;
	state2.FromStringList({"i", "-1"});

	double factor = sqrt(2) / 2; // just to set it up as in the exercise
	state1 *= factor;
	state2 *= factor;

	//PrintVectorToConsole(state2, "state2");
	//PrintVectorToConsole(BraFromKet(state2), "bra of state 2");
	EXPECT_EQ(cdouble("-i"), TransitionAmplitude(state1, state2));

	// Is the explanation wrong in the book? Bra is the conjugate of end state; the transition amplitude
	// should either be the MATRIX PRODUCT of new state bra with old state, or the inner product of new
	// state and old state -- NOT the inner product of bra and ket.
}

TEST_F(QC_Algorithms_Test, exercise_4_1_9)
{
	cdouble_vector ket;
	ket.FromStringList({ "3+i", "-2i" });

	cdouble_vector bra;
	bra.FromStringList({ "3-i", "2i" });

	EXPECT_EQ(bra, BraFromKet(ket));
}

TEST_F(QC_Algorithms_Test, example_4_1_7)
{
	cdouble_vector state1;
	state1.FromStringList({ "1", "-i" });

	cdouble_vector state2;
	state2.FromStringList({ "i", "1" });

	EXPECT_EQ(cdouble("-i"), TransitionAmplitude(state1, state2));
}

TEST_F(QC_Algorithms_Test, example_4_5_1)
{
	cdouble_vector state({ "i", "1-i", "2", "-1-i" });
	double answer = ObservationProbability(state, 3);
	//std::cout << "Answer: " << answer << "\n";
	EXPECT_NEAR(0.2222, answer, 0.0001);
}

TEST_F(QC_Algorithms_Test, exercise_4_5_1)
{
	cdouble_vector state(16, cdouble("1+i"));
	double answer = ObservationProbability(state, 0);
	//std::cout << "Answer: " << answer << "\n"; // can be any position as all have identical values
	EXPECT_NEAR(0.0625, answer, 0.0001);
}

TEST_F(QC_Algorithms_Test, exercise_6_1_2)
{
  cdouble_matrix U({
    { 0, 1, 0, 0 },
    { 1, 0, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 }
    });

  EXPECT_EQ(U, U.Conjugate());
  EXPECT_EQ(cdouble_matrix::CreateIdentityMatrix(4), U * U);
}


TEST_F(QC_Algorithms_Test, phase_inversion) // see pages 196-197
{
	cdouble_matrix Uf
		({
			{ 1, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 1, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 1, 0, 0 },
			{ 0, 0, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 1, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 1 }
			});

	cdouble_matrix inputs = FillWithBinaryVectorsInOrder(3);
	PrintUtil::PrintMatrixToConsole(inputs, "inputs");

	cdouble_matrix outputs = Uf * inputs;
	PrintUtil::PrintMatrixToConsole(outputs, "outputs, note that '10' was found at inputs 4 and 5 (0-based)");

	cdouble a(M_SQRT1_2), b(-M_SQRT1_2); // from here on this is wrong, I have no idea what I'm doing
	cdouble * ptr = &a;

	cdouble_matrix inputsBottomQubitInSuperPosition = inputs;
	for (cdouble_vector & input : inputsBottomQubitInSuperPosition)
	{
		input[2] = *ptr;
		ptr = (ptr == &a) ? &b : &a;
	}
	PrintUtil::PrintMatrixToConsole(inputsBottomQubitInSuperPosition, "inputsBottomQubitInSuperPosition");

	cdouble_matrix I = cdouble_matrix::CreateIdentityMatrix(2);
	cdouble_matrix H = MatrixConstants::HADAMARD_2;

	cdouble_matrix IH_tensorProduct = I.TensorProduct(H);
	PrintUtil::PrintMatrixToConsole(IH_tensorProduct, "IH_tensorProduct");

	cdouble_matrix outputs2 = Uf * inputsBottomQubitInSuperPosition;
	PrintUtil::PrintMatrixToConsole(outputs2, "outputs2");
}

TEST_F(QC_Algorithms_Test, inversion_about_mean) // see page 198
{
	cdouble_vector V({ 53, 38, 17, 23, 79 });

	cdouble_matrix A = AveragerMatrix(5);

	cdouble_vector AV = (A * V);
	for (auto value : AV)
		EXPECT_EQ(42, value.Real());

	cdouble_vector expectedResultInversion({ 31, 46, 67, 61, 5 });

	cdouble_vector Vprime = -V + 2 * A * V;
	EXPECT_EQ(expectedResultInversion, Vprime);

	// alternative calculation
	cdouble_matrix I = cdouble_matrix::CreateIdentityMatrix(5);
	cdouble_vector VprimeAlt = (-I + 2 * A) * V;
	//EXPECT_EQ(expectedResultInversion, VprimeAlt, 0.0001); // this fails as they are not EXACTLY equal
	EXPECT_TRUE(expectedResultInversion.NearEquals(VprimeAlt, 0.0001));

	EXPECT_EQ(A ^ 2, A); // exercise 6.4.3
}
