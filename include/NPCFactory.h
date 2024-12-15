#pragma once
#include "INPC.h"
#include <memory>
#include <string>

class NPCFactory {
public:
    static std::unique_ptr<INPC> createNPC(const std::string& type, const std::string& name, int x, int y);
    static std::unique_ptr<INPC> loadNPCFromFile(const std::string& filename);
}; 