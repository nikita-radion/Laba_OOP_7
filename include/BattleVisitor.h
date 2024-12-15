#pragma once
#include "IVisitor.h"
#include "IObserver.h"
#include <vector>

class BattleVisitor : public IVisitor {
private:
    std::vector<IObserver*> observers;
    std::pair<bool, bool> lastBattleResult;
    
    int rollDice();

public:
    BattleVisitor() = default;
    void visit(INPC* npc1, INPC* npc2) override;
    void addObserver(IObserver* observer);
    std::pair<bool, bool> getLastBattleResult() const { return lastBattleResult; }
}; 