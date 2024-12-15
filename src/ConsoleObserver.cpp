#include "../include/ConsoleObserver.h"
#include <iostream>

void ConsoleObserver::update(const std::string& message) {
    std::cout << message << std::endl;
} 