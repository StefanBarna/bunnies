#include "names.h"

names::names()
{
	// set boy names
	this->m_names_boy.push_front("Charlie");
	this->m_names_boy.push_front("Oscar");
	this->m_names_boy.push_front("Theo");
	this->m_names_boy.push_front("Benjamin");
	this->m_names_boy.push_front("Peter");
	this->m_names_boy.push_front("Chase");
	this->m_names_boy.push_front("Chester");
	this->m_names_boy.push_front("Oliver");
	this->m_names_boy.push_front("Archie");
	this->m_names_boy.push_front("Niko");

	// set girl names
	this->m_names_girl.push_front("Coco");
	this->m_names_girl.push_front("Gloria");
	this->m_names_girl.push_front("Buttons");
	this->m_names_girl.push_front("Petunia");
	this->m_names_girl.push_front("Bianca");
	this->m_names_girl.push_front("Clover");
	this->m_names_girl.push_front("Tink");
	this->m_names_girl.push_front("Ginger");
	this->m_names_girl.push_front("Callie");
	this->m_names_girl.push_front("Nutmeg");

	// set size to the amount of names
	this->m_cnt = 10;
}

std::string names::girl(size_t idx) const
{
	return this->m_names_girl.at(idx);
}

std::string names::boy(size_t idx) const
{
	return this->m_names_boy.at(idx);
}

size_t names::size() const
{
	return this->m_cnt;
}