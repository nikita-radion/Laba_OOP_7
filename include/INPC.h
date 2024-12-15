#pragma once
#include <string>

class INPC {
public:
    virtual ~INPC() = default;
    virtual std::string getName() const = 0;
    virtual int getX() const = 0;
    virtual int getY() const = 0;
    virtual std::string getType() const = 0;
    virtual int getMoveRange() const = 0;
    virtual int getKillRange() const = 0;
    virtual void setPosition(int newX, int newY) = 0;
    virtual bool isAlive() const = 0;
    virtual void setAlive(bool alive) = 0;
}; 