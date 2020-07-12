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

	string getNum() const;
	void setNum(const string &value);

	string getDescription() const;
	void setDescription(const string &value);

	string getDate() const;
	void setDate(const string &value);

	string toString() const;

private:
	uint32_t id;
	string num;
	string name;
	string description;
	string date;
};

#endif // GROUP_H
