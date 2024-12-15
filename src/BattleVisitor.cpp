#include "../include/BattleVisitor.h"
#include "../include/Orc.h"
#include "../include/Squirrel.h"
#include "../include/Druid.h"
#include <cmath>
#include <random>

void BattleVisitor::addObserver(IObserver* observer) {
    observers.push_back(observer);
}

int BattleVisitor::rollDice() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dist(1, 6);
    return dist(gen);
}

void BattleVisitor::visit(INPC* npc1, INPC* npc2) {
    lastBattleResult = {false, false}; 

    int dx = npc1->getX() - npc2->getX();
    int dy = npc1->getY() - npc2->getY();
    double distance = std::sqrt(dx*dx + dy*dy);
    
    bool npc1CanAttack = distance <= npc1->getKillRange();
    bool npc2CanAttack = distance <= npc2->getKillRange();
    
    if (!npc1CanAttack && !npc2CanAttack) return;

    bool npc1Dies = false;
    bool npc2Dies = false;

    if (npc1CanAttack) {
        if ((npc1->getType() == "Orc" && npc2->getType() == "Druid") ||
            (npc1->getType() == "Druid" && npc2->getType() == "Squirrel")) {
            int attackRoll = rollDice();  
            int defenseRoll = rollDice(); 
            
            if (attackRoll > defenseRoll) {
                npc2Dies = true;
            }
        }
    }
    
    if (npc2CanAttack) {
        if ((npc2->getType() == "Orc" && npc1->getType() == "Druid") ||
            (npc2->getType() == "Druid" && npc1->getType() == "Squirrel")) {
            int attackRoll = rollDice();  
            int defenseRoll = rollDice(); 
            
            if (attackRoll > defenseRoll) {
                npc1Dies = true;
            }
        }
    }

    lastBattleResult = {npc1Dies, npc2Dies};

    if (lastBattleResult.first || lastBattleResult.second) {
        std::string message;
        if (lastBattleResult.first) {
            message += npc2->getName() + " (" + npc2->getType() + ") killed " +
                      npc1->getName() + " (" + npc1->getType() + ")\n";
        }
        if (lastBattleResult.second) {
            message += npc1->getName() + " (" + npc1->getType() + ") killed " +
                      npc2->getName() + " (" + npc2->getType() + ")\n";
        }
        
        for (auto observer : observers) {
            observer->update(message);
        }
    }
} 