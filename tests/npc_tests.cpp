#include <gtest/gtest.h>
#include "../include/Orc.h"
#include "../include/Druid.h"
#include "../include/Squirrel.h"

TEST(NPCTests, Creation) {
    Orc orc("Thrall", 50, 50);
    EXPECT_EQ(orc.getName(), "Thrall");
    EXPECT_EQ(orc.getX(), 50);
    EXPECT_EQ(orc.getY(), 50);
    EXPECT_EQ(orc.getType(), "Orc");
}

TEST(NPCTests, InvalidCoordinates) {
    EXPECT_THROW(Orc("Thrall", -1, 100), std::invalid_argument);
    EXPECT_THROW(Orc("Thrall", 501, 100), std::invalid_argument);
    EXPECT_THROW(Orc("Thrall", 100, -1), std::invalid_argument);
    EXPECT_THROW(Orc("Thrall", 100, 501), std::invalid_argument);
} 