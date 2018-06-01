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

		const DataUnit & src = source[i];

		result[i].m_base = randomBase;
		result[i].m_bit = randomBase == src.m_base ? src.m_bit : RandomBit();
	}

	return result;
}

IndexVector QuantumCrypto::CompareBases(const DataPackage & a, const DataPackage & b)
{
	size_t n = a.size(); // assumed equal to b.size()

	IndexVector result(n);

	size_t validBitsCount = 0;

	for (size_t i = 0; i < n; i++)
	{
		if (b[i].m_base == a[i].m_base)
			result[validBitsCount++] = i;
	}

	result.resize(validBitsCount); // because we constructed with max possible count, size of the package

	return result;
}

void QuantumCrypto::RandomizeIndexVector(IndexVector & indexVector)
{
	size_t n = indexVector.size();

	for (size_t i = 0; i < n; i++)
	{
		size_t randomIndex = RandomIndex(n);

		size_t swapSave = indexVector[i];
		indexVector[i] = indexVector[randomIndex];
		indexVector[randomIndex] = swapSave;
	}
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

unsigned QuantumCrypto::AgreementPercentage(const DataPackage & a, const DataPackage & b, const IndexVector & validBits)
{
	size_t n = validBits.size();

	unsigned matches = 0;

	for (size_t i = 0; i < n; i++)
	{
		size_t index = validBits[i];

		if (a[index].m_bit == b[index].m_bit)
			matches++;
	}

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

size_t QuantumCrypto::RandomIndex(size_t limit)
{
	return rand() % limit;
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
