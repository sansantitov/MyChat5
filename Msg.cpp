#include "Msg.h"
#include "main.h"
#include "Functions.h"
#include <iostream>




Msg::Msg(int id, int idFrom, int idTo, std::string message)
{
    _id = id;
    _datetime = currentDateTime();
	_message = message;
	_idFrom = idFrom;
	_idTo = idTo;
}


std::string Msg::getTime()const
{
    return _datetime;
}

int Msg::getIdFrom() const
{
    return _idFrom;
}

int Msg::getIdTo() const
{
    return _idTo;
}

std::string Msg::getMessage() const
{
    return _message;
}


