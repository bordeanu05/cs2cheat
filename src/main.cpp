#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

#include "../inc/common.hpp"
#include "../inc/memory.hpp"
#include "../inc/game_map.hpp"
#include "../inc/cheat.hpp"

void initPlayerBlips(std::vector<sf::CircleShape>&);

inline void handleEvents(sf::RenderWindow&);
inline void resetPlayerBlips(std::vector<sf::CircleShape>&);
inline void setPlayerBlipColor(sf::CircleShape&, const int32_t, const bool);

int main() {
    Cheat cheat(L"cs2.exe");
    GameMap gameMap("no_map");

    sf::RenderWindow radarWindow(sf::VideoMode(512, 512), "OBS Recording Software");

    std::vector<sf::CircleShape> playerBlips(64);
    initPlayerBlips(playerBlips);

    int32_t initialPlayerCount = 0;

    while (radarWindow.isOpen()) {
        handleEvents(radarWindow);

        // --- Getting game data
        std::vector<common::Player> playerVec = cheat.getPlayers();
        std::string currentMap = cheat.getMapName();
        // -----------------------------------------

        if (gameMap.getMapName() != currentMap) { // Check for map change
            gameMap.setMap(currentMap);
        }

        int32_t playerCount = static_cast<int32_t>(playerVec.size());
        if (initialPlayerCount != playerCount) { // Reset blips when players die/respawn
            initialPlayerCount = playerCount;
            resetPlayerBlips(playerBlips);
        }

        // --- Update player blips
        for (size_t playerIdx = 0; playerIdx < playerVec.size(); ++playerIdx) {
            common::Vec2 radarPos = gameMap.getWorldPosToRadarPos(playerVec[playerIdx].pos);

            playerBlips[playerIdx].setPosition(radarPos.x / 2, radarPos.y / 2);
            setPlayerBlipColor(playerBlips[playerIdx], playerVec[playerIdx].team, playerVec[playerIdx].self);
        }
        // --------------------------------------------------------------------------------------------------

        radarWindow.clear();

        radarWindow.draw(gameMap);

        if (currentMap != "<empty>") { // No need to draw blips if map is not loaded
            for (const auto& blip : playerBlips) {
                radarWindow.draw(blip);
            }
        }

        radarWindow.display();
    }
}

void initPlayerBlips(std::vector<sf::CircleShape>& playerBlips) {
    for (auto& circle : playerBlips) {
        circle.setRadius(5);
        circle.setFillColor(sf::Color::Red);
        circle.setPosition(0, 0);
    }
}

inline void handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

inline void resetPlayerBlips(std::vector<sf::CircleShape>& playerBlips) {
    for (auto& circle : playerBlips) {
        circle.setPosition(0, 0);
    }
}

inline void setPlayerBlipColor(sf::CircleShape& playerBlip, const int32_t team, const bool self) {
    if (self) {
        playerBlip.setFillColor(sf::Color::Green); // Player is green
    }
    else {
        if (team == 2) {
            playerBlip.setFillColor(sf::Color::Red); // Terrorists are red
        }
        else {
            playerBlip.setFillColor(sf::Color::Cyan); // Counter-Terrorists are cyan
        }
    }
}
