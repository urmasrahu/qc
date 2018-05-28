#include <gtest\gtest.h>
#include <iostream>

#include "qc_algorithms.h"

using namespace QC_Algorithms;
using namespace testing;

class QC_Algorithms_Test : public Test
{
public:
	QC_Algorithms_Test() = default;

	//TBD: duplicate with test_cvector, move to some "util" class
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