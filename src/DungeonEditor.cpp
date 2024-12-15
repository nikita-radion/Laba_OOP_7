#include "../include/DungeonEditor.h"
#include "../include/NPCFactory.h"
#include "../include/ConsoleObserver.h"
#include "../include/FileObserver.h"
#include <fstream>
#include <iostream>

DungeonEditor::DungeonEditor() {
    battleVisitor = std::make_unique<BattleVisitor>(0);
    observers.push_back(std::make_unique<ConsoleObserver>());
    observers.push_back(std::make_unique<FileObserver>("log.txt"));
    
    for (const auto& observer : observers) {
        battleVisitor->addObserver(observer.get());
    }
}

void DungeonEditor::addNPC(const std::string& type, const std::string& name, int x, int y) {
    npcs.push_back(NPCFactory::createNPC(type, name, x, y));
}

void DungeonEditor::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    for (const auto& npc : npcs) {
        file << npc->getType() << " " << npc->getName() << " " 
             << npc->getX() << " " << npc->getY() << "\n";
    }
}

void DungeonEditor::loadFromFile(const std::string& filename) {
    npcs.clear();
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            npcs.push_back(NPCFactory::loadNPCFromFile(line));
        }
    }
}

void DungeonEditor::printNPCs() const {
    for (const auto& npc : npcs) {
        std::cout << npc->getType() << " " << npc->getName() 
                 << " at (" << npc->getX() << "," << npc->getY() << ")\n";
    }
}

void DungeonEditor::startBattle(int range) {
    battleVisitor = std::make_unique<BattleVisitor>(range);
    
    for (const auto& observer : observers) {
        battleVisitor->addObserver(observer.get());
    }
    
    std::vector<bool> isDead(npcs.size(), false);
    
    for (size_t i = 0; i < npcs.size(); ++i) {
        for (size_t j = i + 1; j < npcs.size(); ++j) {
            battleVisitor->visit(npcs[i].get(), npcs[j].get());
            auto result = battleVisitor->getLastBattleResult();
            
            if (result.first) isDead[i] = true;
            if (result.second) isDead[j] = true;
        }
    }
    
    std::vector<std::unique_ptr<INPC>> survivors;
    for (size_t i = 0; i < npcs.size(); ++i) {
        if (!isDead[i]) {
            survivors.push_back(std::move(npcs[i]));
        }
    }
    
    npcs = std::move(survivors);
} 