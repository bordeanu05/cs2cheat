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

inline void resetPlayerCircles(std::vector<sf::CircleShape>& playerCircles) {
    for (int i = 0; i < 64; ++i) {
        playerCircles[i].setPosition(0, 0);
    }
}

inline void setPlayerCircleColor(sf::CircleShape& playerCircle, int team, bool self) {
    if (self) {
        playerCircle.setFillColor(sf::Color::Green);
    }
    else {
        if (team == 2) {
            playerCircle.setFillColor(sf::Color::Red);
        }
        else {
            playerCircle.setFillColor(sf::Color::Cyan);
        }
    }
}

int main() {
    Cheat cheat(L"cs2.exe");
    GameMap gameMap("no_map");

    sf::RenderWindow window(sf::VideoMode(512, 512), "OBS Recording Software");

    std::vector<sf::CircleShape> playerCircles(64);
    for (int i = 0; i < 64; ++i) {
        playerCircles[i].setRadius(5);
        playerCircles[i].setFillColor(sf::Color::Red);
        playerCircles[i].setPosition(0, 0);
    }

    int32_t initialPlayerCount = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        std::vector<common::Player> playerVec = cheat.getPlayers();
        std::string currentMap = cheat.getMapName();

        if (currentMap != gameMap.getMapName()) {
            gameMap.setMap(currentMap);
        }

        int32_t playerCount = playerVec.size();
        if (playerCount != initialPlayerCount) {
            resetPlayerCircles(playerCircles);
            initialPlayerCount = playerCount;
        }

        for (size_t playerIdx = 0; playerIdx < playerVec.size(); ++playerIdx) {
            common::Vec2 radarPos = gameMap.getWorldPosToRadarPos(playerVec[playerIdx].pos);

            playerCircles[playerIdx].setPosition(radarPos.x / 2, radarPos.y / 2);
            setPlayerCircleColor(playerCircles[playerIdx], playerVec[playerIdx].team, playerVec[playerIdx].self);
        }

        window.clear();

        window.draw(gameMap);

        if (currentMap != "no_map") {
            for (size_t i = 0; i < playerVec.size(); ++i) {
                window.draw(playerCircles[i]);
            }
        }

        window.display();
    }
}