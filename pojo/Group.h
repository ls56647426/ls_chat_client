#ifndef GROUP_H
#define GROUP_H

/* c lib */
#include <stdio.h>
#include <stdint.h>

/* c++ lib */
#include <string>
using namespace std;

#define GROUPNAME_MAX_LEN 32

/* 映射group表 */
class Group
{
public:
	Group();

	uint32_t getId() const;
	void setId(const uint32_t &value);

	string getName() const;
	void setName(const string &value);

	string toString() const;

private:
	uint32_t id;
	string name;
};

#endif // GROUP_H
