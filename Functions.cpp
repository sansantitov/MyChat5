
#include "Functions.h"
#include "Logger.h"
#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstring>
#endif

#ifdef _MYSQLSERVER
#include "mysql.h"
constexpr auto DBHOST = "localhost";
constexpr auto DBNAME = "titov_chat5";
constexpr auto DBUSER = "root";
constexpr auto DBPASS = "root";
                                    
extern MYSQL mysql;


bool connectDb()
{
    //Подключаемся к серверу
    if (!mysql_real_connect(&mysql, DBHOST, DBUSER, DBPASS, DBNAME, 0, NULL, 0))
    {

        rout("Не смог подключиться к MySQL!\n");
        return false;
    };
    //Соединение успешно установлено
    //std::rout << "Success!" << std::endl;
    mysql_set_character_set(&mysql, "utf8");
    rout("Подключились к базе данных!\n");
    return true;
}


void closeDb()
{
    mysql_close(&mysql);
}


int createDb()
{
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql, DBHOST, DBUSER, DBPASS, "SYS", 0, NULL, 0))
    {
        rout("Не могу подключиться к MySQL базе SYS!\n");
        return 1;
    }
    mysql_set_character_set(&mysql, "utf8");
    std::string strF = "DROP DATABASE ";
    std::string str = strF + DBNAME;
    mysql_query(&mysql, str.c_str());
    strF = "CREATE DATABASE ";
    str = strF + DBNAME;
    mysql_query(&mysql, str.c_str());
    mysql_close(&mysql);

    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql, DBHOST, DBUSER, DBPASS, DBNAME, 0, NULL, 0))
    {
        rout("Не смог подключиться к базе!\n");
        return 1;
    };

    //создание таблиц
    mysql_query(&mysql, "CREATE TABLE users "
        "(id INT AUTO_INCREMENT PRIMARY KEY,"
        "login VARCHAR(64),"
        "digest INT UNSIGNED,"
        "name VARCHAR(255),"
        "email VARCHAR(255),"
        "isadmin BOOL,"
        "activ BOOL"
        ")"
    );
    if (mysql_errno(&mysql))
    {
        rout("Не смог создать таблицы в базе!\n");
        return 1;
    }

    mysql_query(&mysql, "CREATE TABLE msgs ("
        "id INT AUTO_INCREMENT PRIMARY KEY,"
        "datetime DATETIME,"
        "idfrom INT,"
        "idto INT,"
        "message VARCHAR(1024)"
        ")"
    );
    mysql_query(&mysql, "ALTER TABLE msg ADD FOREIGN KEY(idfrom) REFERENCES user(id)");
    mysql_query(&mysql, "ALTER TABLE msg ADD FOREIGN KEY(idto) REFERENCES user(id)");
    mysql_close(&mysql);
    rout("База создана!\n");
    return 0;
}
#endif


void rout(std::string text)
{
#ifdef _WIN32
    int cp = GetConsoleOutputCP();
    SetConsoleOutputCP(65001);
    std::cout << text;
    SetConsoleOutputCP(cp);
#else
    std::cout << text;
#endif


}



std::string choice(std::string str, std::string choices)
{
    const char *array = choices.c_str();
    std::string choice = " ";
    while (!(choice.length() == 1 && strchr(array, choice[0])))
    {
        rout(str);
        getline(std::cin, choice);
    }
    return choice;
}

bool is_number(const std::string& s) 
{ 
    return !s.empty() && (s.find_first_not_of("0123456789") == s.npos); 
}

std::string cp1251_to_utf8(const char* str)
{
    std::string res;
#ifdef _WIN32
    WCHAR* ures = NULL;
    char* cres = NULL;

    int result_u = MultiByteToWideChar(1251, 0, str, -1, 0, 0);
    if (result_u != 0)
    {
        ures = new WCHAR[result_u];
        if (MultiByteToWideChar(1251, 0, str, -1, ures, result_u))
        {
            int result_c = WideCharToMultiByte(CP_UTF8, 0, ures, -1, 0, 0, 0, 0);
            if (result_c != 0)
            {
                cres = new char[result_c];
                if (WideCharToMultiByte(CP_UTF8, 0, ures, -1, cres, result_c, 0, 0))
                {
                    res = cres;
                }
            }
        }
    }

    delete[] ures;
    delete[] cres;
#else
    res = str;
#endif
    return res;
}


std::string utf8_to_cp1251(const char* str) {
    std::string res;
#ifdef _WIN32
    WCHAR* ures = NULL;
    char* cres = NULL;

    int result_u = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
    if (result_u != 0)
    {
        ures = new WCHAR[result_u];
        if (MultiByteToWideChar(CP_UTF8, 0, str, -1, ures, result_u))
        {
            int result_c = WideCharToMultiByte(1251, 0, ures, -1, 0, 0, 0, 0);
            if (result_c != 0)
            {
                cres = new char[result_c];
                if (WideCharToMultiByte(1251, 0, ures, -1, cres, result_c, 0, 0))
                {
                    res = cres;
                }
            }
        }
    }

    delete[] ures;
    delete[] cres;
#else
    res = str;
#endif
    return res;
}


std::string tran (const char* str)
{
    std::string res;

#ifdef _MYSQLSERVER
    res = str;
#else
    res = cp1251_to_utf8(str);
#endif
    return res;
}



#pragma warning(disable : 4996)
std::string currentDateTime()
{
    //Формирование текущего времени
    std::ostringstream strstr;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    strstr << ltm->tm_mday << ".";
    strstr << 1 + ltm->tm_mon << ".";
    strstr << 1900 + ltm->tm_year << " ";
    strstr << std::setfill('0') << std::setw(2) << ltm->tm_hour << ":";
    strstr << std::setfill('0') << std::setw(2) << ltm->tm_min << ":";
    strstr << std::setfill('0') << std::setw(2) <<ltm->tm_sec;
    return strstr.str();
}


Logger logger;

void writerLog(const std::string& name)
{
    logger.writeStr(name);
}

void readerLog()
{
    rout(logger.getLastStr());
    std::cout << std::endl;
}

