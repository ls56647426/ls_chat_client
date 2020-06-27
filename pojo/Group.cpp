#include "Group.h"

Group::Group()
{
	id = 0;
	name.clear();
}

uint32_t Group::getId() const
{
	return id;
}

void Group::setId(const uint32_t &value)
{
	id = value;
}

string Group::getName() const
{
	return name;
}

void Group::setName(const string &value)
{
	name = value;
}

string Group::toString() const
{
	return "{\"id\":" + to_string(id) +
		", \"name\":\"" + name + "\"}";
}


