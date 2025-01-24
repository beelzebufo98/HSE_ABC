// ConsoleApp_ABC.cpp
#include "Library.h"
#include "Reader.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
#include <limits>

// Structure for transferring data to a stream
struct ThreadData {
  Reader* reader;
};
// Function performed by each thread
void* threadFunction(void* arg) {
  ThreadData* data = static_cast<ThreadData*>(arg);
  data->reader->run();
  return nullptr;
}

// Function to read the configuration file
bool readConfigFile(const std::string& fileName, int& N, int& M, std::string& outputFile) {
  std::ifstream file(fileName);
  if (!file.is_open()) {
    std::cerr << "Unable to open configuration file: " << fileName << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    // Skip empty lines and comments
    if (line.empty() || line[0] == '#') continue;

    size_t delimiterPos = line.find('=');
    if (delimiterPos == std::string::npos) continue;

    std::string key = line.substr(0, delimiterPos);
    std::string value = line.substr(delimiterPos + 1);

    // Remove whitespace
    key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
    value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());

    if (key == "N") {
      N = std::stoi(value);
    }
    else if (key == "M") {
      M = std::stoi(value);
    }
    else if (key == "outputFile") {
      outputFile = value;
    }
  }

  file.close();
  return true;
}

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "Russian");
  // Initialize the random number generator
  srand(time(nullptr));

  int N = 0; // Number of books
  int M = 0; // Number of readers
  // std::string outputFile = "output.txt";
  std::string outputFile = "";
  std::string configFile = "";

  // Process command-line arguments
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if ((arg == "-n" || arg == "--books") && i + 1 < argc) {
      N = std::atoi(argv[i + 1]);
      i++;
    }
    else if ((arg == "-m" || arg == "--readers") && i + 1 < argc) {
      M = std::atoi(argv[i + 1]);
      i++;
    }
    else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
      outputFile = argv[i + 1];
      std::cout << "Output file from command line: " << outputFile << std::endl;
      i++;
    }
    else if ((arg == "-c" || arg == "--config") && i + 1 < argc) {
      configFile = argv[i + 1];
      i++;
    }
    else if (arg == "-h" || arg == "--help") {
      std::cout << "Usage: " << argv[0] << " [-n number_of_books] [-m number_of_readers] [-o output_file] [-c config_file]\n";
      return 0;
    }
  }

  // If a configuration file is specified, read from it
  if (!configFile.empty()) {
    if (!readConfigFile(configFile, N, M, outputFile)) {
      std::cerr << "Error reading the configuration file.\n";
      return 1;
    }
    if (N <= 2) {
      std::cout << "Enter the number of books (N): ";
      while (!(std::cin >> N) || N <= 2) {
        std::cout << "Incorrect input. Enter a positive integer for N and >2: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }
  else {
    // If not specified, request input from the console
    if (N <= 2) {
      std::cout << "Enter the number of books (N): ";
      while (!(std::cin >> N) || N <= 2) {
        std::cout << "Incorrect input. Enter a positive integer for N and >2: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }

    if (M <= 0) {
      std::cout << "Enter the number of readers (M): ";
      while (!(std::cin >> M) || M <= 0) {
        std::cout << "Incorrect input. Enter a positive integer for M: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
    if (outputFile.empty()) {
      std::cout << "Enter the name of the output file: ";
      std::cin >> outputFile;
    }
    else {
      std::cout << "Output file is set to: " << outputFile << std::endl;
    }
  }

  // Open the output file
  std::ofstream ofs(outputFile);
  if (!ofs.is_open()) {
    std::cerr << "Failed to open the output file: " << outputFile << std::endl;
    return 1;
  }

  // Create a Logger object
  Logger logger(std::cout, ofs);

  // Create a Library object
  Library library(N, &logger);

  // Create readers
  std::vector<Reader*> readers;
  readers.reserve(M);
  for (int i = 0; i < M; ++i) {
    readers.push_back(new Reader(i + 1, &library, &logger));
  }

  // Create threads
  std::vector<pthread_t> threads(M);
  std::vector<ThreadData> threadData(M);
  for (int i = 0; i < M; ++i) {
    threadData[i].reader = readers[i];
    if (pthread_create(&threads[i], nullptr, threadFunction, &threadData[i]) != 0) {
      std::cerr << "Failed to create a thread for reader " << i + 1 << std::endl;
      return 1;
    }
  }

  // Wait for threads to finish
  for (int i = 0; i < M; ++i) {
    pthread_join(threads[i], nullptr);
  }

  // Free resources
  for (int i = 0; i < M; ++i) {
    delete readers[i];
  }

  ofs.close();

  std::cout << "The simulation is complete. The results are recorded in " << outputFile << std::endl;

  return 0;
}
