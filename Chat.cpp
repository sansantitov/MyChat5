#include "Chat.h"
#include "main.h"
#include <vector>
#include <string>
#include <memory>
#include "User.h"
#include "Msg.h"
#include "Functions.h"
#include "sha1.h"

#ifdef _MYSQLSERVER
#include "mysql.h"
#include <iostream>
#include <sstream>
extern MYSQL mysql;
extern MYSQL_RES* res;
extern MYSQL_ROW row;
extern MYSQL_FIELD* field;
#else 
extern std::vector<User> users;
extern std::vector<Msg> msgs;
#endif

 Chat::Chat()
 {

#ifdef _MYSQLSERVER

     mysql_query(&mysql, "SELECT id FROM users WHERE login='0'");
     if (res = mysql_store_result(&mysql))
     {
         if (!(row = mysql_fetch_row(res)))
         {
             const char* tmp = "INSERT INTO users (login, digest, name, email, isadmin, activ) "
                 "VALUES ('0', 0, '***', '', 0, 1)";
             mysql_query(&mysql, tmp);
         }
         
     }
#else
     users.emplace_back(1, "0", 0, "SYSTEM", "", false, true); //0-резервируем системного пользователя "0" (для выдачи всем)
     _userIdMax = 1;
     _msgIdMax =  0;
#endif

     _currentUser = getUserById(0); //установим текущего user как общего (с логином "0" и без пароля)
 }


 std::string Chat::login()
 {
     std::string rr;
     User user = getCurrentUser();
     bool isUser = user.getDigest() != 0;
     std::string nameCurrent = isUser ? user.getLogin() : "не установлен";
     rout(" ***Текущий пользователь : " + nameCurrent + " ***\n");
     if (user.getIsAdmin()) rout("для авторизации введите логин или \"3\"-для регистрации нового пользователя, \"4\"-для корректировки\n");
     else rout("для авторизации введите логин или \"3\"-для регистрации нового пользователя\n");
     while (true)
     {
         std::string loginstr, password;
         int id;
         while (true)
         {

             rout("логин: ");
             getline(std::cin, loginstr);
             if (loginstr == "3")
             {
                 rr = "3";
                 break;
             }
             if (loginstr == "4")
             {
                 rr = "4";
                 break;
             }
             else if (loginstr.length() < 2) rout ("в логине должно быть не менее двух символов!\n");
             else break;
         }

         if (rr != "3" && rr != "4")
         {
             rout("пароль: ");
             getline(std::cin, password);

             const char* p = password.c_str();
             uint* ddd = sha1(p, (uint)password.length());
             id = findUserIdByLoginPassword(loginstr, *ddd);
             if (id < 0)
             {
                 rout("логин-пароль не соответствуют ни одному пользователю!\n");
                 rr = choice("повторить-1, регистрация нового-3, выход-0: ", "130");
                 if (rr != "1") break;
             }
             else
             {
                 setCurrentUser(id);
                 writerLog("авторизовался " + cp1251_to_utf8(loginstr.c_str()));
                 readerLog();
                 break;
             }
         }
         else break;
     }
     return rr;
 }


 void Chat::registr()
 {
     std::string login, password, name, email, stradmin = "0";

     rout("*** Регистрация нового пользователя ***\n");

     while (true)
     {

         rout("логин нового пользователя: ");
         getline(std::cin, login);
         if (login.length() < 2) rout ("в логине должно быть не менее двух символов!\n");
         else if (findUserByLogin(login) >= 0) rout ("пользователь с таким логином уже зарегистрирован!\n");
         else break;
     }
     while (true)
     {
         rout("пароль: ");
         getline(std::cin, password);
         if (password.length() < 1) rout("пароль не должен быть пустым!\n");
         else break;
     }
     rout("имя: ");
     getline(std::cin, name);
     rout("email: ");
     getline(std::cin, email);
     while (1)
     {
         rout("админ-1/пользователь-0: ");
         getline(std::cin, stradmin);
         if (stradmin == "0" || stradmin == "1") break;
     }


     const char* p = password.c_str();
     uint* ddd = sha1(p, (uint)password.length());
     addUser(login, *ddd, name, email, stradmin=="1", true);
     rout("******** регистрация закончена ********\n");
     writerLog("зарегистрировался " + cp1251_to_utf8(login.c_str()));
     readerLog();

 }


 void Chat::modifiUser(int idUser)
 {
     User u = getUserById(idUser);
     if (u.getLogin() == "0") return; //если 
     //корректирую текущий?
     bool isCurr = getCurrentUser().getId() == getUserById(idUser).getId();
     std::string name = u.getName();
     std::string email = u.getEmail();
     std::string stradmin = u.getIsAdmin() ? "1" : "0";
     std::string stractiv = u.getActiv() ? "1" : "0";
     std::string cc = "*** Корректировка пользователя " + tran(u.getLogin().c_str()) + " " + tran(u.getName().c_str()) + " ***\n";
     rout(cc);
     rout("имя ("+tran(name.c_str())+"): ");
     getline(std::cin, name);
     rout("email (" + tran(email.c_str()) + "): ");
     getline(std::cin, email);
     
     while (1)
     {
         rout("админ-1/польз-0 (" + stradmin + "): ");
         getline(std::cin, stradmin);
         if (stradmin == "0" || stradmin == "1") break;
     }

     while (1)
     {
         rout("активный-1/удаленный-0: ");
         getline(std::cin, stractiv);
         if (stractiv == "0" || stractiv == "1") break;
     }

     u.setName(name);
     u.setEmail(email);
     u.setIsAdmin(stradmin == "1");
     u.setActiv(stractiv == "1");
     saveUser(u);
     writerLog("откорректирован " + tran(u.getLogin().c_str()));
     readerLog();
     rout("******** корректировка закончена ********\n");

     //если удаляет сам себя, то сбросить его из текущего
     if (stractiv == "0" && isCurr) setCurrentUser(0);
 }


 void Chat::addUser(std::string login, unsigned int digest, std::string name, std::string email, bool isadmin, bool activ)
 {
 #ifdef _MYSQLSERVER
     std::ostringstream strstr;
     std::string str;
     int isad = isadmin ? 1 : 0;
     int acti = activ ? 1 : 0;
     strstr << "INSERT INTO users (login, digest, name, email, isadmin, activ) VALUES ("
         << "'" << cp1251_to_utf8(login.c_str()) << "'" << "," << digest << "," << "'" << cp1251_to_utf8(name.c_str()) << "'" << "," <<
         "'" << cp1251_to_utf8(email.c_str()) << "'" << "," << isad << "," << acti << ")";
     str = strstr.str();
     mysql_query(&mysql, str.c_str());
 #else
      std::unique_ptr<User> u = std::make_unique<User>(++_userIdMax, login, digest, name, email, isadmin, activ);
      users.push_back(*u);
 #endif
 }


 void Chat::showUsers()
 {
     rout("--- ID  Имя пользователя -----------\n");

 #ifdef _MYSQLSERVER
     mysql_query(&mysql, "SELECT id, name FROM users WHERE activ = 1 AND login != '0'");
     if (res = mysql_store_result(&mysql))
     {
         while (row = mysql_fetch_row(res))
         {
             int cp = GetConsoleOutputCP();
             SetConsoleOutputCP(65001);
             std::cout << "     " << row[0] << "  " << row[1] << std::endl;
             SetConsoleOutputCP(cp);
             
         }
     }
 #else
     if (users.size() > 1)
     {
         for (unsigned int i = 1; i < users.size(); ++i) if (users[i].getActiv())
         {
             std::cout << "     " << users[i].getId() << "  " << users[i].getName() << std::endl;
         }
     }

     
#endif

     std::cout << "------------------------------------\n";
     writerLog("отображение пользователей"); readerLog();
 }

 void Chat::showMsgs() const
 {
     User user = getCurrentUser();
     
 #ifdef _MYSQLSERVER

     #ifdef _WIN32
     int cp = GetConsoleOutputCP();
     SetConsoleOutputCP(65001);
     #endif
     
     if (user.getLogin() != "0")
     {
         std::cout << "******** " << user.getName() << ": ";
         rout("принятые сообщения *******\n");
         std::ostringstream strstr;
         std::string str;
         strstr << "SELECT datetime, users.name, message FROM msgs "
             "JOIN users ON idfrom = users.id WHERE idto = 0 OR idto = " << user.getId();
         str = strstr.str();
         mysql_query(&mysql, str.c_str());
         if (res = mysql_store_result(&mysql))
         {
             while (row = mysql_fetch_row(res))
             {
                 std::cout << row[0] << " " << row[1] << ":" << row[2] << std::endl;
             }
         }
         std::string s(37 + user.getName().length(), '*');
         std::cout << s << std::endl;
     }
     #ifdef _WIN32
     SetConsoleOutputCP(cp);
     #endif
 #else
     if (user.getLogin() != "0")
     {
         std::cout << "******** " << user.getName() << ": ";
         rout("принятые сообщения *******\n");

         for (unsigned int i = 0; i < msgs.size(); ++i)
         {
             if (msgs[i].getIdTo() == 0 || msgs[i].getIdTo() == user.getId())
             {
                 std::cout << msgs[i].getTime() << " " << getUserById(msgs[i].getIdFrom()).getName() << ":" << msgs[i].getMessage() << std::endl;
             }

         }

         std::string s(37 + user.getName().length(), '*');
         std::cout << s << std::endl;
     }
#endif
     writerLog("отображение всех сообщений для пользователя " + tran(user.getLogin().c_str()));
     readerLog();
 }

 void Chat::sendMsg(int userIdTo, std::string message)
 {
     User user = getCurrentUser();
 #ifdef _MYSQLSERVER
     std::ostringstream strstr;
     std::string str;
     strstr << "INSERT INTO msgs (datetime, idfrom, idto, message) VALUES ("
         << "NOW()," << user.getId() << "," << userIdTo << "," << "'" << cp1251_to_utf8(message.c_str()) << "')";
     str = strstr.str();
     mysql_query(&mysql, str.c_str());
 #else
     std::unique_ptr<Msg> m = std::make_unique<Msg>(++_msgIdMax, user.getId(), userIdTo, message);
     msgs.push_back(*m);
 #endif

     writerLog("посылка сообщения для пользователя Id = " + std::to_string(userIdTo));
     readerLog();

 }


 int Chat::findUserByLogin(std::string login)
 {
     int id = -1;
#ifdef _MYSQLSERVER
     std::ostringstream strstr;
     std::string str;


     strstr << "SELECT id, login FROM users WHERE activ = 1 AND login = '" << cp1251_to_utf8(login.c_str()) << "'";
     str = strstr.str();
     mysql_query(&mysql, str.c_str());

     if (res = mysql_store_result(&mysql))
     {
         if (row = mysql_fetch_row(res))
         {
             id = std::stoi(row[0]);
         }
     }
     else
     {
         strstr << "\nОшибка MySql: " << mysql_error(&mysql);
         str = strstr.str();
         rout(str);
     }


#else

     for (auto u = users.begin(); u != users.end(); ++u)
     {
         if (u->getLogin() == login && u->getActiv())
         {

             id = u->getId();
             break;
         }

     }
#endif

     return id;
 }




