#ifndef USERGROUPMAP_H
#define USERGROUPMAP_H

/* c lib */
#include <stdio.h>
#include <stdint.h>

/* c++ lib */
#include <string>
using namespace std;

#include "User.h"
#include "Group.h"

/**
 * @brief The UserGroupMapStatus enum
 * GROUP_USER_STAT_NORMAL：正常
 * GROUP_USER_STAT_BANPOST：禁言
 */
enum UserGroupMapStatus
{
	GROUP_USER_STAT_NORMAL, GROUP_USER_STAT_BANPOST
};

/**
 * @brief The UserGroupMapPermission enum
 * PERM_USER：普通用户权限
 * PERM_ADMI：管理员权限
 */
enum UserGroupMapPermission
{
	PERM_USER, PERM_ADMI
};

class UserGroupMap
{
public:
	UserGroupMap();

	uint32_t getId() const;
	void setId(const uint32_t &value);

	uint32_t getUid() const;
	void setUid(const uint32_t &value);

	uint32_t getGid() const;
	void setGid(const uint32_t &value);

	uint32_t getStatus() const;
	void setStatus(const uint32_t &value);

	uint32_t getPermission() const;
	void setPermission(const uint32_t &value);

	string getDate() const;
	void setDate(const string &value);

	string toString() const;

private:
	uint32_t id;
	uint32_t uid;
	uint32_t gid;
	uint32_t status;
	uint32_t permission;
	string date;
};

#endif // USERGROUPMAP_H
