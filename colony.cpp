#include <Windows.h>
#include <iomanip>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include "colony.h"
#include "list.h"

// List available in C++ standard
//#include <forward_list>
//#include <list>

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

static random_device device;
static default_random_engine engine(device());

size_t colony::size() const
{
	return this->m_list.size();
}

void colony::cull()
{
	size_t victimCnt = this->size() / 2;
	for (size_t i = 0; i < victimCnt; i++)
	{
		uniform_int_distribution<size_t> dist(0, this->size() - 1);
		size_t victimIDX = dist(engine);
		auto it = this->m_list.begin();
		it = it + victimIDX;
		this->m_list.erase(it);
	}
}

void mergeSort(list<bunny>& li)
{
	if (li.size() > 1)
	{
		size_t lenA = li.size() / 2;
		size_t lenB = li.size() - lenA;

		list<bunny> colA;
		for (auto it = li.begin(); it != li.begin() + lenA; ++it)
			colA.push_back(*it);

		list<bunny> colB;
		for (auto it = li.begin() + lenA; it != li.end(); ++it)
			colB.push_back(*it);

		mergeSort(colA);
		mergeSort(colB);

		list<bunny>::iterator itA = colA.begin();
		list<bunny>::iterator itB = colB.begin();

		for (list<bunny>::iterator it = li.begin(); it != li.end(); ++it)
		{
			if (itA != colA.end() && itB != colB.end())
			{
				if (*itA <= *itB)
				{
					*it = *itA;
					++itA;
				}
				else
				{
					*it = *itB;
					++itB;
				}
			}
			else if (itA != colA.end())
			{
				*it = *itA;
				++itA;
			}
			else
			{
				*it = *itB;
				++itB;
			}
		}
	}
}

size_t colony::bunnyofcolor(colour col, bool sex)
{
	size_t cnt = 0;
	for (auto it = this->m_list.begin(); it != this->m_list.end(); ++it)
	{
		if ((*it).getSex() == sex && (*it).getColour() == col)
			cnt++;
	}
	return cnt;
}

void colony::age()
{
	list<bunny> markedForDeath;
	for (auto it = this->m_list.begin(); it != this->m_list.end(); ++it)
	{
		(*it).age();
		if ((*it).deathCheck())
			markedForDeath.push_front(*it);
	}
	for (auto it = markedForDeath.begin(); it != markedForDeath.end(); ++it)
	{
		m_list.remove(*it);
	}
}

void colony::breed()
{
	bool breed = false;
	size_t prevSize = this->size();
	for (auto it = this->m_list.begin(); it != this->m_list.end(); ++it)
	{
		if ((*it).getSex() && !(*it).isInfected() && (*it).getAge() >= 2)
		{
			breed = true;
			break;
		}
	}
	if (breed)
	{
		// creates a list of babies
		list<bunny> babies;
		for (auto it = this->m_list.begin(); it != this->m_list.end(); ++it)
		{
			if (!(*it).getSex() && !(*it).isInfected() && (*it).getAge() >= 2)
				babies.push_front(bunny((*it).getColour()));
		}
		// adds the babies to the colony
		for (auto it = babies.begin(); it != babies.end(); ++it)
			this->m_list.push_back(*it);
	}
}

void colony::infect()
{
	bool infection = false;
	bool infectable = false;
	size_t infected = 0;
	// is there an infection?
	for (auto it = this->m_list.begin(); it != this->m_list.end(); ++it)
	{
		if ((*it).isInfected())
		{
			infection = true;
			break;
		}
		else if (!(*it).isInfected())
		{
			infectable = true;
			break;
		}
	}
	if (infection && infectable)
	{
		uniform_int_distribution<size_t> dist(0ull, this->size() - 1ull);

		for (auto it = this->m_list.begin(); it != this->m_list.end(); ++it)
		{
			// find number infected
			if ((*it).isInfected())
				++infected;
		}
		// infect the clean bunnies number infected times
		for (size_t i = 0; i < infected; i++)
		{
			size_t loc = dist(engine);
			while (this->m_list.at(loc).isInfected())
				loc = dist(engine);
			this->m_list.at(loc).infect();
		}
	}
}

void colony::userKill()
{
	INPUT_RECORD inRecord{};
	DWORD r = 0;
	HANDLE consoleHandle = GetStdHandle(STD_INPUT_HANDLE);

	// check if some console event happened; leave that event in the queue
	if (PeekConsoleInput(consoleHandle, &inRecord, 1, &r) && r > 0)
	{
		// extract the event (remove it from the queue)
		if (ReadConsoleInput(consoleHandle, &inRecord, 1, &r))
		{
			// check if the event was related to keywboard
			if (inRecord.EventType == KEY_EVENT)
			{
				// the user pressed a key, find which key
				if (inRecord.Event.KeyEvent.bKeyDown != 0 && inRecord.Event.KeyEvent.wVirtualKeyCode == 'K')
					this->cull();
			}
		}
	}
}

void colony::run()
{
	random_device device;
	default_random_engine rand(device());

	// creating the first bunnies
	for (size_t i = 0; i < 5; i++)
		this->m_list.push_front(bunny());

	size_t it = 1;

	// "game" loop
	while (this->size() != 0)
	{
		{
			cout << "\n========================== Year " << setw(3) << it << " ==========================" << endl
				<< " | " << setw(4) << this->bunnyofcolor(colour::white, true) << " | " << setw(4) << this->bunnyofcolor(colour::white, false)
				<< " | " << setw(4) << this->bunnyofcolor(colour::black, true) << " | " << setw(4) << this->bunnyofcolor(colour::black, false)
				<< " | " << setw(4) << this->bunnyofcolor(colour::brown, true) << " | " << setw(4) << this->bunnyofcolor(colour::brown, false)
				<< " | " << setw(4) << this->bunnyofcolor(colour::spotted, true) << " | " << setw(4) << this->bunnyofcolor(colour::spotted, false)
				<< " | " << this->size() << endl
				<< "==============================================================" << endl << endl;
		}

		// ageing
		this->age();

		// breeding
		this->breed();

		// infection
		this->infect();
		
		// culling
		if (this->size() > 1000)
		{
			this->cull();
		}

		// sorting
		mergeSort(this->m_list);

		// printing
		//for (const bunny& item : this->m_list)
		//	cout << item << endl;
		std::for_each(m_list.begin(), m_list.end(), [](const bunny& item) { cout << item << endl; }); // for evey elent in range, do something (lambda)
		auto cntInfected = std::count_if(m_list.begin(), m_list.end(), [](const bunny& item) { return item.isInfected(); });

		if (cntInfected > 0)
		{
			cout << "Infected: " << cntInfected << endl;
			bunny* ptrI = new bunny[cntInfected];
			std::copy_if(m_list.begin(), m_list.end(), ptrI, [](const bunny& item) {return item.isInfected(); });
			std::for_each(ptrI, ptrI + cntInfected, [](const bunny& item) { cout << item << endl; });

			if (cntInfected > 3)
				std::transform(m_list.begin(), m_list.end(), m_list.begin(), [](bunny& item) { if (item.isInfected()) { item.age(); item.age(); } return item; });
		}

		// waiting for 1 second
		this_thread::sleep_for(1s);

		// user killing
		this->userKill();

		it++; 
	}
}

// TODO: EVERY TIME YOU MODIFY SOMETHING ADD IT TO GITHUB
// TODO: create the tester project and test everything (all classes, all functions)
//          when you design tests, don't look at the implementation of the tested function/class; look what the specs (specifications, what the function should achieve).
// TODO: [X] add console interractivity.