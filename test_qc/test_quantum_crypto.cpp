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


TEST_F(QuantumCrypto_Test, BB84) // BB84 quantum key exchange protocol, starts on page 268
{
	const size_t n = 8000; // this must be large enough to avoid the possibility of coincidental matches when Eve is in the picture

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

	IndexVector validBits = CompareBases(sentByAlice, receivedByBob); // via public channel

	IndexVector comparedValidBits = validBits; // this chooses random half of the valid bits for public comparison
	RandomizeIndexVector(comparedValidBits);
	validBits.resize(comparedValidBits.size() / 2);
	unsigned agreementPercentage = AgreementPercentage(sentByAlice, receivedByBob, comparedValidBits);

	std::cout << "Agreement on " << comparedValidBits.size() << " randomly chosen valid bits: " << agreementPercentage << "%\n";

	EXPECT_EQ(100, agreementPercentage); // because we have no noise, this must be 100%

	// now, the scenario where Eve is eavesdropping :)

	DataPackage receivedByEve = ReceiveWithRandomBases(sentByAlice);
	std::cout << "Eve receives:\n" <<
		ToBitString(receivedByBob) << "\n" <<
		ToBaseString(receivedByBob) << "\n";

	receivedByBob = ReceiveWithRandomBases(receivedByEve); // Eve forwards the observed message to Bob in the same basis

	std::cout << "Bob receives the message eavesdropped and resent by Eve:\n" <<
		ToBitString(receivedByBob) << "\n" <<
		ToBaseString(receivedByBob) << "\n";

	std::cout << "Agreement: " << AgreementPercentage(sentByAlice, receivedByBob) << "%\n";

	validBits = CompareBases(sentByAlice, receivedByBob); // via public channel

	comparedValidBits = validBits;
	RandomizeIndexVector(comparedValidBits);
	validBits.resize(comparedValidBits.size() / 2);
	agreementPercentage = AgreementPercentage(sentByAlice, receivedByBob, comparedValidBits);

	std::cout << "Agreement on " << comparedValidBits.size() << " randomly chosen valid bits: " << agreementPercentage << "%\n";

	EXPECT_LT(agreementPercentage, 100);
}
