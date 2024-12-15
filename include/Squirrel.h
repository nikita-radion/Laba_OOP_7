#pragma once
#include "NPC.h"

class Squirrel : public NPC {
public:
    Squirrel(const std::string& name, int x, int y);
    std::string getType() const override;
    int getMoveRange() const override;
    int getKillRange() const override;
}; 