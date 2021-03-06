#include <stdio.h>
#include "blockchain.h"

Block & createTransaction(std::string data) {
	Block block(Blockchain::head.getHash(), std::vector<uint8_t>(data.begin(), data.end()));
	Blockchain::pushBlock(block);
	return Blockchain::head;
}

void testBlockchain(float difficulty, int tries, std::string transactionData) {
	Blockchain::theChain.setDifficulty(difficulty);

	double avgTime = 0.0;
	for (int i = 0; i < tries; i++) {
		Block & b = createTransaction(transactionData);
		avgTime += b.getStats().getMineDuration();
	}

	avgTime /= tries;

	printf("\n> Average mine time: %.4f\n", avgTime);
}

int main() {
	Blockchain::setVerbose(true);
	Blockchain::createEmptyChain();

	testBlockchain(3, 10, "{\"amount\": 10.00, \"from\": \"me\", \"to\": \"you\"}");

	printf("\n________________\nDONE");
	getchar();

    return 0;
}