int Chat::findUserIdByLoginPassword(std::string login, unsigned int digest)
{
    int id = -1;

#ifdef _MYSQLSERVER
    std::ostringstream strstr;
    std::string str;


    strstr << "SELECT id,login FROM users WHERE activ = 1 AND login = '" << cp1251_to_utf8(login.c_str()) << "' AND digest = " << digest;
    str = strstr.str();
    mysql_query(&mysql, str.c_str());

    if (res = mysql_store_result(&mysql))
    {
        if (row = mysql_fetch_row(res))
        {
            id = std::stoi(row[0]);
        }
    }
    else
    {
        strstr << "\nОшибка MySql: " << mysql_error(&mysql);
        str = strstr.str();
        rout(str);
    }

#else

    for (auto u = users.begin(); u != users.end(); ++u)
    {
        if (u->getLogin() == login && u->getDigest() == digest && u->getActiv())
        {
            id = u->getId();
            break;
        }

    }
    
#endif
    return id;
}



User Chat::getCurrentUser() const
{
    return _currentUser;
}


void Chat::setCurrentUser(int userId)
{
    _currentUser = getUserById(userId);
}

User Chat::getUserById(int userId) const
{
    User user;
#ifdef _MYSQLSERVER
    std::ostringstream strstr;
    std::string str;
    if (userId == 0) mysql_query(&mysql, "SELECT id,login,digest,name,email,isadmin,activ FROM users WHERE login = '0'");
    else
    {
        strstr << "SELECT id,login,digest,name,email,isadmin,activ FROM users WHERE id = " << userId;
        str = strstr.str();
        mysql_query(&mysql, str.c_str());
    }
    if (res = mysql_store_result(&mysql)) 
    {
        if (row = mysql_fetch_row(res)) 
        {
            user.setId(std::stoi(row[0])); //id
            user.setLogin(row[1]); //login
            user.setDigest(std::stoul(row[2])); //digest
            user.setName(row[3]); //name
            user.setEmail(row[4]); //email
            user.setIsAdmin(std::stoi(row[5])); //isadmin
            user.setActiv(std::stoi(row[6])); //activ
        }
    }
    else
    {
        strstr << "\nОшибка MySql: " << mysql_error(&mysql);
        str = strstr.str();
        rout(str);
    }

    return user;
#else

    for (auto u = users.begin(); u != users.end(); ++u)
    {
        if ((userId==0 && u->getLogin() == "0") || (u->getActiv() && u->getId() == userId))
        {
          user = *u;
          break;
        }
    }
    return user; //если не нашел, то возвращаем пользователя "0"
#endif
}

void Chat::saveUser(User user)
{
#ifdef _MYSQLSERVER
    std::ostringstream strstr;
    std::string str;
    strstr << "UPDATE users SET name = '" << cp1251_to_utf8(user.getName().c_str()) << "', email = '" << cp1251_to_utf8(user.getEmail().c_str()) << "', "
            "isadmin = " << user.getIsAdmin() << ", activ = " << user.getActiv() << " WHERE id= " << user.getId();
    str = strstr.str();
    mysql_query(&mysql, str.c_str());
#else
    int i = -1;
    for (auto u = users.begin(); u != users.end(); ++u)
    {
        i++;
        if (u->getActiv() && u->getId() == user.getId())
        {
            User uu = *u;
            users[i] = user;
            break;
        }
    }
#endif
}
