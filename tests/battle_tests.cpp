#include <gtest/gtest.h>
#include "../include/BattleVisitor.h"
#include "../include/Orc.h"
#include "../include/Druid.h"
#include "../include/Squirrel.h"

class BattleTests : public ::testing::Test {
protected:
    std::unique_ptr<BattleVisitor> visitor;
    std::unique_ptr<Orc> orc;
    std::unique_ptr<Druid> druid;
    std::unique_ptr<Squirrel> squirrel;

    void SetUp() override {
        visitor = std::make_unique<BattleVisitor>();
        orc = std::make_unique<Orc>("Thrall", 0, 0);
        druid = std::make_unique<Druid>("Malfurion", 10, 11);
        squirrel = std::make_unique<Squirrel>("Nutkin", 10, 10);
    }
};

TEST_F(BattleTests, OrcKillsDruid) {
    visitor->visit(orc.get(), druid.get());
    auto result = visitor->getLastBattleResult();
    EXPECT_FALSE(result.first);  
    EXPECT_TRUE(result.second);  
}

TEST_F(BattleTests, DruidKillsSquirrel) {
    visitor->visit(druid.get(), squirrel.get());
    auto result = visitor->getLastBattleResult();
    EXPECT_FALSE(result.first);   
    EXPECT_TRUE(result.second);   
}

TEST_F(BattleTests, SquirrelPacifist) {
    visitor->visit(squirrel.get(), orc.get());
    auto result = visitor->getLastBattleResult();
    EXPECT_FALSE(result.first);   
    EXPECT_FALSE(result.second);  
}

TEST_F(BattleTests, OutOfRange) {
    auto farOrc = std::make_unique<Orc>("Grom", 99, 99);
    visitor->visit(orc.get(), farOrc.get());
    auto result = visitor->getLastBattleResult();
    EXPECT_FALSE(result.first);
    EXPECT_FALSE(result.second);
} 