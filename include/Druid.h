#pragma once
#include "NPC.h"

class Druid : public NPC {
public:
    Druid(const std::string& name, int x, int y);
    std::string getType() const override;
    int getMoveRange() const override;
    int getKillRange() const override;
}; 