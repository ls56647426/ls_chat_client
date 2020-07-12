#include "Group.h"

Group::Group()
{
	id = 0;
	num.clear();
	name.clear();
	description.clear();
	date.clear();
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

string Group::getNum() const
{
	return num;
}

void Group::setNum(const string &value)
{
	num = value;
}

string Group::getDescription() const
{
	return description;
}

void Group::setDescription(const string &value)
{
	description = value;
}

string Group::getDate() const
{
	return date;
}

void Group::setDate(const string &value)
{
	date = value;
}

string Group::toString() const
{
	return "{\"id\":" + to_string(id) +
			", \"num\":\"" + num +
			"\", \"name\":\"" + name +
			"\", \"description\":\"" + description +
			"\", \"date\":\"" + date + "\"}";
}
