#include <random>
#include <iomanip>
#include "bunny.h"
using namespace std;

ostream& operator<<(ostream& os, const colour& c)
{
	switch (c)
	{
	case colour::black:
		os << "Black";
		break;
	case colour::brown:
		os << "Brown";
		break;
	case colour::white:
		os << "White";
		break;
	case colour::spotted:
		os << "Spotted";
		break;
	}
	return os;
}

int bunny::m_cnt = 0;
names bunny::m_namelist;

// setting up "random"
random_device device;
default_random_engine randEng(device());
uniform_int_distribution<size_t> sdist(0, 1);  // sex generator
uniform_int_distribution<size_t> cdist(0, 3);  // colour generator
uniform_int_distribution<size_t> ndist(0, 9);  // name generator
uniform_int_distribution<size_t> percentdist(0, 99); // percent generator


bunny::bunny()
{
	// number of bunnies (starting at 0)
	this->m_ID = this->m_cnt;

	// male [1] or female [0]
	this->m_sex = sdist(randEng);

	// selects one from the colour enum
	this->m_colour = static_cast<colour>(cdist(randEng));

	// sets age
	this->m_age = 0;

	// selects a name based on sex
	if (this->m_sex)
		this->m_name = this->m_namelist.boy(ndist(randEng));
	else
		this->m_name = this->m_namelist.girl(ndist(randEng));

	// 2% chance of being infected at birth
	if (percentdist(randEng) < 2)
		this->m_infect = true;
	else
		this->m_infect = false;

	// increment number of bunnies
	this->m_cnt++;
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

bool bunny::isInfected() const
{
	return this->m_infect;
}

void bunny::infect()
{
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
	cout << setw(4) << right << setfill('0') << b.m_ID << setfill(' ') << " | ";
	cout << setw(8) << left << b.m_name << " | ";
	if (b.m_sex)
		cout << "M";
	else
		cout << "F";
	cout << " | ";
	cout << setw(8) << b.m_colour << " | ";
	cout << setw(2) << setfill('0') << right << b.m_age << setfill(' ') << " | ";
	if (b.m_infect)
		cout << "Y";
	else
		cout << " ";
	return os;
}