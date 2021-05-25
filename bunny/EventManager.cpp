#include <algorithm>
#include <fstream>
#include <iomanip>
#include "EventManager.h"

using namespace std;

EventManager* EventManager::instance = nullptr;

EventManager::EventManager()
{
	ofstream file("info.txt", ios::trunc);
	file.close();
}

EventManager* EventManager::getInstance()
{
	if (instance == nullptr)
		instance = new EventManager;
	return instance;
}

void EventManager::addEvent(event_type type, bunny subj)
{
	// create the event
	event evt;
	evt.m_eventType = type;
	evt.m_subject = subj;

	// push the event
	this->m_events.push_back(evt);
}

size_t EventManager::eventCnt(event_type evt) const
{
	return count_if(EventManager::getInstance()->m_events.begin(), EventManager::getInstance()->m_events.end(), [&evt](event item) { if (item.m_eventType == evt) return true; else return false; });
}

void EventManager::printFile(size_t year) const
{
	ofstream file("info.txt", std::ios::app);
	if (file.is_open())
	{
		file << "===============" << endl;
		file << "      " << setw(3) << setfill('0') << right << year << "      " << endl;
		file << "===============" << endl;
		file << "    SUMMARY    " << endl;
		file << "===============" << endl;

		// summary
		file << setw(3) << setfill('0') << right << EventManager::getInstance()->eventCnt(event_type::born) << "[BORN]" << endl;
		file << setw(3) << setfill('0') << right << EventManager::getInstance()->eventCnt(event_type::dead) << "[DIED]" << endl;
		file << setw(3) << setfill('0') << right << EventManager::getInstance()->eventCnt(event_type::killed) << "[KILLED]" << endl;
		file << setw(3) << setfill('0') << right << EventManager::getInstance()->eventCnt(event_type::infected) << "[INFECTED]" << endl << endl;

		file << "===============" << endl;
		file << "    DETAILS    " << endl;
		file << "===============" << endl;

		// detailed bunnies
		for (auto it = this->m_events.begin(); it != this->m_events.end(); ++it)
			file << *it << endl;

		file.close();
	}
}

void EventManager::deleteList()
{
	this->m_events.~list();
}

ostream& operator<<(ostream& os, const event& evt)
{
	os << "[" << setw(4) << right << setfill('0') << evt.m_subject.getID() << "]";
	os << "[" << setw(8) << left << setfill(' ') << evt.m_subject.getName() << "]";
	os << "[" << setw(7) << left << setfill(' ');
	if (evt.m_subject.getColour() == colour::black)
		os << "black";
	else if (evt.m_subject.getColour() == colour::brown)
		os << "brown";
	else if (evt.m_subject.getColour() == colour::white)
		os << "white";
	else if (evt.m_subject.getColour() == colour::spotted)
		os << "spotted";
	os << "]";
	os << "[";
	if (evt.m_subject.getSex())
		os << "M";
	else
		os << "F";
	os << "]";

	if (evt.m_eventType == event_type::born)
		os << "[BORN]";
	else if (evt.m_eventType == event_type::dead)
		os << "[DIED]";
	else if (evt.m_eventType == event_type::killed)
		os << "[KILLED]";
	else if (evt.m_eventType == event_type::infected)
		os << "[INFECTED]";

	return os;
}