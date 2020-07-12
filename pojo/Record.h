#ifndef RECORD_H
#define RECORD_H

/* c lib */
#include <stdio.h>
#include <stdint.h>

/* c++ lib */
#include <string>
using namespace std;

class Record
{
public:
	Record();

	uint32_t getId() const;
	void setId(const uint32_t &value);

	uint32_t getGid() const;
	void setGid(const uint32_t &value);

	uint32_t getSuid() const;
	void setSuid(const uint32_t &value);

	uint32_t getDuid() const;
	void setDuid(const uint32_t &value);

	string getContent() const;
	void setContent(const string &value);

	string getDate() const;
	void setDate(const string &value);

	bool getStatus() const;
	void setStatus(bool value);

	string toString() const;

private:
	uint32_t id;
	uint32_t gid;
	uint32_t suid;
	uint32_t duid;
	string content;
	string date;
	bool status;
};

#endif // RECORD_H
