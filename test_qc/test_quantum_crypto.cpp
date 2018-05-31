#include <gtest\gtest.h>
#include <iostream>

#include "quantum_crypto.h"

using namespace QuantumCrypto;
using namespace testing;

class QuantumCrypto_Test : public Test
{
public:
	QuantumCrypto_Test() = default;
};


TEST_F(QuantumCrypto_Test, BB84) // starts on page 268
{
	const size_t n = 200;

	DataPackage sentByAlice = CreateRandomPackage(n);

	std::cout << "Alice sends:\n" <<
		ToBitString(sentByAlice) << "\n" <<
		ToBaseString(sentByAlice) << "\n";

	DataPackage receivedByBob = ReceiveWithRandomBases(sentByAlice);

	std::cout << "Bob receives:\n" <<
		ToBitString(receivedByBob) << "\n" <<
		ToBaseString(receivedByBob) << "\n";

	for (size_t i = 0; i < n; i++)
	{
		if (sentByAlice[i].m_base == receivedByBob[i].m_base)
		{
			EXPECT_EQ(sentByAlice[i].m_bit, receivedByBob[i].m_bit);
		}
	}

	std::cout << "Agreement: " << AgreementPercentage(sentByAlice, receivedByBob) << "%\n";
}

