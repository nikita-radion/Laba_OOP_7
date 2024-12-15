#include "../include/FileObserver.h"

FileObserver::FileObserver(const std::string& filename) {
    logFile.open(filename, std::ios::app);
}

FileObserver::~FileObserver() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void FileObserver::update(const std::string& message) {
    if (logFile.is_open()) {
        logFile << message << std::endl;
    }
} 