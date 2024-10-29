#include "Logger.h"
#include "Functions.h"
#include <iostream>



Logger::Logger()
{
    fs.open(LOGGER_FILE, std::ios::in | std::ios::app);
    if (!fs.is_open())
    {
        rout("Не могу открыть log-файл\n");
    }
}

Logger::~Logger()
{
    fs.close();
    if (fs.is_open())
    {
        rout("Не могу закрыть log-файл\n");
    }
}

void Logger::writeStr(const std::string& str)
{
    shared_mutex.lock();
    last_str = "ЛОГ: " + currentDateTime() + " " + str;
    fs << last_str << std::endl;
    shared_mutex.unlock();
}

std::string Logger::getLastStr()
{
    shared_mutex.lock_shared();
    std::string lastStr = last_str;
    shared_mutex.unlock_shared();
    return lastStr;
}
