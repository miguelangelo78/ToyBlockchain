#pragma once

#include "block.h"
#include "protocol.h"
#include <random>

class Blockchain
{
private:
	bool verbose = false;
	float difficulty = PROT_INITIAL_DIFFICULTY;
	
	static Block createGenesis(void);

public:
	static Block genesis;
	static Block & head;
	static std::vector<Block> chain;
	
	static std::random_device randomDev;
	static std::mt19937_64 randomGen;
	
	Blockchain();
	~Blockchain();

	static void debug(std::string msg);

	static void createEmptyChain(void);
	static bool pushBlock(Block & newBlock);

	static void setDifficulty(float newDifficulty);
	static float getDifficulty(void);

	static void setVerbose(bool verbose);
	
	static Blockchain theChain;
};