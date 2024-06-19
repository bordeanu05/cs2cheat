#include "../inc/game_map.hpp"

GameMap::GameMap() : mMapName("no_map_selected"), mMapCoords({ 0.f, 0.f, 0.f }) {}

GameMap::GameMap(std::string mapName) {
    mMapName = mapName;
    mMapCoords = mMapCoordsDict[mapName];
    std::cout << "mMapCoords.x: " << mMapCoords.x << std::endl;
    std::cout << "mMapCoords.y: " << mMapCoords.y << std::endl;
    std::cout << "mMapCoords.z: " << mMapCoords.z << std::endl;
    loadRadarImg();
}

void GameMap::loadRadarImg() {
    std::string radarImgDir = "radar_imgs/" + mMapName + ".png";

    if (!mMapTexture.loadFromFile(radarImgDir)) {
        std::cerr << "Failed to load radar image!" << std::endl;
        exit(EXIT_FAILURE);
    }

    this->setTexture(mMapTexture);
    this->setScale(0.5f, 0.5f);
}