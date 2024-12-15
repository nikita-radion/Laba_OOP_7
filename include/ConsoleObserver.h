#pragma once
#include "IObserver.h"

class ConsoleObserver : public IObserver {
public:
    void update(const std::string& message) override;
}; 