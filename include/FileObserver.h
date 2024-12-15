#pragma once
#include "IObserver.h"
#include <fstream>

class FileObserver : public IObserver {
private:
    std::ofstream logFile;

public:
    FileObserver(const std::string& filename);
    ~FileObserver();
    void update(const std::string& message) override;
}; 