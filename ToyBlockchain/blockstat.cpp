#include "blockstat.h"
#include "blockchain.h"

Blockstat::Blockstat() {

}

Blockstat::~Blockstat() {

}

bool Blockstat::validate(void) {
	bool valid = false;
	
	valid = true; /* TODO */

	return valid;
}

bool Blockstat::isValid(void) {
	return valid;
}

enum Blockstat::State Blockstat::getState(void) {
	return state;
}

void Blockstat::setState(enum State newState) {
	state = newState;

	switch (newState) {
	case State::MINING:
		Blockchain::debug("Mining block . . .");
		mineClock = clock();
		break;
	case State::MINED:
		if (valid = validate()) {
			mineDuration = (double)(clock() - mineClock) / CLOCKS_PER_SEC;
			Blockchain::debug("Finished mining! (VALID)\n");
		}
		else {
			Blockchain::debug("Finished mining! (!INVALID!)\n");
		}
		break;
	}
}

double Blockstat::getMineDuration(void) {
	return mineDuration;
}

void Blockstat::incrementHashTrialCount(void) {
	hashesTried++;
}

uint64_t Blockstat::getHashesTriedCount(void) {
	return hashesTried;
}