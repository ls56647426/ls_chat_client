#include "User.h"

User::User()
{
	id = 0;
	username.clear();
	password.clear();
	mobile.clear();
	status = 0;
}

uint32_t User::getId() const
{
	return id;
}

void User::setId(const uint32_t &value)
{
	id = value;
}

string User::getUsername() const
{
	return username;
}

void User::setUsername(const string &value)
{
	username = value;
}

string User::getPassword() const
{
	return password;
}

void User::setPassword(const string &value)
{
	password = value;
}

string User::getMobile() const
{
	return mobile;
}

void User::setMobile(const string &value)
{
	mobile = value;
}

uint32_t User::getStatus() const
{
	return status;
}

void User::setStatus(const uint32_t &value)
{
	status = value;
}

string User::toString() const
{
	return "{\"id\":" + to_string(id) +
		", \"username\":\"" + username +
		"\", \"password\":\"" + password +
		"\", \"mobile\":\"" + mobile +
		"\", \"status\":" + to_string(status) + "}";
}

