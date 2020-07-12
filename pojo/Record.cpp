#include "Record.h"

Record::Record()
{
	id = gid = suid = duid = 0;
	content.clear();
	date.clear();
	status = 0;
}

uint32_t Record::getId() const
{
	return id;
}

void Record::setId(const uint32_t &value)
{
	id = value;
}

uint32_t Record::getGid() const
{
	return gid;
}

void Record::setGid(const uint32_t &value)
{
	gid = value;
}

uint32_t Record::getSuid() const
{
	return suid;
}

void Record::setSuid(const uint32_t &value)
{
	suid = value;
}

uint32_t Record::getDuid() const
{
	return duid;
}

void Record::setDuid(const uint32_t &value)
{
	duid = value;
}

string Record::getContent() const
{
	return content;
}

void Record::setContent(const string &value)
{
	content = value;
}

string Record::getDate() const
{
	return date;
}

void Record::setDate(const string &value)
{
	date = value;
}

bool Record::getStatus() const
{
    return status;
}

void Record::setStatus(bool value)
{
    status = value;
}

string Record::toString() const
{
	return "{\"id\":" + to_string(id) +
			", \"gid\":" + to_string(gid) +
			", \"suid\":" + to_string(suid) +
			", \"duid\":" + to_string(duid) +
			", \"content\":\"" + content +
			"\", \"date\":\"" + date +
			"\", \"status\":" + to_string(status) + "\"}";
}
