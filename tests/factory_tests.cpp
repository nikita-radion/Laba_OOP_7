#include <gtest/gtest.h>
#include "../include/NPCFactory.h"
#include <fstream>
#include <filesystem>

TEST(FactoryTests, CreateNPC) {
    auto orc = NPCFactory::createNPC("Orc", "Thrall", 99, 99);
    EXPECT_EQ(orc->getType(), "Orc");
    EXPECT_EQ(orc->getName(), "Thrall");

    auto druid = NPCFactory::createNPC("Druid", "Malfurion", 10, 11);
    EXPECT_EQ(druid->getType(), "Druid");
    EXPECT_EQ(druid->getName(), "Malfurion");

    auto squirrel = NPCFactory::createNPC("Squirrel", "Nutkin", 10, 10);
    EXPECT_EQ(squirrel->getType(), "Squirrel");
    EXPECT_EQ(squirrel->getName(), "Nutkin");
}

TEST(FactoryTests, InvalidType) {
    EXPECT_THROW(NPCFactory::createNPC("Invalid", "Name", 50, 50), std::invalid_argument);
}

TEST(FactoryTests, LoadFromFile) {
    {
        std::ofstream test_file("test_npcs.txt");
        test_file << "Orc Thrall 50 50\n";
    }

    std::ifstream file("test_npcs.txt");
    std::string line;
    std::getline(file, line);
    auto npc = NPCFactory::loadNPCFromFile(line);
    
    EXPECT_EQ(npc->getType(), "Orc");
    EXPECT_EQ(npc->getName(), "Thrall");
    EXPECT_EQ(npc->getX(), 50);
    EXPECT_EQ(npc->getY(), 50);

    file.close();
    std::filesystem::remove("test_npcs.txt");
} 