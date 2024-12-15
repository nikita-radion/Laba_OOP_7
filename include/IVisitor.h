#pragma once
#include "INPC.h"

class IVisitor {
public:
    virtual ~IVisitor() = default;
    virtual void visit(INPC* npc1, INPC* npc2) = 0;
}; 