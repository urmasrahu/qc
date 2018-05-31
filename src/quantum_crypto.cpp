#include "quantum_crypto.h"

#include <stdlib.h>

using namespace QuantumCrypto;

DataPackage QuantumCrypto::CreateRandomPackage(size_t size)
{
	DataPackage result(size);

	for (auto & element : result)
	{
		element.m_bit = RandomBit();
		element.m_base = RandomBase();
	}

	return result;
}

DataPackage QuantumCrypto::ReceiveWithRandomBases(const DataPackage & source)
{
	size_t n = source.size();

	DataPackage result(n);

	for (size_t i = 0; i < n; i++)
	{
		bool randomBase = RandomBase();

		const BitAndBase & src = source[i];

		result[i].m_base = randomBase;
		result[i].m_bit = randomBase == src.m_base ? src.m_bit : RandomBit();
	}

	return result;
}

unsigned QuantumCrypto::AgreementPercentage(const DataPackage & a, const DataPackage & b)
{
	size_t n = a.size(); // assumed equal to b.size() and must not be zero, no error handling

	unsigned matches = 0;
	for (size_t i = 0; i < n; i++)
		if (a[i].m_bit == b[i].m_bit)
			matches++;

	return 100 * matches / n;
}

char QuantumCrypto::RandomBit()
{
	return rand() % 2;
}

bool QuantumCrypto::RandomBase()
{
	return rand() % 2 ? false : true;
}

std::string QuantumCrypto::ToBitString(const DataPackage & package)
{
	std::string result;

	for (const auto & element : package)
		result += element.m_bit ? '1' : '0';

	return result;
}

std::string QuantumCrypto::ToBaseString(const DataPackage & package)
{
	std::string result;

	for (const auto & element : package)
		result += element.m_base ? 'X' : '+';

	return result;
}
