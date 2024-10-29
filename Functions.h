#pragma once
#include <string>
#include "main.h"
#include <ctime>
//#include <chrono>
#include <sstream>

void rout(std::string); //выдача кирилицы в терминал
std::string choice(std::string, std::string); //выбор в терминале символа с проверкой
bool is_number(const std::string&); //проверка стринга на цифру
std::string cp1251_to_utf8(const char* str); //перекодировка 
std::string utf8_to_cp1251(const char* str); //перекодировка
std::string currentDateTime(); //текущее data-time
void writerLog(const std::string& name); //запись в лог
void readerLog(); //чтение последней записи лог
std::string tran (const char* str); //преобразование стринга из базы или введенного с клавиатуры


#ifdef _MYSQLSERVER
int createDb(); //создание базы данных
bool connectDb(); //установка соединения
void closeDb(); //закрытие соединения
#endif


