#ifndef MESSAGE_H
#define MESSAGE_H

/* c lib */
#include <stdio.h>
#include <stdint.h>

/* c++ lib */
#include <string>
using namespace std;

class Message
{
public:
	Message();

	uint32_t getId() const;
	void setId(const uint32_t &value);

	uint32_t getType() const;
	void setType(const uint32_t &value);

	uint32_t getUid() const;
	void setUid(const uint32_t &value);

	string getSrc() const;
	void setSrc(const string &value);

	string getContent() const;
	void setContent(const string &value);

	string getDate() const;
	void setDate(const string &value);

	bool getStatus() const;
	void setStatus(bool value);

	string toString() const;

private:
	uint32_t id;
	uint32_t type;
	uint32_t uid;
	string src;
	string content;
	string date;
	bool status;
};

#endif // MESSAGE_H
