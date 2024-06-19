#include "../inc/cheat.hpp"

Cheat::Cheat(const wchar_t* processName) : mMem(processName) {
    mClient = mMem.getModuleAddress(L"client.dll");
    mEntityList = mMem.memRead<uintptr_t>(mClient + offsets::dwEntityList);
    mListEntry = mMem.memRead<uintptr_t>(mEntityList + 0x10);
}

std::vector<common::Player> Cheat::getPlayers() {
    std::vector<common::Player> playerVec;

    for (size_t i = 0; i < 64; ++i) {
        if (!mListEntry) {
            continue;
        }

        uintptr_t currentController = mMem.memRead<uintptr_t>(mListEntry + i * 0x78);

        if (!currentController) {
            continue;
        }

        int pawnHandle = mMem.memRead<int>(currentController + offsets::m_hPlayerPawn);
        if (!pawnHandle) {
            continue;
        }

        uintptr_t listEntry2 = mMem.memRead<uintptr_t>(mEntityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);

        uintptr_t currentPawn = mMem.memRead<uintptr_t>(listEntry2 + 0x78 * (pawnHandle & 0x1FF));

        int32_t entHealth = mMem.memRead<int32_t>(currentPawn + offsets::m_iHealth);
        int32_t entTeam = mMem.memRead<int32_t>(currentPawn + offsets::m_iTeamNum);

        if (entHealth <= 0 || entHealth > 100) {
            continue;
        }

        float xPos = mMem.memRead<float>(currentPawn + offsets::x_offset);
        float yPos = mMem.memRead<float>(currentPawn + offsets::y_offset);
        float zPos = mMem.memRead<float>(currentPawn + offsets::z_offset);

        uintptr_t localPlayerPawn = mMem.memRead<uintptr_t>(mClient + 0x181A9C8);

        if (currentPawn == localPlayerPawn) {
            playerVec.push_back({ entHealth, entTeam, true, {xPos, yPos, zPos} });
            continue;
        }

        playerVec.push_back({ entHealth, entTeam, false, {xPos, yPos, zPos} });
    }

    return playerVec;
}