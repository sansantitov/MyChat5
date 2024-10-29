#pragma once
#include <string>

class User
{
public:
    User(); //конструктор
    User(int id, std::string login, unsigned int digest, std::string name, std::string email, bool isadmin, bool activ); //конструктор с установкой свойств
    int getId() const; //получение ID пользователя
    void setId(int); //установка ID пользователя
    std::string getLogin() const; //получение логина пользователя
    void setLogin(std::string); //установка логина пользователя
    unsigned int getDigest() const; //получение хеш-пароля пользователя
    void setDigest(unsigned int); //установка хеш-пароля пользователя
    std::string getName() const; //получение имени пользователя
    void setName(std::string); //установка имени пользователя
    std::string getEmail() const; //получение email
    void setEmail(std::string); //установка email
    bool getActiv() const; //получение признака активности
    void setActiv(bool); //установка признака активности
    bool getIsAdmin() const; //получение признака адинистратора
    void setIsAdmin(bool); //установка признака адинистратора
private:
    int _id; //ID пользователя
    std::string _login; //логин
    unsigned int _digest; //хеш-пароль
    std::string _name; //имя
    std::string _email; //почта
    bool _isadmin; //признак администратора
    bool _activ; //признак активности
};

