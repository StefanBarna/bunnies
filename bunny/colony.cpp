#include <Windows.h>
#include <iomanip>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include <sstream>
#include "colony.h"
#include "list.h"

// TODO: for next week create a new list [milestone 1]

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

void mergeSort(list<bunny>& li, bool(*comp)(list<bunny>::iterator, list<bunny>::iterator) = [](list<bunny>::iterator iA, list<bunny>::iterator iB) { return *iA <= *iB; })
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

		mergeSort(colA, comp);
		mergeSort(colB, comp);

		list<bunny>::iterator itA = colA.begin();
		list<bunny>::iterator itB = colB.begin();

		for (list<bunny>::iterator it = li.begin(); it != li.end(); ++it)
		{
			if (itA != colA.end() && itB != colB.end())
			{
				if (comp(itA, itB))
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
		auto it = this->m_list.begin();
		size_t startSize = this->size();
		for (size_t i = 0; i < startSize; ++i, ++it)
		{
			if (!(*it).getSex() && !(*it).isInfected() && (*it).getAge() >= 2)
			{
				bunny b((*it).getColour());
				bool uniqueCoord = true;
				do
				{
					uniqueCoord = true;
					for (auto gridIt = this->m_list.begin(); gridIt != this->m_list.end(); ++gridIt)
					{
						if (b.location() == gridIt->location())
							uniqueCoord = false;
					}
					if (!uniqueCoord)
						b.location() = coord();
				} while (!uniqueCoord);
				this->m_list.push_back(b);
			}
		}
	}
}

bool colony::infect()
{
	bool infection = false;
	bool infectable = false;
	size_t infected = 0;

	// is there an infection
	for (auto it = this->m_list.begin(); it != this->m_list.end(); ++it)
	{
		if ((*it).isInfected())
			infection = true;
		else if (!(*it).isInfected())
			infectable = true;

		// breaks if we have the required data
		if (infection && infectable)
			break;
	}

	// if there is an infection
	if (infection && infectable)
	{
		uniform_int_distribution<size_t> dist(0ull, this->size() - 1ull);

		for (auto it = this->m_list.begin(); it != this->m_list.end(); ++it)
		{
			// find number infected
			if ((*it).isInfected())
				++infected;
		}

		// manages number of infectable bunnis
		size_t clean = this->m_list.size() - infected;

		// infect the clean bunnies number infected times
		for (size_t i = 0; i < infected; i++)
		{
			if (clean > 0)
			{
				size_t loc = dist(engine);
				while (this->m_list.at(loc).isInfected())
					loc = dist(engine);
				this->m_list.at(loc).infect();
				--clean;
			}
			else
			{
				return false;
				break;
			}
		}
	}

	return true;
}

void colony::monogender(bool& gameOver)
{
	bool genderType = this->m_list[0].getSex();
	bool genderCheck = false;
	for (size_t i = 1; i < this->m_list.size(); i++)
	{
		if (this->m_list[i].getSex() != genderType)
			genderCheck = true;
	}
	if (!genderCheck)
		gameOver = true;
}

void colony::input(bool& action)
{
	INPUT_RECORD inRecord[100]{};
	DWORD r = 0;
	HANDLE consoleHandle = GetStdHandle(STD_INPUT_HANDLE);

	// check if some console event happened; leave that event in the queue
	if (PeekConsoleInput(consoleHandle, inRecord, 1, &r) && r > 0)
	{
		// extract the event (remove it from the queue)
		do {
			if (ReadConsoleInput(consoleHandle, inRecord, 100, &r))
			{
				for (size_t i = 0; i < r; i++)
				{
					// check if the event was related to keyboard
					if (inRecord[i].EventType == KEY_EVENT)
					{
						// the user pressed a key, find which key
						if (inRecord[i].Event.KeyEvent.bKeyDown && inRecord[i].Event.KeyEvent.wVirtualKeyCode == 'K')
							this->cull();
						else if (inRecord[i].Event.KeyEvent.bKeyDown &&
							((inRecord[i].Event.KeyEvent.dwControlKeyState & RIGHT_CTRL_PRESSED) != 0 || (inRecord[i].Event.KeyEvent.dwControlKeyState & LEFT_CTRL_PRESSED) != 0) &&
							inRecord[i].Event.KeyEvent.wVirtualKeyCode == 'L')
							cout << "it works";
						// TODO: buffer not cleared once per turn. Either do all the events at once or remove all other events after the first one

						action = true;
					}

				}
			}
		} while (r == 100);
	}
	else
		action = false;
}

