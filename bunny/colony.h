#ifndef COLONY_H
#define COLONY_H

#include "list.h"
#include "bunny.h"

class colony
{
	list<bunny> m_list = {};			// contains bunnies in the colony

public:
	// returns the size of the colony
	size_t size() const;

	// runs the colony simulator
	void run();

#pragma region Actions

	// kills half of the population
	void cull();

	// increments the age of each bunny and kills old ones 
	void age();

	// breeds the bunnies in the colony
	void breed();

	// infects members of a colony
	bool infect();

	// checks if all bunnies are the same gender
	void monogender(bool& gameOver);

	// recieves player instruction
	void input(bool& action);

#pragma endregion

};

#endif