#pragma once

#include <string>
#include <array>
#include <vector>
#include "protocol.h"
#include "blockstat.h"

struct BlockHeader
{
	uint32_t magicno;
	uint32_t blocksize;
	uint32_t version;
	std::array<uint8_t, PROT_HASH_LEN> hashPrevBlock;
	std::array<uint8_t, PROT_HASH_LEN> hashMerkleRoot;
	uint32_t time;
	uint32_t bits;
	uint32_t nonce;
};

class Block
{
private:
	BlockHeader blockHeader;
	std::vector<uint8_t> transactionCounter;
	std::vector<uint8_t> payload;

	std::string sha256Input;
	std::array<uint8_t, PROT_HASH_LEN> currentHash;

	Blockstat blockStats;

	Blockstat mine(void);

	template <typename InContainer>
	std::array<uint8_t, PROT_HASH_LEN> sha256(InContainer input);

public:
	Block();
	Block(std::array<uint8_t, PROT_HASH_LEN> prevHash, std::vector<uint8_t> payload);
	~Block();
	
	std::string getSHA256Input(void);
	BlockHeader & getBlockHeader(void);
	std::array<uint8_t, PROT_HASH_LEN> getHash(void);
	Blockstat getStats(void);
};
