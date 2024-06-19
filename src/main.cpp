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

void resetPlayerCircles(std::vector<sf::CircleShape>& playerCircles) {
    for (int i = 0; i < 64; ++i) {
        playerCircles[i].setPosition(0, 0);
    }
}

int main() {
    std::string mapName; std::cin >> mapName;

    Cheat cheat(L"cs2.exe");
    GameMap gameMap(mapName);

    sf::RenderWindow window(sf::VideoMode(512, 512), "CS2 Radar Hack");

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

        window.clear();

        window.draw(gameMap);

        int32_t playerCount = playerVec.size();
        if (playerCount != initialPlayerCount) {
            resetPlayerCircles(playerCircles);
            initialPlayerCount = playerCount;
        }

        for (size_t playerIdx = 0; playerIdx < playerVec.size(); ++playerIdx) {
            common::Vec2 radarPos = gameMap.getWorldPosToRadarPos(playerVec[playerIdx].pos);

            playerCircles[playerIdx].setPosition(radarPos.x / 2, radarPos.y / 2);

            if (playerVec[playerIdx].self) {
                playerCircles[playerIdx].setFillColor(sf::Color::Green);
            }
            else {
                if (playerVec[playerIdx].team == 2) {
                    playerCircles[playerIdx].setFillColor(sf::Color::Red);
                }
                else {
                    playerCircles[playerIdx].setFillColor(sf::Color::Cyan);
                }
            }
        }

        for (size_t i = 0; i < playerVec.size(); ++i) {
            window.draw(playerCircles[i]);
        }

        window.display();
    }
}