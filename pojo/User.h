#ifndef USER_H
#define USER_H

/* c lib */
#include <stdio.h>
#include <stdint.h>

/* c++ lib */
#include <string>
using namespace std;

/**
 * @brief The UserStatus enum
 * USER_STAT_ONLINE：在线
 * USER_STAT_OFFLINE：离线
 * USER_STAT_HIDE：隐身
 */
enum UserStatus
{
	USER_STAT_ONLINE, USER_STAT_OFFLINE, USER_STAT_HIDE
};

/* 映射user表 */
class User
{
public:
	User();

	uint32_t getId() const;
	void setId(const uint32_t &value);

	string getUsername() const;
	void setUsername(const string &value);

	string getPassword() const;
	void setPassword(const string &value);

	string getMobile() const;
	void setMobile(const string &value);

	uint32_t getStatus() const;
	void setStatus(const uint32_t &value);

	string toString() const;

private:
	/* 字段映射 */
	uint32_t id;
	string username;
	string password;
	string mobile;
	uint32_t status;
};

#endif // USER_H
