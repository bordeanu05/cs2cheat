#include <iostream>
#include <thread>
#include <chrono>
#include <array>
#include <windows.h>

#include <SFML/Graphics.hpp>

#include "../inc/common.hpp"
#include "../inc/memory.hpp"
#include "../inc/game_map.hpp"
#include "../inc/cheat.hpp"

void makeWindowOnTop(sf::WindowHandle);
void initPlayerBlips(std::array<sf::CircleShape, 64>&);

inline void handleEvents(sf::RenderWindow&);
inline void handleGlobalInput(sf::RenderWindow&, bool&);
inline void updateBlips(std::array<sf::CircleShape, 64>&, const std::array<common::Player, 64>&,
    const int32_t, const GameMap&);

int main() {
    Cheat cheat(L"cs2.exe");
    GameMap gameMap("no_map");

    // --- Get CS2 window handle
    HWND cs2hwnd = FindWindowW(NULL, L"Counter-Strike 2");
    while (cs2hwnd == NULL) {
        std::cerr << "Failed to find CS2 window" << std::endl;
        cs2hwnd = FindWindowW(NULL, L"Counter-Strike 2");
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    }
    // -------------------------------------------------------

    sf::RenderWindow radarWindow(sf::VideoMode(256, 256), "OBS Recording Software", sf::Style::None);
    radarWindow.setFramerateLimit(60);

    makeWindowOnTop(radarWindow.getSystemHandle());

    std::array<sf::CircleShape, 64> playerBlips;
    initPlayerBlips(playerBlips);

    std::array<common::Player, 64> playersDataArr;

    int32_t playerCount = 0;

    bool radarVisible = true;

    while (radarWindow.isOpen()) {
        handleEvents(radarWindow);
        handleGlobalInput(radarWindow, radarVisible);

        // --- Set window position to CS2 window
        RECT rect;
        GetWindowRect(cs2hwnd, &rect);

        radarWindow.setPosition(sf::Vector2i(rect.left + 22, rect.top + 22));
        // ------------------------------------------------------------------

        // --- Getting game data
        int32_t currentPlayerCount;
        cheat.UpdatePlayersInfo(playersDataArr, currentPlayerCount);
        std::string currentMap = cheat.getMapName();
        // -----------------------------------------

        if (gameMap.getMapName() != currentMap) { // Check for map change
            gameMap.setMap(currentMap);
        }

        if (currentPlayerCount != playerCount) {
            playerCount = currentPlayerCount;
        }

        updateBlips(playerBlips, playersDataArr, playerCount, gameMap);

        radarWindow.clear();

        radarWindow.draw(gameMap);

        if (currentMap != "<empty>") { // No need to draw blips if map is not loaded
            for (size_t playerIdx = 0; playerIdx < playerCount; ++playerIdx) {
                radarWindow.draw(playerBlips[playerIdx]);
            }
        }

        radarWindow.display();
    }
}

void makeWindowOnTop(sf::WindowHandle handle) {
    long style = GetWindowLong(handle, GWL_EXSTYLE);
    SetWindowLong(handle, GWL_EXSTYLE, style | WS_EX_LAYERED | WS_EX_TRANSPARENT);
    SetWindowPos(handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void initPlayerBlips(std::array<sf::CircleShape, 64>& playerBlips) {
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

inline void handleGlobalInput(sf::RenderWindow& window, bool& radarVisible) {
    if (GetAsyncKeyState('V') & 0x8000) {
        radarVisible = !radarVisible;
        if (radarVisible) {
            window.setVisible(true);
        }
        else {
            window.setVisible(false);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

inline void updateBlips(std::array<sf::CircleShape, 64>& playerBlips, const std::array<common::Player, 64>& playersDataArr, const int32_t playerCount, const GameMap& gameMap) {
    for (size_t playerIdx = 0; playerIdx < playerCount; ++playerIdx) {
        common::Vec2 radarPos = gameMap.getWorldPosToRadarPos(playersDataArr[playerIdx].pos);

        playerBlips[playerIdx].setPosition(radarPos.x / 4, radarPos.y / 4); // div by 4 to scale down (radar size is 1024x1024)

        // --- Set player blip color
        if (playersDataArr[playerIdx].self) {
            playerBlips[playerIdx].setFillColor(sf::Color::Green); // Player is green
        }
        else {
            if (playersDataArr[playerIdx].team == 2) {
                playerBlips[playerIdx].setFillColor(sf::Color::Yellow); // Terrorists are red
            }
            else {
                playerBlips[playerIdx].setFillColor(sf::Color::Cyan); // Counter-Terrorists are cyan
            }
        }
    }
}
