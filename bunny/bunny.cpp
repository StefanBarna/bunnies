#include <random>
#include <iomanip>
#include "bunny.h"
using namespace std;

int bunny::m_cnt = 0;
names bunny::m_namelist;

// setting up "random"
random_device device;
default_random_engine randEng(device());
uniform_int_distribution<size_t> sdist(0, 1);			// sex generator
uniform_int_distribution<size_t> cdist(0, 3);			// colour generator
uniform_int_distribution<size_t> ndist(0, 9);			// name generator
uniform_int_distribution<size_t> percentdist(0, 99);	// percent generator
uniform_int_distribution<int> ldist(1, SIDE_LENGTH);	// location / coordinate generator
uniform_int_distribution<size_t> rdist(0, MAX_RESIST);

coord::coord()
{
	this->m_x = ldist(randEng);
	this->m_y = ldist(randEng);
}

bool coord::operator<=(const coord& c) const
{
	if (this->m_y < c.m_y)
		return true;
	else if (this->m_y == c.m_y)
	{
		if (this->m_x <= c.m_x)
			return true;
	}
	return false;
}

bool coord::operator==(const coord& c) const
{
	if (this->m_y == c.m_y)
	{
		if (this->m_x == c.m_x)
			return true;
	}
	return false;
}

bunny::bunny()
{
	this->m_ID = this->m_cnt;
	this->m_sex = sdist(randEng);
	this->m_colour = static_cast<colour>(cdist(randEng));
	this->m_age = 0;
	if (this->m_sex)
		this->m_name = this->m_namelist.boy(ndist(randEng));
	else
		this->m_name = this->m_namelist.girl(ndist(randEng));
	if (percentdist(randEng) < 2)
		this->m_infect = true;
	else
		this->m_infect = false;
	this->m_resistance = rdist(randEng);
	this->m_cnt++;
	this->m_loc = coord();
}

bunny::bunny(const colour col) : bunny()
{
	if (percentdist(randEng) < 1)
		this->m_colour = static_cast<colour>(cdist(randEng)); // 1% chance of a colour mutation
	else
		this->m_colour = col;
}

void bunny::age()
{
	this->m_age++;
}

bool bunny::deathCheck() const
{
	if (!this->m_infect)
	{
		if (this->m_age > 10)
			return true;
	}
	else
	{
		if (this->m_age > 50)
			return true;
	}
	return false;
}

colour bunny::getColour() const
{
	return this->m_colour;
}

bool bunny::getSex() const
{
	return this->m_sex;
}

size_t bunny::getAge() const
{
	return this->m_age;
}

size_t bunny::getID() const
{
	return this->m_ID;
}

int bunny::getX() const
{
	return this->m_loc.m_x;
}

int bunny::getY() const
{
	return this->m_loc.m_y;
}

string bunny::getName() const
{
	return this->m_name;
}

coord& bunny::location()
{
	return this->m_loc;
}

bool bunny::isInfected() const
{
	return this->m_infect;
}

void bunny::infect()
{
	if (percentdist(randEng) > this->m_resistance)
		this->m_infect = true;
}

bool bunny::operator!=(const bunny& other) const
{
	return (this->m_ID != other.m_ID);
}

bool bunny::operator==(const bunny& b) const
{
	return (this->m_ID == b.m_ID);
}

bool bunny::operator<=(const bunny& b) const
{
	return (this->m_age <= b.m_age);
}

ostream& operator<<(ostream& os, const bunny& b)
{
	// colour
	if (b.m_infect)
		os << "\33[38;2;245;66;75m";
	else if (b.m_colour == colour::black)
		os << "\33[38;2;151;196;181m";
	else if (b.m_colour == colour::brown)
		os << "\33[38;2;235;178;99m";
	else if (b.m_colour == colour::white)
		os << "\33[38;2;255;255;255m";
	else if (b.m_colour == colour::spotted)
		os << "\33[38;2;255;253;181m";

	// sex
	if (b.m_sex)
		os << "M\33[0m";
	else if (!b.m_sex)
		os << "F\33[0m";

	return os;
}