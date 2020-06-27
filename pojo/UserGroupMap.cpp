#include "UserGroupMap.h"

UserGroupMap::UserGroupMap()
{
	id = uid = gid = status = permission = 0;
}

uint32_t UserGroupMap::getId() const
{
	return id;
}

void UserGroupMap::setId(const uint32_t &value)
{
	id = value;
}

uint32_t UserGroupMap::getUid() const
{
	return uid;
}

void UserGroupMap::setUid(const uint32_t &value)
{
	uid = value;
}

uint32_t UserGroupMap::getGid() const
{
	return gid;
}

void UserGroupMap::setGid(const uint32_t &value)
{
	gid = value;
}

uint32_t UserGroupMap::getStatus() const
{
	return status;
}

void UserGroupMap::setStatus(const uint32_t &value)
{
	status = value;
}

uint32_t UserGroupMap::getPermission() const
{
	return permission;
}

void UserGroupMap::setPermission(const uint32_t &value)
{
	permission = value;
}

string UserGroupMap::toString() const
{
	return "{\"id\":" + to_string(id) +
		", \"uid\":" + to_string(uid) +
		", \"gid\":" + to_string(gid) +
		", \"status\":" + to_string(status) +
		", \"permission\":" + to_string(permission) + "}";
}

