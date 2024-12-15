#pragma once
#include "INPC.h"
#include <vector>
#include <memory>
#include <mutex>
#include <shared_mutex>

class GameState {
private:
    std::vector<std::unique_ptr<INPC>> npcs;
    mutable std::shared_mutex mutex;
    bool gameRunning;
    static constexpr int MAP_SIZE = 100;

public:
    GameState();
    
    void addNPC(std::unique_ptr<INPC> npc);
    void removeNPC(size_t index);
    size_t getNPCCount() const;
    bool isGameRunning() const;
    void setGameRunning(bool running);
    static int getMapSize() { return MAP_SIZE; }

    std::shared_mutex& getMutex() { return mutex; }
    const std::vector<std::unique_ptr<INPC>>& getNPCs() const { return npcs; }
    std::vector<std::unique_ptr<INPC>>& getNPCs() { return npcs; }
}; 