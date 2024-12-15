#pragma once
#include "INPC.h"

class NPC : public INPC {
protected:
    std::string name;
    int x, y;
    bool alive;

public:
    NPC(const std::string& name, int x, int y);
    std::string getName() const override;
    int getX() const override;
    int getY() const override;
    void setPosition(int newX, int newY) override;
    bool isAlive() const override;
    void setAlive(bool alive) override;
}; 