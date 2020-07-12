#include "User.h"

User::User()
{
	id = 0;
	username.clear();
	password.clear();
	pic.clear();
	nickname.clear();
	sign.clear();
	sex.clear();
	birthday.clear();
	location.clear();
	profession.clear();
	mobile.clear();
	email.clear();
	status = 0;
	date.clear();
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

string User::getPic() const
{
	return pic;
}

void User::setPic(const string &value)
{
	pic = value;
}

string User::getNickname() const
{
	return nickname;
}

void User::setNickname(const string &value)
{
	nickname = value;
}

string User::getSign() const
{
	return sign;
}

void User::setSign(const string &value)
{
	sign = value;
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

string User::getDate() const
{
    return date;
}

void User::setDate(const string &value)
{
    date = value;
}

string User::toString() const
{
    return "{\"id\":" + to_string(id) +
            ", \"username\":\"" + username +
			"\", \"password\":\"" + password +
			"\", \"pic\":\"" + pic +
			"\", \"nickname\":\"" + nickname +
			"\", \"sign\":\"" + sign +
			"\", \"sex\":\"" + sex +
			"\", \"birthday\":\"" + birthday +
			"\", \"location\":\"" + location +
			"\", \"profession\":\"" + profession +
			"\", \"mobile\":\"" + mobile +
			"\", \"email\":\"" + email +
			"\", \"status\":" + to_string(status) +
			", \"date\":\"" + date + "\"}";
}
