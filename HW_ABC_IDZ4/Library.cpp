// Library.cpp
#include "Library.h"
#include <algorithm>

// Constructor with full initialization
Library::Library(int numBooks, Logger* loggerPtr)
  : numBooks(numBooks), bookAvailable(numBooks, true), logger(loggerPtr) {
  // Initialization of mutexes and condition variables
  bookMutexes.resize(numBooks);
  bookConds.resize(numBooks);

  for (int i = 0; i < numBooks; ++i) {
    // Initialize each mutex and condition variable
    if (pthread_mutex_init(&bookMutexes[i], nullptr) != 0) {
      // Handle mutex initialization error
      logger->log("Error initializing mutex for book " + std::to_string(i) + "\n");
    }

    if (pthread_cond_init(&bookConds[i], nullptr) != 0) {
      // Handle condition variable initialization error
      logger->log("Error initializing condition variable for book " + std::to_string(i) + "\n");
    }
  }
}

// Destructor for releasing resources
Library::~Library() {
  // Destroy mutexes and condition variables
  for (int i = 0; i < numBooks; ++i) {
    pthread_mutex_destroy(&bookMutexes[i]);
    pthread_cond_destroy(&bookConds[i]);
  }
}

// The remaining class methods remain unchanged
void Library::takeBooks(int readerId, const std::vector<int>& desiredBooks, std::vector<int>& takenBooks, std::vector<int>& missingBooks) {
  // Existing implementation
  std::vector<int> sortedBooks = desiredBooks;
  std::sort(sortedBooks.begin(), sortedBooks.end());

  for (int bookId : sortedBooks) {
    pthread_mutex_lock(&bookMutexes[bookId]);

    if (bookAvailable[bookId]) {
      bookAvailable[bookId] = false;
      takenBooks.push_back(bookId);
      logger->log("The reader " + std::to_string(readerId) + " took the book " + std::to_string(bookId) + ".\n");
    }
    else {
      missingBooks.push_back(bookId);
      logger->log("The reader " + std::to_string(readerId) + " couldn't take the book " + std::to_string(bookId) + " (busy).\n");
    }

    pthread_mutex_unlock(&bookMutexes[bookId]);
  }
}

void Library::returnBooks(int readerId, const std::vector<int>& books) {
  // Existing implementation
  std::vector<int> sortedBooks = books;
  std::sort(sortedBooks.begin(), sortedBooks.end());

  for (int bookId : sortedBooks) {
    pthread_mutex_lock(&bookMutexes[bookId]);

    bookAvailable[bookId] = true;
    pthread_cond_broadcast(&bookConds[bookId]);

    logger->log("The reader " + std::to_string(readerId) + " returned the book " + std::to_string(bookId) + ".\n");

    pthread_mutex_unlock(&bookMutexes[bookId]);
  }
}
