#pragma once

#include <array>
#include <cstdint>

#include "common.hpp"
#include "offsets.hpp"
#include "memory.hpp"

class Cheat {
public:
    Cheat(const wchar_t*);

    void UpdatePlayersInfo(std::array<common::Player, 64>&, int32_t&);
    std::string getMapName();

private:
    Memory mMem;

    uintptr_t mClient; // "client.dll"

    uintptr_t mEntityList;
    uintptr_t mListEntry;
    uintptr_t mLocalPlayerPawn;
};
