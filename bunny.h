#ifndef BUNNY_H
#define BUNNY_H

#include <string>
#include <iostream>
#include "names.h"

enum class colour
{
	white,
	brown,
	black,
	spotted
};

std::ostream& operator<<(std::ostream& os, const colour& c);

class bunny
{
	static int m_cnt;
	static names m_namelist;

	int m_ID;
	bool m_sex;			// true if male
	colour m_colour;
	size_t m_age;
	std::string m_name;
	bool m_infect;

public:
	// generates a new bunny
	bunny();

	// creates a new bunny with mother features
	bunny(const colour col);

	// ages the bunny by one year
	void age();

	// query that returns true if a bunny dies
	bool deathCheck() const;

	// query that returns colour of a bunny
	colour getColour() const;

	// query that returns sex of a bunny
	bool getSex() const;

	// query that returns the age of a bunny
	size_t getAge() const;

	// query that returns if a bunny is infected
	bool isInfected() const;

	// infects the target bunny
	void infect();

#pragma region Conditional_Operators

	// query that returns if two items are different
	bool operator!=(const bunny& other) const;

	// checks if this bunny is equal to another
	bool operator==(const bunny& b) const;

	// checks if this bunny age is less than or equal to another
	bool operator<=(const bunny& b) const;

#pragma endregion

	// prints the data of the current object
	friend std::ostream& operator<<(std::ostream& os, const bunny& b);
};

#endif