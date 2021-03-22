#ifndef NAMES_H
#define NAMES_H

#include <string>
#include "list.h"

class names
{
	list<std::string> m_names_boy;
	list<std::string> m_names_girl;
	int m_cnt;

public:
	// base constructor
	names();

	// returns girl name at given index
	std::string girl(size_t idx) const;

	// returns boy name at given idex
	std::string boy(size_t idx) const;

	// returns the amount of names;
	size_t size() const;
};

#endif