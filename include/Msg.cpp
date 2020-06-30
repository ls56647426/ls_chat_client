#include "Msg.h"

Msg::Msg()
{
	type = 0;
}

uint32_t Msg::getType() const
{
	return type;
}

void Msg::setType(const uint32_t &value)
{
	type = value;
}

Group Msg::getGroup() const
{
	return group;
}

void Msg::setGroup(const Group &value)
{
	group = value;
}

User Msg::getSrc() const
{
	return src;
}

void Msg::setSrc(const User &value)
{
	src = value;
}

User Msg::getDest() const
{
	return dest;
}

void Msg::setDest(const User &value)
{
	dest = value;
}

MsgInfo Msg::getInfo() const
{
	return info;
}

void Msg::setInfo(const MsgInfo &value)
{
	info = value;
}

string Msg::toString() const
{
	return "{\"type\":" + to_string(type) +
		", \"group\":" + group.toString() +
		", \"src\":" + src.toString() +
		", \"dest\":" + dest.toString() +
		", \"info\":" + info.toString() + "}";
}

MsgInfo::MsgInfo()
{
	info.clear();
}

string MsgInfo::getInfo() const
{
	return info;
}

void MsgInfo::setInfo(const string &value)
{
	info = value;
}

string MsgInfo::toString() const
{
	string s = "{\"info\":\"" + info +
			"\", \"userListInfo\":[";

	for(auto it = userListInfo.begin(); it != userListInfo.end(); it++)
	{
		if(it != userListInfo.begin())
			s += ", ";
		s += "{\"user\":" + it->toString() + "}";
	}

	s += "]}";

	return s;
}

list<User> MsgInfo::getUserListInfo() const
{
	return userListInfo;
}

void MsgInfo::setUserListInfo(const list<User> &value)
{
	userListInfo = value;
}
