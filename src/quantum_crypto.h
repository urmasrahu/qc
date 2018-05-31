#pragma once

#include <vector>

namespace QuantumCrypto
{
	struct BitAndBase
	{
		char m_bit = 0; // 0 or 1, so also could be bool
		bool m_base = false; // false == '+', true == 'X', could be enum
	};

	typedef std::vector<BitAndBase> DataPackage;

	DataPackage CreateRandomPackage(size_t size);
	DataPackage ReceiveWithRandomBases(const DataPackage & source);
	unsigned AgreementPercentage(const DataPackage & a, const DataPackage & b);

	char RandomBit();
	bool RandomBase();

	std::string ToBitString(const DataPackage & package);
	std::string ToBaseString(const DataPackage & package);
}
