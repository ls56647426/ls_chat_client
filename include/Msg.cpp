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
	return "'type':" + to_string(type) +
		", 'group':{" + group.toString() +
		"}, 'src':{" + src.toString() +
		"}, 'dest':{" + dest.toString() +
		"}, 'info':{" + info.toString() + "}";
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
	return "'info':'" + info + "'";
}
