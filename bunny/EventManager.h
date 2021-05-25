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
};

class EventManager
{
	// single instance
	static EventManager* instance;

	// list of events
	list<event> m_events = {};

	// private constructor
	EventManager() = default;

public:	
	// singleton
	static EventManager* getInstance();

	// adds an event to the list of events
	void addEvent(event);

	// returns quantity of an event type in the list of events
	size_t eventCnt(event_type evt) const;

	// deletes the current list
	void deleteList();
};

#endif