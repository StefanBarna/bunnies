#ifndef COLONY_H
#define COLONY_H

#include "list.h"
//#include <list>
#include "bunny.h"

class colony
{
	list<bunny> m_list = {};

	// returns the amount of bunnies of a certain colour and gender
	size_t bunnyofcolor(colour col, bool sex);

public:
	// returns the size of the colony
	size_t size() const;

	// kills half of the population
	void cull();

	// increments the age of each bunny and kills old ones 
	void age();

	// breeds the bunnies in the colony
	void breed();

	// infects members of a colony
	void infect();

	// recieves player instruction for a cull
	void userKill();

	// runs the colony simulator
	void run();
};

#endif