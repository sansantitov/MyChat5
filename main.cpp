#include "main.h"
#include "Msg.h"
#include "User.h"
#include "Chat.h"
#include "Logger.h"
#include <iostream>
#include <memory>
#include <vector>
#include "Functions.h"
#include <string>
#ifdef _WIN32
#include <Windows.h>
#endif




#ifdef _MYSQLSERVER
#include "mysql.h"
MYSQL mysql;
MYSQL_RES* res;
MYSQL_ROW row;
MYSQL_FIELD* field;

int main(int argc, char* argv[])
#else
std::vector <User> users{}; //класс пользователей
std::vector<Msg> msgs{}; //класс сообщений
int main()
#endif
{

#ifdef _WIN32
    int cp1 = GetConsoleCP();
    int cp2 = GetConsoleOutputCP();
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("cls");
#endif



#ifdef _MYSQLSERVER
    mysql_init(&mysql);
    const std::string ss =  "1";
    if (argc == 2 && argv[1] == ss) 
        return createDb();
    connectDb();
#endif


    std::unique_ptr<Chat> chat = std::make_unique<Chat>();
    bool isExit = false;
    std::string strUserTo; //для выбора Id пользователя-адресата
    int intUserTo = 0; //Id пользователя - целое


    while (!isExit)
    {
        std::string strChoice;
        //strChoice = "0" - выход
        //strChoice = "1" - сообщение
        //strChoice = "2" - вход-авторизация(логин-пароль)
        //strChoice = "3" - регистрация нового пользователя
        //strChoice = "4" - корректировка пользователя

        strChoice = chat->login(); //авторизация
        while (1) //!isExit1)
        {
            if (strChoice == "0") //на выход
            {
                isExit = true;
                break;
            }
            else if (strChoice == "3")
            {
                chat->registr(); //регистрация
                break;
            }

            else if (strChoice == "4")
            {
                while (1)
                {
                    chat->showUsers(); //отображение пользователей для выбора Id адресата
                    rout("ID пользователя для корректировки?: ");
                    getline(std::cin, strUserTo); //ввод Id адресата
                    if (is_number(strUserTo))
                    {
                        intUserTo = stoi(strUserTo);
                        if (chat->getUserById(intUserTo).getActiv()) break;

                    }
                }
                chat->modifiUser(intUserTo);
                break;
            }

            else
            {
                break; //авторизация прошла
            }

        }

        std::string login = chat->getCurrentUser().getLogin();
        if (!isExit && login != "0")
        {
            while (true)
            {
                std::string message;
                chat->showMsgs(); //отображение всех сообщений для авторизованного пользователя
                std::string str = tran(login.c_str()) + ":послать сообщение-1, авторизация (новый вход)-2, закончить-0: ";
                std::string strChoice1 = choice(str, "120");

                if (strChoice1 == "0") { isExit = true; break; } //на выход
                else if (strChoice1 == "2") { break; } //на авторизацию

                if (strChoice1 == "1") //сообщение
                {
                    while (true)
                    {
                        chat->showUsers(); //отображение пользователей для выбора Id адресата - кому послать или 0-для всех
                        rout("Кому послать (ID адресата)? (послать всем-0): ");
                        getline(std::cin, strUserTo); //ввод Id адресата
                        if (is_number(strUserTo))
                        {
                            intUserTo = stoi(strUserTo);
                            if (chat->getUserById(intUserTo).getActiv()) break;
                        }
                    }
                    rout("сообщение: ");
                    getline(std::cin, message); //ввод сообщения
                    chat->sendMsg(intUserTo, message); //запись сообщения
                }
            }
        }
    }
    std::cout << std::endl;
#ifdef _WIN32
    SetConsoleCP(cp1);
    SetConsoleOutputCP(cp2);
#endif
#ifndef _MYSQLSERVER
    msgs.clear();
    users.clear();
#endif
    return 0;
}

