// Logger.cpp
#include "Logger.h"

Logger::Logger(std::ostream& consoleStream, std::ofstream& fileStream)
  : console(consoleStream), file(fileStream) {
  pthread_mutex_init(&logMutex, nullptr);
}

Logger::~Logger() {
  pthread_mutex_destroy(&logMutex);
}

void Logger::log(const std::string& message) {
  pthread_mutex_lock(&logMutex);
  console << message;
  file << message;
  pthread_mutex_unlock(&logMutex);
}