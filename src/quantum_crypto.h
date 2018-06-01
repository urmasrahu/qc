#pragma once

#include <vector>

namespace QuantumCrypto
{
	struct DataUnit
	{
		char m_bit = 0; // 0 or 1, so also could be bool
		bool m_base = false; // false == '+', true == 'X', could be enum
	};

	typedef std::vector<DataUnit> DataPackage;
	typedef std::vector<size_t> IndexVector;

	DataPackage CreateRandomPackage(size_t size);
	DataPackage ReceiveWithRandomBases(const DataPackage & source);
	IndexVector CompareBases(const DataPackage & a, const DataPackage & b); // returns the vector of indexes to valid bits (sent and measured in the same basis)
	void RandomizeIndexVector(IndexVector & indexVector);

	unsigned AgreementPercentage(const DataPackage & a, const DataPackage & b);
	unsigned AgreementPercentage(const DataPackage & a, const DataPackage & b, const IndexVector & validBits);

	char RandomBit();
	bool RandomBase();
	size_t RandomIndex(size_t limit); // generates a random number from 0 to limit-1

	std::string ToBitString(const DataPackage & package);
	std::string ToBaseString(const DataPackage & package);
}
