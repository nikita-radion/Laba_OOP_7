#include "../include/Squirrel.h"

Squirrel::Squirrel(const std::string& name, int x, int y) 
    : NPC(name, x, y) {}

std::string Squirrel::getType() const {
    return "Squirrel";
}

int Squirrel::getMoveRange() const {
    return 5;
}

int Squirrel::getKillRange() const {
    return 5;
} 