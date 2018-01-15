#pragma once

#include <cstdint>
#include <time.h>

class Blockstat {
public:
	enum State {
		INVAL,
		INIT,
		MINING,
		MINED
	};

private:
	enum State state = State::INVAL;
	uint64_t hashesTried = 0;
	double mineDuration = 0;
	clock_t mineClock;
	bool valid = false;

public:
	Blockstat();
	~Blockstat();

	bool validate(void);
	bool isValid(void);
	enum State getState(void);
	void setState(enum State newState);
	double getMineDuration(void);

	void incrementHashTrialCount(void);
	uint64_t getHashesTriedCount(void);
};