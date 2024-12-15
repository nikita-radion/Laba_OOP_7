#include "../include/Druid.h"

Druid::Druid(const std::string& name, int x, int y) 
    : NPC(name, x, y) {}

std::string Druid::getType() const {
    return "Druid";
}

int Druid::getMoveRange() const {
    return 10;
}

int Druid::getKillRange() const {
    return 10;
} 