#include "Friend.h"

Friend::Friend()
{
	id = 0;
	uid1 = 0;
	uid2 = 0;
}

uint32_t Friend::getId() const
{
	return id;
}

void Friend::setId(const uint32_t &value)
{
	id = value;
}

uint32_t Friend::getUid1() const
{
	return uid1;
}

void Friend::setUid1(const uint32_t &value)
{
	uid1 = value;
}

uint32_t Friend::getUid2() const
{
	return uid2;
}

void Friend::setUid2(const uint32_t &value)
{
	uid2 = value;
}

string Friend::toString() const
{
	return "'id':'" + to_string(id) +
		"', 'uid1':'" + to_string(uid1) +
		"', 'uid2':'" + to_string(uid2) + "'";
}


