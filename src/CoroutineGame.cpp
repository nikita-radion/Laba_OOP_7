#include "../include/CoroutineGame.h"
#include "../include/NPCFactory.h"
#include "../include/ConsoleObserver.h"
#include "../include/FileObserver.h"
#include <iostream>
#include <cmath>

CoroutineGame::CoroutineGame() : rng(std::random_device{}()) {
    battleVisitor = std::make_unique<BattleVisitor>();
    
    auto consoleObs = std::make_unique<ConsoleObserver>();
    auto fileObs = std::make_unique<FileObserver>("coroutine_battle_log.txt");
    
    battleVisitor->addObserver(consoleObs.get());
    battleVisitor->addObserver(fileObs.get());
    
    observers.push_back(std::move(consoleObs));
    observers.push_back(std::move(fileObs));
    
    initializeNPCs();
}

void CoroutineGame::initializeNPCs() {
    std::uniform_int_distribution<int> dist(0, GameState::getMapSize() - 1);
    std::array<std::string, 3> types = {"Orc", "Squirrel", "Druid"};
    
    for (int i = 0; i < 50; ++i) {
        int x = dist(rng);
        int y = dist(rng);
        std::string type = types[i % 3];
        state.addNPC(NPCFactory::createNPC(type, "NPC" + std::to_string(i), x, y));
    }
}

void CoroutineGame::processMovementAndCombat() {
    std::uniform_int_distribution<int> dist(-1, 1);
    auto& npcs = state.getNPCs();
    
    std::vector<std::pair<INPC*, INPC*>> combatPairs;
    
    for (size_t i = 0; i < npcs.size(); ++i) {
        auto& npc1 = npcs[i];
        if (!npc1->isAlive()) continue;
        
        int dx = dist(rng) * npc1->getMoveRange();
        int dy = dist(rng) * npc1->getMoveRange();
        
        int newX = std::clamp(npc1->getX() + dx, 0, GameState::getMapSize() - 1);
        int newY = std::clamp(npc1->getY() + dy, 0, GameState::getMapSize() - 1);
        
        npc1->setPosition(newX, newY);
        
        for (size_t j = i + 1; j < npcs.size(); ++j) {
            auto& npc2 = npcs[j];
            if (!npc2->isAlive()) continue;
            
            int combatDx = npc1->getX() - npc2->getX();
            int combatDy = npc1->getY() - npc2->getY();
            double distance = std::sqrt(combatDx*combatDx + combatDy*combatDy);
            
            if (distance <= npc1->getKillRange() || distance <= npc2->getKillRange()) {
                combatPairs.emplace_back(npc1.get(), npc2.get());
            }
        }
    }
    
    for (const auto& [npc1, npc2] : combatPairs) {
        battleVisitor->visit(npc1, npc2);
        auto result = battleVisitor->getLastBattleResult();
        
        if (result.first) npc1->setAlive(false);
        if (result.second) npc2->setAlive(false);
    }
}

GameSimulation CoroutineGame::simulationCoroutine() {
    auto startTime = std::chrono::steady_clock::now();
    
    while (std::chrono::steady_clock::now() - startTime < std::chrono::seconds(30)) {
        {
            std::unique_lock<std::shared_mutex> lock(state.getMutex());
            processMovementAndCombat();
        }
        co_await TimeAwaiter(std::chrono::milliseconds(100));
    }
    
    state.setGameRunning(false);
}

void CoroutineGame::run() {
    auto simulation = simulationCoroutine();
    auto startTime = std::chrono::steady_clock::now();
    
    // Main thread handles map printing
    while (std::chrono::steady_clock::now() - startTime < std::chrono::seconds(30)) {
        printMap();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    state.setGameRunning(false);
    printSurvivors();
}

// Reuse the same printMap and printSurvivors methods from Game class
void CoroutineGame::printMap() {
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

void CoroutineGame::printSurvivors() {
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