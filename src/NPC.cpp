#include "../include/NPC.h"
#include <stdexcept>

NPC::NPC(const std::string& name, int x, int y) 
    : name(name), x(x), y(y), alive(true) {
    if (x < 0 || x >= 100 || y < 0 || y >= 100) {
        throw std::invalid_argument("Coordinates must be between 0 and 99");
    }
}

std::string NPC::getName() const { return name; }
int NPC::getX() const { return x; }
int NPC::getY() const { return y; }

void NPC::setPosition(int newX, int newY) {
    if (newX >= 0 && newX < 100 && newY >= 0 && newY < 100) {
        x = newX;
        y = newY;
    }
}

bool NPC::isAlive() const { return alive; }
void NPC::setAlive(bool isAlive) { alive = isAlive; } 