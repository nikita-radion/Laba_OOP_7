#include "../include/Game.h"
#include "../include/NPCFactory.h"
#include "../include/ConsoleObserver.h"
#include "../include/FileObserver.h"
#include "../include/BattleVisitor.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>

Game::Game() : rng(std::random_device{}()) {

    battleVisitor = std::make_unique<BattleVisitor>();
    
    auto consoleObs = std::make_unique<ConsoleObserver>();
    auto fileObs = std::make_unique<FileObserver>("battle_log.txt");
    
    battleVisitor->addObserver(consoleObs.get());
    battleVisitor->addObserver(fileObs.get());
    
    observers.push_back(std::move(consoleObs));
    observers.push_back(std::move(fileObs));
    
    initializeNPCs();
}

void Game::initializeNPCs() {
    std::uniform_int_distribution<int> dist(0, GameState::getMapSize() - 1);
    std::array<std::string, 3> types = {"Orc", "Squirrel", "Druid"};
    
    for (int i = 0; i < 50; ++i) {
        int x = dist(rng);
        int y = dist(rng);
        std::string type = types[i % 3];
        state.addNPC(NPCFactory::createNPC(type, "NPC" + std::to_string(i), x, y));
    }
}

int Game::rollDice() {
    std::uniform_int_distribution<int> dist(1, 6);
    return dist(rng);
}

void Game::movementThread() {
    std::mt19937 moveRng(std::random_device{}());
    
    while (state.isGameRunning()) {
        std::vector<CombatTask> newTasks;
        
        {
            std::unique_lock<std::shared_mutex> stateLock(state.getMutex());
            
            auto& npcs = state.getNPCs();
            for (size_t i = 0; i < npcs.size(); ++i) {
                auto& npc1 = npcs[i];
                if (!npc1->isAlive()) continue;
                
                // Move NPC
                std::uniform_int_distribution<int> dist(-1, 1);
                int dx = dist(moveRng) * npc1->getMoveRange();
                int dy = dist(moveRng) * npc1->getMoveRange();
                
                int newX = std::clamp(npc1->getX() + dx, 0, GameState::getMapSize() - 1);
                int newY = std::clamp(npc1->getY() + dy, 0, GameState::getMapSize() - 1);
                
                npc1->setPosition(newX, newY);
                
                // Collect potential combats
                for (size_t j = i + 1; j < npcs.size(); ++j) {
                    auto& npc2 = npcs[j];
                    if (!npc2->isAlive()) continue;
                    
                    int dx = npc1->getX() - npc2->getX();
                    int dy = npc1->getY() - npc2->getY();
                    double distance = std::sqrt(dx*dx + dy*dy);
                    
                    if (distance <= npc1->getKillRange() || distance <= npc2->getKillRange()) {
                        newTasks.emplace_back(&npc1, &npc2);
                    }
                }
            }
        }
        
        if (!newTasks.empty()) {
            std::lock_guard<std::mutex> qLock(queueMutex);
            for (auto& task : newTasks) {
                combatTasks.push(std::move(task));
                combatCV.notify_one();
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Game::combatThread() {
    while (state.isGameRunning()) {
        CombatTask task{nullptr, nullptr};
        bool hasTask = false;
        
        {
            std::unique_lock<std::mutex> qLock(queueMutex);
            combatCV.wait(qLock, [this]() { 
                return !combatTasks.empty() || !state.isGameRunning(); 
            });
            
            if (!state.isGameRunning()) break;
            
            if (!combatTasks.empty()) {
                task = std::move(combatTasks.front());
                combatTasks.pop();
                hasTask = true;
            }
        }
        
        if (!hasTask || !task.isValid()) continue;
        
        {
            std::unique_lock<std::shared_mutex> stateLock(state.getMutex());
            
            battleVisitor->visit(task.npc1->get(), task.npc2->get());
            auto result = battleVisitor->getLastBattleResult();
            
            if (result.first) task.npc1->get()->setAlive(false);
            if (result.second) task.npc2->get()->setAlive(false);
        }
    }
}

void Game::printMap() {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::shared_lock<std::shared_mutex> npcsLock(state.getMutex());
    
    std::vector<std::vector<char>> map(GameState::getMapSize(), 
                                     std::vector<char>(GameState::getMapSize(), '.'));
    
    for (const auto& npc : state.getNPCs()) {
        if (npc->isAlive()) {
            char symbol = npc->getType()[0];  
            map[npc->getY()][npc->getX()] = symbol;
        }
    }
    
    std::cout << "\033[2J\033[H";  
    for (const auto& row : map) {
        for (char cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "Alive NPCs: " << std::count_if(state.getNPCs().begin(), 
                                                state.getNPCs().end(),
                                                [](const auto& npc) { return npc->isAlive(); })
              << '\n';
}

void Game::printSurvivors() {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::shared_lock<std::shared_mutex> npcsLock(state.getMutex());
    
    std::cout << "\nSurviving NPCs:\n";
    std::cout << "---------------\n";
    
    for (const auto& npc : state.getNPCs()) {
        if (npc->isAlive()) {
            std::cout << npc->getName() << " (" << npc->getType() << ") at position ["
                     << npc->getX() << "," << npc->getY() << "]\n";
        }
    }
}

void Game::run() {
    std::thread movement([this]() { movementThread(); });
    std::thread combat([this]() { combatThread(); });

    auto startTime = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - startTime < std::chrono::seconds(30)) {
        printMap();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    state.setGameRunning(false);
    combatCV.notify_all();  
    
    movement.join();
    combat.join();

    printSurvivors();
} 