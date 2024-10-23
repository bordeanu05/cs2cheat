#pragma once

#include <vector>
#include <cstdint>

#include "common.hpp"
#include "offsets.hpp"
#include "memory.hpp"

class Cheat {
public:
    Cheat(const wchar_t*);

    std::vector<common::Player> getPlayers();
    std::string getMapName();

private:
    Memory mMem;

    uintptr_t mClient; // "client.dll"

    uintptr_t mEntityList;
    uintptr_t mListEntry;
    uintptr_t mLocalPlayerPawn;
    uintptr_t mGlobalVars;
};
