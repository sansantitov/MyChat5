#pragma once
#include <iostream>

class Msg
{
public:
    Msg(int id, int idFrom, int idTo, std::string message); //конструктор с установкой значений
    std::string getTime() const; //выдача текущего времени
    int getIdFrom() const; //получение ID адресата сообщения
    int getIdTo() const; //получение ID пользователя-адресата сообщения
    std::string getMessage() const; //получения текста сообщения
private:
    int _id; //ID сообщения
    std::string _datetime; //время формирования сообщения
    int _idFrom; //ID пользователя-источника сообщения (от кого)
    int _idTo; //ID пользователя-адресата сообщения (кому)
    std::string _message; //текст сообщения
};

