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

	bool VectorsNearEqual(const std::vector<double> & a, const std::vector<double> & b)
	{
		size_t n = a.size();
		if (n != b.size())
			return false;

		for (size_t i = 0; i < n; i++)
		{
			double diff = abs(a[i] - b[i]);
			if (diff > DBL_EPSILON * 10)
				return false;
		}

		return true;
	}
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
	//PrintUtil::PrintMatrixToConsole(inputs, "inputs");

	cdouble_matrix outputs = Uf * inputs;
	//PrintUtil::PrintMatrixToConsole(outputs, "outputs, note that '10' was found at inputs 4 and 5 (0-based)");

	cdouble a(M_SQRT1_2), b(-M_SQRT1_2); // from here on this is wrong, I have no idea what I'm doing
	cdouble * ptr = &a;

	cdouble_matrix inputsBottomQubitInSuperPosition = inputs;
	for (cdouble_vector & input : inputsBottomQubitInSuperPosition)
	{
		input[2] = *ptr;
		ptr = (ptr == &a) ? &b : &a;
	}
	//PrintUtil::PrintMatrixToConsole(inputsBottomQubitInSuperPosition, "inputsBottomQubitInSuperPosition");

	cdouble_matrix I = cdouble_matrix::CreateIdentityMatrix(2);
	cdouble_matrix H = MatrixConstants::HADAMARD;

	cdouble_matrix IH_tensorProduct = I.TensorProduct(H);
	//PrintUtil::PrintMatrixToConsole(IH_tensorProduct, "IH_tensorProduct");

	cdouble_matrix outputs2 = Uf * inputsBottomQubitInSuperPosition;
	//PrintUtil::PrintMatrixToConsole(outputs2, "outputs2");
}

TEST_F(QC_Algorithms_Test, inversion_about_mean) // see page 198
{
	cdouble_vector V({ 53, 38, 17, 23, 79 });

	cdouble_matrix A = AveragerMatrix(5);

	cdouble_vector AV = (A * V);
	for (const auto & value : AV)
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

	// finally, use the InverseAboutMean() method of the namespace
	EXPECT_EQ(expectedResultInversion, InverseAboutMean(V));
}

TEST_F(QC_Algorithms_Test, example_6_4_1)
{
	cdouble_vector v(5, 10); // (6.125)

	v[3] = -v[3]; // phase inversion of one element
	EXPECT_EQ(cdouble_vector({ 10, 10, 10, -10, 10 }), v); // (6.126)

	v = InverseAboutMean(v);

	EXPECT_EQ(cdouble_vector({ 2, 2, 2, 22, 2 }), v); // (6.129)

	v[3] = -v[3]; // repeat (6.130)
	v = InverseAboutMean(v);

	EXPECT_EQ(cdouble_vector({ -7.6, -7.6, -7.6, 16.4, -7.6 }), v); // (6.133)
}

TEST_F(QC_Algorithms_Test, HadamardMatrix)
{
	cdouble_matrix h = HadamardMatrix(1);
	EXPECT_EQ(MatrixConstants::HADAMARD, h);

	h = HadamardMatrix(2);
	//PrintUtil::PrintMatrixToConsole(h, "H2");
	EXPECT_EQ(MatrixConstants::HADAMARD.TensorProduct(MatrixConstants::HADAMARD), h);
}

