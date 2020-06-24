#ifndef FRIEND_H
#define FRIEND_H

/* c lib */
#include <stdio.h>
#include <stdint.h>

/* c++ lib */
#include <string>
using namespace std;

class Friend
{
public:
	Friend();

	uint32_t getId() const;
	void setId(const uint32_t &value);

	uint32_t getUid1() const;
	void setUid1(const uint32_t &value);

	uint32_t getUid2() const;
	void setUid2(const uint32_t &value);

	string toString() const;

private:
	uint32_t id;
	uint32_t uid1;
	uint32_t uid2;
};

#endif // FRIEND_H
