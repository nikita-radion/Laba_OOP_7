#pragma once
#include "GameState.h"
#include "IObserver.h"
#include "BattleVisitor.h"
#include <thread>
#include <mutex>
#include <random>
#include <vector>
#include <queue>
#include <condition_variable>

struct CombatTask {
    std::unique_ptr<INPC>* npc1;
    std::unique_ptr<INPC>* npc2;
    
    CombatTask(std::unique_ptr<INPC>* n1, std::unique_ptr<INPC>* n2) 
        : npc1(n1), npc2(n2) {}
        
    bool isValid() const { return npc1 != nullptr && npc2 != nullptr; }
};

class Game {
private:
    GameState state;
    std::mutex coutMutex;
    std::mutex queueMutex;
    std::condition_variable combatCV;
    std::queue<CombatTask> combatTasks;
    std::mt19937 rng;
    std::vector<std::unique_ptr<IObserver>> observers;
    std::unique_ptr<BattleVisitor> battleVisitor;

    void movementThread();
    void combatThread();
    void initializeNPCs();
    int rollDice();
    void printMap();
    void notifyObservers(const std::string& message);
    void printSurvivors();

public:
    Game();
    void addObserver(std::unique_ptr<IObserver> observer);
    void run();
    bool isRunning() const { return state.isGameRunning(); }
}; 