TEST_F(QC_Algorithms_Test, Grovers_algorithm)
{
	cdouble_matrix H = HadamardMatrix(3);

	cdouble_vector s(8); // s for "state"
	s[0] = 1;
	//PrintUtil::PrintVectorToConsole(s, "s1");
	EXPECT_EQ(cdouble_vector({1, 0, 0 , 0 , 0 , 0 , 0 , 0 }), s);

	s = H * s;
	//PrintUtil::PrintVectorToConsole(s, "s2");
	EXPECT_TRUE(s.NearEquals(cdouble_vector({ 0.353553, 0.353553, 0.353553, 0.353553, 0.353553, 0.353553, 0.353553, 0.353553 }), 0.0001));

	s[5] = -s[5];
	//PrintUtil::PrintVectorToConsole(s, "s3a (6.137)");
	EXPECT_TRUE(s.NearEquals(cdouble_vector({ 0.353553, 0.353553, 0.353553, 0.353553, 0.353553, -0.353553, 0.353553, 0.353553 }), 0.0001));

	s = InverseAboutMean(s);
	//PrintUtil::PrintVectorToConsole(s, "s3b (6.141)");
	EXPECT_TRUE(s.NearEquals(cdouble_vector({ 0.176777, 0.176777, 0.176777, 0.176777, 0.176777, 0.883883, 0.176777, 0.176777 }), 0.0001));

	s[5] = -s[5];
	EXPECT_TRUE(s.NearEquals(cdouble_vector({ 0.176777, 0.176777, 0.176777, 0.176777, 0.176777, -0.883883, 0.176777, 0.176777 }), 0.0001));

	s = InverseAboutMean(s);
	EXPECT_TRUE(s.NearEquals(cdouble_vector({ -0.08839, -0.08839, -0.08839, -0.08839, -0.08839, 0.97227, -0.08839, -0.08839, }), 0.0001));
}

TEST_F(QC_Algorithms_Test, PowersOfModulo)
{
	cint_vector v = PowersOfModulo(2, 15, 13);
	//PrintUtil::PrintVectorToConsole(v);
	EXPECT_EQ(cint_vector({ 1,2,4,8,1,2,4,8,1,2,4,8,1 }), v);

	v = PowersOfModulo(4, 15, 13);
	EXPECT_EQ(cint_vector({ 1,4,1,4,1,4,1,4,1,4,1,4,1 }), v);

	v = PowersOfModulo(13, 15, 13);
	//PrintUtil::PrintVectorToConsole(v);
	EXPECT_EQ(cint_vector({ 1,13,4,7,1,13,4,7,1,13,4,7,1 }), v);

	PrintUtil::PrintVectorToConsole(PowersOfModulo(6, 371, 8), "PowersOfModulo(6, 371, 8)");
}

TEST_F(QC_Algorithms_Test, example_7_2_4)
{
	cdouble_vector R = CreateQubitStateVectorAndInitializeToZero(2);
	cdouble_matrix U = HadamardMatrix(2);
	R = U * R;
	//PrintUtil::PrintVectorToConsole(R, "7.7");

	double probability11 = R[3].ModulusSquared();
	EXPECT_EQ(0.25, probability11);
}

TEST_F(QC_Algorithms_Test, exercise_7_2_4)
{
	cdouble_vector R = CreateQubitStateVectorAndInitializeToZero(2);
	cdouble_matrix U = HadamardMatrix(1).TensorProduct(cdouble_matrix::CreateIdentityMatrix(2));
	R = U * R;
	//PrintUtil::PrintVectorToConsole(R, "line 3");

	R = MatrixConstants::CNOT * R;
	//PrintUtil::PrintVectorToConsole(R, "line 6");

	std::vector<double> RES_probabilities = MeasurementProbabilitiesVector(R);
	//PrintUtil::PrintVectorToConsole(RES_probabilities, "line 7 (measurement probabilities)");

	EXPECT_TRUE(VectorsNearEqual(std::vector<double>({ 0.5, 0, 0, 0.5 }), RES_probabilities)); // no answer given in book but this is what I got

	for (int i=0; i<100; i++) // exercise 7.2.5
	{
		R = U * R;
		R = MatrixConstants::CNOT * R;
		RES_probabilities = MeasurementProbabilitiesVector(R);
		//PrintUtil::PrintVectorToConsole(RES_probabilities);

		if (RES_probabilities[2] > 0) // probability of "10" state
			break;
	}
}

TEST_F(QC_Algorithms_Test, quantum_computer_simultor)
{ // runs the exercise 7.2.4 through 10 simulations
	for (int i = 0; i < 10; i++)
	{
		//std::string outputDescription = "Simulation round ";
		//outputDescription += std::to_string(i);

		cdouble_vector R = CreateQubitStateVectorAndInitializeToZero(2);
		cdouble_matrix U = HadamardMatrix(1).TensorProduct(cdouble_matrix::CreateIdentityMatrix(2));

		R = U * R;

		R = MatrixConstants::CNOT * R;

		//PrintUtil::PrintVectorToConsole(R, outputDescription.c_str());
		size_t RES = Measure(R);
		std::cout << "Measured state: " << RES << "\n";
	}
}
