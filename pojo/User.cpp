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

string User::getHead_portrait() const
{
	return head_portrait;
}

void User::setHead_portrait(const string &value)
{
	head_portrait = value;
}

string User::getNickname() const
{
	return nickname;
}

void User::setNickname(const string &value)
{
	nickname = value;
}

string User::getSignature() const
{
	return signature;
}

void User::setSignature(const string &value)
{
	signature = value;
}

string User::getSex() const
{
	return sex;
}

void User::setSex(const string &value)
{
	sex = value;
}

string User::getBirthday() const
{
	return birthday;
}

void User::setBirthday(const string &value)
{
	birthday = value;
}

string User::getLocation() const
{
	return location;
}

void User::setLocation(const string &value)
{
	location = value;
}

string User::getProfession() const
{
	return profession;
}

void User::setProfession(const string &value)
{
	profession = value;
}

string User::getEmail() const
{
	return email;
}

void User::setEmail(const string &value)
{
	email = value;
}

string User::toString() const
{
	return "{\"id\":" + to_string(id) +
			", \"username\":\"" + username +
			"\", \"password\":\"" + password +
			"\", \"head_portrait\":\"" + head_portrait +
			"\", \"nickname\":\"" + nickname +
			"\", \"signature\":\"" + signature +
			"\", \"sex\":\"" + sex +
			"\", \"birthday\":\"" + birthday +
			"\", \"location\":\"" + location +
			"\", \"profession\":\"" + profession +
			"\", \"mobile\":\"" + mobile +
			"\", \"email\":\"" + email +
			"\", \"status\":" + to_string(status) + "}";
}
