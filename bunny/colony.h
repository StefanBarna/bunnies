#ifndef COLONY_H
#define COLONY_H

#include "list.h"
#include "bunny.h"

class colony
{
	// event type
	enum class event_type
	{
		born,
		dead,		// died of age
		killed,		// died of culling
		infected
	};

	// keeps track of events
	struct eventTracker
	{
		list<event_type> m_eventlist = {};	// contains events in the colony

		// returns number of an event
		size_t eventCnt(event_type) const;
	};

	list<bunny> m_list = {};			// contains bunnies in the colony
	eventTracker m_events;				// keeps track of events in the colony
	bool m_run = true;					// manages the length of the game

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
	bool breed();

	// infects members of a colony
	bool infect();

	// checks if all bunnies are the same gender
	bool monogender();

	// recieves player instruction
	void input(bool& action);

	// printing
	void print(size_t year) const;

#pragma endregion

};

#endif