// 0000 0001
// 0000 0100    <-  LEFT_CTRL
// 0000 1000    <-  RIGHT_CTRL
// 0100 0000
// 0001 0000

// 0000 0100 &        <- the value returned by OS
// 0000 1000          <- RIGHT_CTRL
// 0000 1000          <- != 0

// 0000 1000 | 0000 0100


/*
* 
* 1 AND 1   is 1
* 0 AND 1   is 0
* 1 AND 0   is 0
* 0 AND 0   is 0

0 OR 0   is 0
1 OR 0   is 1
0 OR 1   is 1
1 OR 1   is 1

NOT 0     is 1
NOT 1     is 0

0 XOR 0    is 0
0 XOR 1    is 1
1 XOR 0    is 1
1 XOR 1    is 0

| A | B | A & B | A or B | ~A | A ^ B |
|---|---|-------|--------|----|-------|
| 0 | 0 |   0   |   0    | 1  |   0   |
| 0 | 1 |   0   |   1    | 1  |   1   |
| 1 | 0 |   0   |   1    | 0  |   1   |
| 1 | 1 |   1   |   1    | 0  |   0   |

0000    AND
1111
----
0000



*/

void colony::run()
{
	// sets the console to UTF8
	SetConsoleOutputCP(CP_UTF8);

	// random
	random_device device;
	default_random_engine rand(device());

	// creating the first bunnies
	for (size_t i = 0; i < 5; i++)
		this->m_list.push_front(bunny());

	// manages the length of the game
	bool gameOver = false;

	// preliminary game check (all female / all male)
	this->monogender(gameOver);

	// manages user's actions
	bool action = false;

	// "game" loop
	while (this->size() != 0)
	{
		// continue conditions
		this->monogender(gameOver);

		// user input
		this->input(action);

		if (!action & !gameOver)
		{
			// ageing
			this->age();

			// infection
			gameOver = !this->infect();

			// breeding
			this->breed();

			// culling
			if (this->size() > 1000)
				this->cull();

			// sorting
			mergeSort(this->m_list, [](list<bunny>::iterator iA, list<bunny>::iterator iB) {return iA->location() <= iB->location(); });
		}

		// console clear
		system("CLS");

		// printing
		stringstream ss;
		auto it = this->m_list.begin();
		
		ss << u8"┌";
		for (size_t i = 0; i < SIDE_LENGTH; ++i)
			ss << u8"─";
		ss << u8"┐" << endl;
		for (size_t i = 0; i < SIDE_LENGTH; ++i)
		{
			ss << u8"│";
			for (size_t j = 0; j < SIDE_LENGTH; ++j)
			{
				if (it != this->m_list.end() && it->location().m_y == i + 1 && it->location().m_x == j + 1)
				{
					ss << *it;
					++it;
				}
				else
					ss << u8"\33[38;2;51;51;51m·\33[0m";
			}
			ss << u8"│" << endl;
		}
		ss << u8"└";
		for (size_t i = 0; i < SIDE_LENGTH; ++i)
			ss << u8"─";
		ss << u8"┘" << endl;
		if (gameOver)
			ss << "[GAME OVER]" << endl;

		cout << ss.str();

		// waiting for 1 second
		this_thread::sleep_for(1s);

		// ends program
		if (gameOver)
			return;
	}
}