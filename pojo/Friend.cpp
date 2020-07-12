#include "Friend.h"

Friend::Friend()
{
	id = suid = duid = 0;
	date.clear();
	permission = 0;
}

uint32_t Friend::getId() const
{
	return id;
}

void Friend::setId(const uint32_t &value)
{
	id = value;
}

uint32_t Friend::getSuid() const
{
	return suid;
}

void Friend::setSuid(const uint32_t &value)
{
	suid = value;
}

uint32_t Friend::getDuid() const
{
	return duid;
}

void Friend::setDuid(const uint32_t &value)
{
	duid = value;
}

string Friend::getDate() const
{
	return date;
}

void Friend::setDate(const string &value)
{
	date = value;
}

uint32_t Friend::getPermission() const
{
	return permission;
}

void Friend::setPermission(const uint32_t &value)
{
	permission = value;
}

string Friend::toString() const
{
	return "{\"id\":" + to_string(id) +
			", \"suid\":" + to_string(suid) +
			", \"duid\":" + to_string(duid) +
			", \"date\":\"" + date +
			"\", \"permission\":" + to_string(permission) + "}";
}


