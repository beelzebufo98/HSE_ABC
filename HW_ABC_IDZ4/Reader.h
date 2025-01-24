// Reader.h
#ifndef READER_H
#define READER_H

#include "Library.h"
#include "Logger.h"
#include <vector>

class Reader {
public:
  Reader(int id, Library* libraryPtr, Logger* loggerPtr);
  void run();

private:
  int id;                     // Reader's identifier
  Library* library;           // Pointer to the library
  Logger* logger;             // Pointer to the Logger object

  // Select from 1 to 3 random unique books
  std::vector<int> chooseBooks();

  // Simulate reading books
  void readBooks(const std::vector<int>& books);

  // Library visit to specifically obtain missing books
  void collectMissingBooks(const std::vector<int>& missingBooks);
};

#endif // READER_H
