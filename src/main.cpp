#include "../include/Game.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        std::cout << "Starting Balagur Fate 3 Battle Simulation\n";
        std::cout << "Map size: 100x100\n";
        std::cout << "Initial NPCs: 50\n";
        std::cout << "Simulation will run for 30 seconds\n\n";
        std::cout << "Legend:\n";
        std::cout << "O - Orc (Move: 20, Kill: 10)\n";
        std::cout << "S - Squirrel (Move: 5, Kill: 5)\n";
        std::cout << "D - Druid (Move: 10, Kill: 10)\n";
        std::cout << ". - Empty space\n\n";
        std::cout << "Press Enter to start...";
        
        std::cin.get();
        
        Game game;
        game.run();
        
        std::cout << "\nSimulation completed!\n";
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 