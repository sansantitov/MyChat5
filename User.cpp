#include "User.h"
#include "main.h"
#include <iostream>

User::User()
{
    _id = 0;
    _login = "";
    _digest = 0;
    _name = "";
    _email = "";
    _isadmin = false;
    _activ = false;

}

User::User(int id, std::string login, unsigned int digest, std::string name, std::string email, bool isadmin, bool activ)
{
	
	_id = id;
	_login = login;
    _digest = digest;
	_name = name;
    _email = email;
    _isadmin = isadmin;
    _activ = activ;
}

int User::getId() const
{
    return _id;
}

 void User::setId(int id)
{
     _id = id;
}

std::string User::getLogin() const
{
    return _login;
}

void User::setLogin(std::string login)
{
    _login = login;
}

unsigned int User::getDigest() const
{
    return _digest;
}

void User::setDigest(unsigned int digest)
{
    _digest = digest;
}

std::string User::getName() const
{
    return _name;
}

void User::setName(std::string name)
{
    _name = name;
}

std::string User::getEmail() const
{
    return _email;
}

void User::setEmail(std::string email)
{
    _email = email;
}

bool User::getActiv() const
{
    return _activ;
}

void User::setActiv(bool activ)
{
    _activ = activ;
}

bool User::getIsAdmin() const
{
    return _isadmin;
}

void User::setIsAdmin(bool isadmin)
{
    _isadmin = isadmin;
}
