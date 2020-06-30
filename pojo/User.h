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

	string getHead_portrait() const;
	void setHead_portrait(const string &value);

	string getNickname() const;
	void setNickname(const string &value);

	string getSignature() const;
	void setSignature(const string &value);

	string getSex() const;
	void setSex(const string &value);

	string getBirthday() const;
	void setBirthday(const string &value);

	string getLocation() const;
	void setLocation(const string &value);

	string getProfession() const;
	void setProfession(const string &value);

	string getEmail() const;
	void setEmail(const string &value);

private:
	/* 字段映射 */
	uint32_t id;
	string username;
	string password;
	string head_portrait;
	string nickname;
	string signature;
	string sex;
	string birthday;
	string location;
	string profession;
	string mobile;
	string email;
	uint32_t status;
};

#endif // USER_H
