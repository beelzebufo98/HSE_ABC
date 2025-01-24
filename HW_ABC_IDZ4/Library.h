// Library.h
#ifndef LIBRARY_H
#define LIBRARY_H

#include <pthread.h>
#include <vector>
#include <string>
#include "Logger.h"

class Library {
public:
  // Constructor and destructor
  Library(int numBooks, Logger* loggerPtr);  // Important: Constructor declaration
  ~Library();  // Important: Destructor declaration

  // Other method declarations remain unchanged
  void takeBooks(int readerId, const std::vector<int>& desiredBooks, std::vector<int>& takenBooks, std::vector<int>& missingBooks);
  void returnBooks(int readerId, const std::vector<int>& books);

  int numBooks;
  std::vector<bool> bookAvailable;
  std::vector<pthread_mutex_t> bookMutexes;
  std::vector<pthread_cond_t> bookConds;

private:
  Logger* logger;

  // Copying is prohibited
  Library(const Library&) = delete;
  Library& operator=(const Library&) = delete;
};

#endif // LIBRARY_H
