#ifndef CHEAT_HPP
#define CHEAT_HPP

#include <vector>
#include <cstdint>

#include "common.hpp"
#include "offsets.hpp"

#include "memory.hpp"

class Cheat {
private:
    Memory mMem;

    uintptr_t mClient;
    uintptr_t mEntityList;
    uintptr_t mListEntry;

public:
    Cheat(const wchar_t*);

    std::vector<common::Player> getPlayers();

};

#endif