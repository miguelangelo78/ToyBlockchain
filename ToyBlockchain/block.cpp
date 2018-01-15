#include "picosha2.h"
#include "blockchain.h"
#include "util.h"
#include <random>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <iterator>

Block::Block() {

}

Block::Block(std::array<uint8_t, PROT_HASH_LEN> prevHash, std::vector<uint8_t> payload)
{
	Blockchain::debug("Creating " + std::string((Blockchain::chain.empty() ? "Genesis" : "new")) + " block . . .\n\t> Payload: '" + std::string(payload.begin(), payload.end()) + "'");

	blockHeader.magicno = PROT_MAGIC_NO;
	blockHeader.blocksize = 
		sizeof(blockHeader.magicno)   + 
		sizeof(blockHeader.blocksize) + 
		sizeof(blockHeader.version)   + 
		PROT_HASH_LEN * 2             + 
		sizeof(blockHeader.time)      + 
		sizeof(blockHeader.bits)      + 
		sizeof(blockHeader.nonce)     + 
		0 +
		payload.size();

	blockHeader.version = PROT_BLOCKCHAIN_VERSION;
	blockHeader.hashPrevBlock = prevHash;
	blockHeader.hashMerkleRoot = sha256(payload); /* TODO */
	blockHeader.time = (uint32_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	blockHeader.bits = (uint32_t)Blockchain::getDifficulty();
	blockHeader.nonce = 0;

	transactionCounter = std::vector<uint8_t>(1, 0); /* TODO */
	blockHeader.blocksize += transactionCounter.size();

	this->payload = payload;

	std::stringstream sha256Input_ss;
	sha256Input_ss << std::setfill('0') << std::setw(sizeof(uint32_t) * 2) << std::hex << swapByteOrder(blockHeader.version);

	for (int i = 0; i < PROT_HASH_LEN; i++)
		sha256Input_ss << std::setfill('0') << std::setw(sizeof(uint8_t) * 2) << std::hex << blockHeader.hashPrevBlock[i];

	for (int i = 0; i < PROT_HASH_LEN; i++)
		sha256Input_ss << std::setfill('0') << std::setw(sizeof(uint8_t) * 2) << std::hex << blockHeader.hashMerkleRoot[i];

	sha256Input_ss << std::setfill('0') << std::setw(sizeof(uint32_t) * 2) << std::hex << swapByteOrder(blockHeader.time);
	sha256Input_ss << std::setfill('0') << std::setw(sizeof(uint32_t) * 2) << std::hex << swapByteOrder(blockHeader.bits);

	sha256Input = sha256Input_ss.str();

	blockStats.setState(Blockstat::State::INIT);
	mine();
}

Block::~Block() {
	
}

Blockstat Block::mine(void) {
	if (blockHeader.nonce || blockStats.isValid())
		return blockStats;

	std::uniform_int_distribution<unsigned int> randomDistr(0, UINT32_MAX);

	blockStats.setState(Blockstat::State::MINING);
	
	while (1) {
		std::stringstream nonce_ss;
		nonce_ss << std::setfill('0') << std::setw(sizeof(uint32_t) * 2) << std::hex << swapByteOrder(blockHeader.nonce);
		std::string nonceStr = nonce_ss.str();

		currentHash = sha256(sha256(sha256Input + nonceStr));
		blockStats.incrementHashTrialCount();

		uint32_t zero_count = 0;

		for (int i = 0; i < PROT_HASH_LEN; i++)
			if (currentHash[i] == '0')
				zero_count++;
			else
				break;

		if (zero_count == blockHeader.bits) {
			blockStats.setState(Blockstat::State::MINED);
			sha256Input += nonceStr;
			break;
		}
		else {
			blockHeader.nonce = randomDistr(Blockchain::randomGen);
		}
	}

	return blockStats;
}

template <typename InContainer>
std::array<uint8_t, PROT_HASH_LEN> Block::sha256(InContainer input) {
	std::array<uint8_t, PROT_HASH_LEN> hash;
	picosha2::hash256(input, hash);
	
	std::string hashStr = picosha2::bytes_to_hex_string(hash);
	for (int i = 0; i < PROT_HASH_LEN; i++)
		hash[i] = hashStr.at(i);

	return hash;
}

std::string Block::getSHA256Input(void) {
	return sha256Input;
}

BlockHeader & Block::getBlockHeader(void) {
	return blockHeader;
}

std::array<uint8_t, PROT_HASH_LEN> Block::getHash(void) {
	return currentHash;
}

Blockstat Block::getStats(void) {
	return blockStats;
}