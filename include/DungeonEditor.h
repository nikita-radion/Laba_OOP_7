#pragma once
#include "INPC.h"
#include "IObserver.h"
#include "BattleVisitor.h"
#include <vector>
#include <memory>

class DungeonEditor {
private:
    std::vector<std::unique_ptr<INPC>> npcs;
    std::vector<std::unique_ptr<IObserver>> observers;
    std::unique_ptr<BattleVisitor> battleVisitor;

public:
    DungeonEditor();
    void addNPC(const std::string& type, const std::string& name, int x, int y);
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void printNPCs() const;
    void startBattle(int range);
}; 