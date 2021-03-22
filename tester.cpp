/*#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <sstream>

int main()
{
	bool keepRunning = true;
	int val = 0;

	while (keepRunning)
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
					if (inRecord.Event.KeyEvent.bKeyDown != 0 && inRecord.Event.KeyEvent.wVirtualKeyCode == 'Q')
						keepRunning = false;
				}
			}
		}

		val++;
		std::stringstream ss;
		//ss << "\b\b\b\b\b\b\b\b\b\b" << std::setw(10) << val;
		// https://en.wikipedia.org/wiki/ANSI_escape_code
		ss << "\33[2K\33[1G" << "\33[38;2;255;175;0m" << std::setw(10) << val << "\33[0m";
		std::cout << ss.str();
	}
}*/

#include "colony.h"
using namespace std;

int main()
{
	colony c;
	c.run();
}