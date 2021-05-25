#ifndef EVENT_MANAGER
#define EVENT_MANAGER

#include "bunny.h"

// event type
enum class event_type
{
	born,
	dead,		// died of age
	killed,		// died of culling
	infected
};

// event type
struct event
{
	event_type m_eventType;
	bunny m_subject;

	// printer
	friend std::ostream& operator<<(std::ostream& os, const event& evt);
};

class EventManager
{
	// single instance
	static EventManager* instance;

	// list of events
	list<event> m_events = {};

	// private constructor
	EventManager();

public:	
	// singleton
	static EventManager* getInstance();

	// adds an event to the list of events
	void addEvent(event_type type, bunny subj);

	// returns quantity of an event type in the list of events
	size_t eventCnt(event_type evt) const;

	// prints the contents of the event list to a text file
	void printFile(size_t year) const;

	// deletes the current list
	void deleteList();

};

#endif