#pragma once
#include "GameState.h"
#include "IObserver.h"
#include "BattleVisitor.h"
#include <coroutine>
#include <random>
#include <vector>
#include <queue>
#include <optional>
#include <thread>

struct GameSimulation {
    struct promise_type {
        GameSimulation get_return_object() { 
            return GameSimulation{std::coroutine_handle<promise_type>::from_promise(*this)}; 
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };

    std::coroutine_handle<promise_type> handle;
    
    GameSimulation(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~GameSimulation() {
        if (handle) handle.destroy();
    }
};

struct TimeAwaiter {
    TimeAwaiter(std::chrono::milliseconds time) : wait_time(time) {}
    
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> h) {
        std::thread([this, h]() {
            std::this_thread::sleep_for(wait_time);
            h.resume();
        }).detach();
    }
    void await_resume() {}
    
private:
    std::chrono::milliseconds wait_time;
};

class CoroutineGame {
private:
    GameState state;
    std::mutex coutMutex;
    std::mt19937 rng;
    std::vector<std::unique_ptr<IObserver>> observers;
    std::unique_ptr<BattleVisitor> battleVisitor;

    void initializeNPCs();
    void processMovementAndCombat();
    GameSimulation simulationCoroutine();
    void printMap();
    void printSurvivors();

public:
    CoroutineGame();
    void run();
    bool isRunning() const { return state.isGameRunning(); }
}; 