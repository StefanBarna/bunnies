#include <algorithm>
#include "EventManager.h"

EventManager* EventManager::instance = nullptr;

EventManager* EventManager::getInstance()
{
	if (instance == nullptr)
		instance = new EventManager;
	return instance;
}

void EventManager::addEvent(event evt)
{
	this->m_events.push_back(evt);
}

size_t EventManager::eventCnt(event_type evt) const
{
	return std::count_if(EventManager::getInstance()->m_events.begin(), EventManager::getInstance()->m_events.end(), [&evt](event item) { if (item.m_eventType == evt) return true; else return false; });
}

void EventManager::deleteList()
{
	this->m_events.~list();
}