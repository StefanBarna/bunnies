#ifndef BUNNY_H
#define BUNNY_H

#include <string>
#include <iostream>
#include "names.h"

static const size_t SIDE_LENGTH = 50;
static const size_t MAX_RESIST = 50;

// bunny colours [ENUM] 
enum class colour
{
	white,
	brown,
	black,
	spotted
};

// grid coordinates [STRUCTURE]
struct coord
{
	int m_x;
	int m_y;

	// base constructor
	coord();

	// compares x and y coordinate values
	bool operator<=(const coord& c) const;

	// query returning whether the coordinates between two bunnies are the same
	bool operator==(const coord& c) const;
};

class bunny
{
	static int m_cnt;
	static names m_namelist;

	int m_ID;					// unique value for each bunny
	bool m_sex;					// true if male
	colour m_colour;			// bunny colour
	size_t m_age;				// bunny age
	std::string m_name;			// bunny name
	bool m_infect;				// infection identifier
	size_t m_resistance;		// chance to ignore infection
	coord m_loc;				// bunny location

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

	// query that returns the ID of a bunny
	size_t getID() const;

	// query that returns the x-value of a bunny
	int getX() const;
	
	// query that returns the y-value of a bunny
	int getY() const;

	// query that returns the name of a bunny
	std::string getName() const;

	// returns the address of the coordinate member of a bunny
	coord& location();

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

	// formatted printing of current object for grid
	friend std::ostream& operator<<(std::ostream& os, const bunny& b);
};

#endif