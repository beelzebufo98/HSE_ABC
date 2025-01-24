#ifdef _WIN32
#include <windows.h> // For Windows
#elif __linux__
#include <unistd.h> // For Linux
#else
#error "Unsupported operating system"
#endif
// Reader.cpp
#include "Reader.h"
#include <algorithm>
//#include <windows.h>
//#include <unistd.h>
#include <cstdlib>
#include <ctime>

static void MySleep() {
#ifdef _WIN32
  Sleep((rand() % 3 + 1) * 1000);
  //std::cout << "Windows\n";
#elif __linux__
  usleep((rand() % 3 + 1) * 100000);
  //std::cout << "Linux\n";
#endif
}

Reader::Reader(int readerId, Library* libraryPtr, Logger* loggerPtr)
  : id(readerId), library(libraryPtr), logger(loggerPtr) {
  // Initialize random number generator with a different seed for each reader
  srand(time(nullptr) + id);
}

std::vector<int> Reader::chooseBooks() {
  int numBooks = rand() % 3 + 1; // Select between 1 to 3 books
  std::vector<int> chosenBooks;
  chosenBooks.reserve(numBooks);

  while ((int)chosenBooks.size() < numBooks) {
    int bookId = rand() % library->numBooks; // Books are numbered from 0 to N-1
    if (std::find(chosenBooks.begin(), chosenBooks.end(), bookId) == chosenBooks.end()) {
      chosenBooks.push_back(bookId);
    }
  }

  return chosenBooks;
}

void Reader::readBooks(const std::vector<int>& books) {
  // Log the start of reading
  std::string msg = "The reader " + std::to_string(id) + " started reading a book(-s): ";
  for (auto book : books) msg += std::to_string(book) + " ";
  msg += "\n";
  logger->log(msg);

  MySleep();
}

void Reader::collectMissingBooks(const std::vector<int>& missingBooks) {
  for (int bookId : missingBooks) {
    pthread_mutex_lock(&library->bookMutexes[bookId]);

    // Use a loop with a timeout
    struct timespec timeout;
    timeout.tv_sec = time(nullptr) + 5; // Wait for a maximum of 5 seconds
    timeout.tv_nsec = 0;

    while (!library->bookAvailable[bookId]) {
      int result = pthread_cond_timedwait(&library->bookConds[bookId],
        &library->bookMutexes[bookId],
        &timeout);

      if (result == ETIMEDOUT) {
        logger->log("The reader " + std::to_string(id) + " exceeded the book waiting time " + std::to_string(bookId) + ".\n");
        pthread_mutex_unlock(&library->bookMutexes[bookId]);
        return; // Exit if the book is unavailable for too long
      }
    }

    // Book is available, take it
    library->bookAvailable[bookId] = false;
    logger->log("The reader " + std::to_string(id) + " received a notification about the availability of the book " + std::to_string(bookId) + " and took it.\n");

    pthread_mutex_unlock(&library->bookMutexes[bookId]);

    // Read the book
    std::vector<int> singleBook = { bookId };
    readBooks(singleBook);

    // Return the book
    library->returnBooks(id, singleBook);
  }
}

void Reader::run() {
  // Number of library visits
  const int numVisits = 3;

  for (int visit = 0; visit < numVisits; ++visit) {
    std::vector<int> desiredBooks = chooseBooks();
    std::vector<int> takenBooks;
    std::vector<int> missingBooks;

    // Attempt to take books
    library->takeBooks(id, desiredBooks, takenBooks, missingBooks);

    if (!takenBooks.empty()) {
      // Read the taken books
      readBooks(takenBooks);

      // Return the books
      library->returnBooks(id, takenBooks);
    }

    if (!missingBooks.empty()) {
      // Wait some time before a special visit for missing books
      logger->log("The reader " + std::to_string(id) + " waits for some time before a special visit for missing books.\n");

      // Simulate waiting: 1 to 3 seconds
      MySleep();

      // Special visit for missing books
      collectMissingBooks(missingBooks);
    }

    // Simulate time between visits: 1 to 3 seconds
    MySleep();
  }

  logger->log("The reader " + std::to_string(id) + " completed his visits to the library.\n");
}
