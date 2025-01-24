// Logger.h
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <pthread.h>
#include <string>

class Logger {
public:
  Logger(std::ostream& consoleStream, std::ofstream& fileStream);
  ~Logger();

  // Method for writing messages
  void log(const std::string& message);

private:
  std::ostream& console;        // Reference to the console output stream
  std::ofstream& file;          // Reference to the file stream
  pthread_mutex_t logMutex;     // Mutex for synchronizing output

  // Copying is prohibited
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
};

#endif // LOGGER_H
