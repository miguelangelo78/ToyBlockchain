#include "blockchain.h"
#include "picosha2.h"
#include <cstdio>

Blockchain Blockchain::theChain;
std::vector<Block> Blockchain::chain;

Block Blockchain::genesis;
Block & Blockchain::head = genesis;

std::random_device Blockchain::randomDev;
std::mt19937_64 Blockchain::randomGen;

Blockchain::Blockchain()
{

}

Blockchain::~Blockchain()
{

}

Block Blockchain::createGenesis(void) {
	std::string genesisData = "{\"data\": \"nil\"}";
	std::array<uint8_t, PROT_HASH_LEN> prevNilHash;
	prevNilHash.fill('0');

	Block genesis(prevNilHash, std::vector<uint8_t>(genesisData.begin(), genesisData.end()));
	return genesis;
}

void Blockchain::createEmptyChain(void) {
	debug("(Blockchain): Creating new Blockchain . . .");
	randomGen.seed(randomDev());
	chain.clear();
	pushBlock(genesis = Blockchain::createGenesis());
}

void Blockchain::setDifficulty(float newDifficulty) {
	theChain.difficulty = newDifficulty;
}

float Blockchain::getDifficulty(void) {
	return theChain.difficulty;
}

bool Blockchain::pushBlock(Block & newBlock) {
	bool success = false;

	if (newBlock.getStats().isValid()) {
		chain.push_back(newBlock);
		head = chain[chain.size() - 1];
		success = true;
		
		BlockHeader blockHeader = newBlock.getBlockHeader();
		Blockstat stats = newBlock.getStats();
		auto currentHash = newBlock.getHash();

		char msg[2048];

		sprintf_s(msg, "(Blockchain): ADDED NEW BLOCK\
\n\t> Block size: %d bytes\
\n\t> Block version: %u\
\n\t> Previous Hash: %s\
\n\t> Hash Merkle Root: %s\
\n\t> Time: %u\
\n\t> Difficulty: %u\
\n\t> Nonce: %u\
\n\t> SHA256 Input: %s\
\n\t> Result/Current Hash: %s\
\n\t> Mine duration: %.7f seconds\
\n\t> # of hashes tried: %I64u\n",
			blockHeader.blocksize,
			blockHeader.version,
			std::string(blockHeader.hashPrevBlock.begin(), blockHeader.hashPrevBlock.end()).c_str(),
			std::string(blockHeader.hashMerkleRoot.begin(), blockHeader.hashMerkleRoot.end()).c_str(),
			blockHeader.time,
			blockHeader.bits,
			blockHeader.nonce,
			newBlock.getSHA256Input().c_str(),
			std::string(currentHash.begin(), currentHash.end()).c_str(),
			stats.getMineDuration(),
			stats.getHashesTriedCount()
		);
		
		debug(std::string(msg));
	}
	else {
		debug("(Blockchain | ERROR): Tried to push invalid block!");
	}

	return success;
}

void Blockchain::setVerbose(bool verbose) {
	theChain.verbose = verbose;
}

void Blockchain::debug(std::string msg) {
	if (theChain.verbose)
		printf("\nDEBUG> %s", msg.c_str());
}