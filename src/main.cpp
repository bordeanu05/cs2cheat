#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#include "../inc/memory.hpp"
#include "../inc/offsets.hpp"

struct CSEntity {
    char name[128];
};

struct Player {
    int health;
    int team;

    float xPos, yPos, zPos;
};

std::vector<Player> getPlayers(Memory& mem) {
    uintptr_t client = mem.getModuleAddress(L"client.dll");

    const auto localPlayerAddr = mem.memRead<uintptr_t>(client + offsets::localPlayerOffset);

    int localTeam = mem.memRead<int>(localPlayerAddr + offsets::m_iTeamNum);

    uintptr_t entityList = mem.memRead<uintptr_t>(client + offsets::dwEntityList);

    uintptr_t listEntry = mem.memRead<uintptr_t>(entityList + 0x10);

    std::vector<Player> playerVec;

    for (int i = 0; i < 64; ++i) {
        if (!listEntry) {
            continue;
        }

        uintptr_t currentController = mem.memRead<uintptr_t>(listEntry + i * 0x78);

        if (!currentController) {
            continue;
        }

        int pawnHandle = mem.memRead<int>(currentController + offsets::m_hPlayerPawn);
        if (!pawnHandle) {
            continue;
        }

        uintptr_t listEntry2 = mem.memRead<uintptr_t>(entityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);

        uintptr_t currentPawn = mem.memRead<uintptr_t>(listEntry2 + 0x78 * (pawnHandle & 0x1FF));

        int entHealth = mem.memRead<int>(currentPawn + offsets::m_iHealth);
        int entTeam = mem.memRead<int>(currentPawn + offsets::m_iTeamNum);

        if (entTeam == localTeam) {
            continue;
        }

        if (entHealth <= 0 || entHealth > 100) {
            continue;
        }

        float xPos = mem.memRead<float>(currentPawn + offsets::x_offset);
        float yPos = mem.memRead<float>(currentPawn + offsets::y_offset);
        float zPos = mem.memRead<float>(currentPawn + offsets::z_offset);

        playerVec.push_back({ entHealth, entTeam, xPos, yPos, zPos });
    }

    return playerVec;
}

int main() {
    Memory mem = Memory(L"cs2.exe");

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        system("cls");

        std::vector<Player> playerVec = getPlayers(mem);

        int playerCount = 0;
        for (auto player : playerVec) {
            std::cout << "Player " << playerCount++ << ": Health: " << player.health << " --- Team: " << (player.team == 2 ? "T" : "CT") << " --- Position: (" <<
                player.xPos << ", " << player.yPos << ", " << player.zPos << ")" << std::endl;
        }

        // TODO: infrumusetat cod, adaugat interfata cu harti
    }
}