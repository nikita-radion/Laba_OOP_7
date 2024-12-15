#include "../include/Orc.h"

Orc::Orc(const std::string& name, int x, int y) 
    : NPC(name, x, y) {}

std::string Orc::getType() const {
    return "Orc";
}

int Orc::getMoveRange() const {
    return 20;
}

int Orc::getKillRange() const {
    return 10;
} 