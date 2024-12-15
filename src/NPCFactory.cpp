#include "../include/NPCFactory.h"
#include "../include/Orc.h"
#include "../include/Squirrel.h"
#include "../include/Druid.h"
#include <fstream>
#include <sstream>

std::unique_ptr<INPC> NPCFactory::createNPC(const std::string& type, 
    const std::string& name, int x, int y) {
    if (type == "Orc") return std::make_unique<Orc>(name, x, y);
    if (type == "Squirrel") return std::make_unique<Squirrel>(name, x, y);
    if (type == "Druid") return std::make_unique<Druid>(name, x, y);
    throw std::invalid_argument("Unknown NPC type");
}

std::unique_ptr<INPC> NPCFactory::loadNPCFromFile(const std::string& line) {
    std::stringstream ss(line);
    std::string type, name;
    int x, y;
    ss >> type >> name >> x >> y;
    return createNPC(type, name, x, y);
} 