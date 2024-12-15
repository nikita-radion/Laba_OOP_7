#include "../include/GameState.h"
#include <shared_mutex>

GameState::GameState() : gameRunning(true) {}

void GameState::addNPC(std::unique_ptr<INPC> npc) {
    std::unique_lock lock(mutex);
    npcs.push_back(std::move(npc));
}

void GameState::removeNPC(size_t index) {
    std::unique_lock lock(mutex);
    if (index < npcs.size()) {
        npcs.erase(npcs.begin() + index);
    }
}

size_t GameState::getNPCCount() const {
    std::shared_lock lock(mutex);
    return npcs.size();
}

bool GameState::isGameRunning() const {
    std::shared_lock lock(mutex);
    return gameRunning;
}

void GameState::setGameRunning(bool running) {
    std::unique_lock lock(mutex);
    gameRunning = running;
} 