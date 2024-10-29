#pragma once
#include <thread>
#include <shared_mutex>
#include <fstream>
#define LOGGER_FILE "log.txt"

class Logger
{
public:

  Logger();
  ~Logger();
  void writeStr(const std::string& str);
  std::string getLastStr();
private:
  std::fstream fs;
  std::string last_str = "";
  std::shared_mutex shared_mutex;
};
