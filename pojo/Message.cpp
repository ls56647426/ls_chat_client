#include "Message.h"

Message::Message()
{
	id = type = uid = 0;
	src.clear();
	content.clear();
	date.clear();
	status = 0;
}

uint32_t Message::getId() const
{
	return id;
}

void Message::setId(const uint32_t &value)
{
	id = value;
}

uint32_t Message::getType() const
{
	return type;
}

void Message::setType(const uint32_t &value)
{
	type = value;
}

uint32_t Message::getUid() const
{
	return uid;
}

void Message::setUid(const uint32_t &value)
{
	uid = value;
}

string Message::getSrc() const
{
	return src;
}

void Message::setSrc(const string &value)
{
	src = value;
}

string Message::getContent() const
{
	return content;
}

void Message::setContent(const string &value)
{
	content = value;
}

string Message::getDate() const
{
	return date;
}

void Message::setDate(const string &value)
{
	date = value;
}

bool Message::getStatus() const
{
    return status;
}

void Message::setStatus(bool value)
{
    status = value;
}

string Message::toString() const
{
	return "{\"id\":" + to_string(id) +
			", \"type\":" + to_string(type) +
			", \"uid\":" + to_string(uid) +
			", \"src\":\"" + src +
			"\", \"content\":\"" + content +
			"\", \"date\":\"" + date +
			"\", \"status\":" + to_string(status) + "\"}";
}
