#include "../inc/cheat.hpp"

Cheat::Cheat(const wchar_t* processName) : mMem(processName) {
    mClient = mMem.getModuleAddress(L"client.dll");

    mEntityList = mMem.memRead<uintptr_t>(mClient + offsets::dwEntityList);
    mListEntry = mMem.memRead<uintptr_t>(mEntityList + 0x10);
}

void Cheat::UpdatePlayersInfo(std::array<common::Player, 64>& playerDataArr, int32_t& playerCount) {
    mLocalPlayerPawn = mMem.memRead<uintptr_t>(mClient + offsets::dwLocalPlayerPawn);
    playerCount = 0;

    for (size_t i = 0; i < 64; ++i) {
        uintptr_t currentController = mMem.memRead<uintptr_t>(mListEntry + i * 0x78);
        if (!currentController) {
            continue;
        }

        int32_t pawnHandle = mMem.memRead<int32_t>(currentController + offsets::m_hPlayerPawn);
        if (!pawnHandle) {
            continue;
        }

        uintptr_t listEntry2 = mMem.memRead<uintptr_t>(mEntityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);
        uintptr_t currentPawn = mMem.memRead<uintptr_t>(listEntry2 + 0x78 * (pawnHandle & 0x1FF));

        // ------- Read player info
        int32_t entHealth = mMem.memRead<int32_t>(currentPawn + offsets::m_iHealth);
        int32_t entTeam = mMem.memRead<int32_t>(currentPawn + offsets::m_iTeamNum);

        common::Vec3 entPos = mMem.memRead<common::Vec3>(currentPawn + offsets::m_vOldOrigin);

        bool isSelf = (currentPawn == mLocalPlayerPawn);
        bool isDead = (entHealth <= 0 || entHealth > 100);
        // ---------------------------------------------------------------

        if (!isDead) {
            playerDataArr[playerCount++] = { entHealth, entTeam, isSelf, entPos };
        }
    }
}

std::string Cheat::getMapName() {
    uintptr_t mGlobalVars = mMem.memRead<uintptr_t>(mClient + offsets::dwGlobalVars);
    uintptr_t mapAddress = mMem.memRead<uintptr_t>(mGlobalVars + offsets::m_current_mapname);

    if (!mapAddress) {
        return "<empty>";
    }

    std::string mapName;
    mMem.memReadString(mapAddress, mapName);

    return mapName;
}
