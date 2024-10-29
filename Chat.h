#pragma once
#include "main.h" 
#include "User.h"
#include <iostream>


class Chat
{
public:
    Chat(); //конструктор
    std::string login(); //авторизация пользователя по логину-паролю
    void registr(); //регистрация нового пользователя
    void modifiUser(int IdUser); //корректировка пользователя
    void addUser(std::string login, unsigned int digest, std::string name, std::string email, bool isadmin, bool activ); //добавление пользователя
    void showUsers(); //отображение пользователей
    void showMsgs() const; //отображение сообщений для текущего пользоваеля
    void sendMsg(int userIdTo, std::string message); //запись сообщения для "пересылки"
    int findUserByLogin(std::string login); //поиск ID пользователя по логину
    int findUserIdByLoginPassword(std::string login, unsigned int digest); //поиск ID пользователя по логину-паролю
    User getCurrentUser() const; //получение текущего пользователя
    void setCurrentUser(int userId); //установка текущего пользователя
    User getUserById(int userId) const; //получение пользователя по Id
    void saveUser(User user); //запись 
private:
    User _currentUser; //ID текущего пользователя
#ifndef _MYSQLSERVER
    int _userIdMax; //максимальное (последнее) значение ID зарегистрированного пользователя
    int _msgIdMax; //максимальное (последнее) значение ID записанного сообщения
#endif
	
};

