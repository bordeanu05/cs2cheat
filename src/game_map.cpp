#include "../inc/game_map.hpp"

GameMap::GameMap() : mMapName("no_map_selected"), mMapCoords({ 0.f, 0.f, 0.f }) {}

GameMap::GameMap(std::string mapName) {
    mMapName = mapName;
    mMapCoords = mMapCoordsDict[mapName];
    loadRadarImg();
}

void GameMap::setMap(std::string mapName) {
    mMapName = mapName;
    mMapCoords = mMapCoordsDict[mapName];
    loadRadarImg();
}

void GameMap::loadRadarImg() {
    bool mapExists = mMapCoordsDict.find(mMapName) != mMapCoordsDict.end();
    if (!mapExists) {
        mMapName = "no_map";
        mMapCoords = mMapCoordsDict["no_map"];

        if (!mMapTexture.loadFromFile("radar_imgs/no_map.png")) {
            std::cerr << "Failed to load radar image for map: " << mMapName << std::endl;
        }
    }
    else {
        std::string radarImgDir = "radar_imgs/" + mMapName + ".png";

        if (!mMapTexture.loadFromFile(radarImgDir)) {
            std::cerr << "Failed to load radar image for map: " << mMapName << std::endl;
        }
    }

    this->setTexture(mMapTexture);
    this->setScale(0.5f, 0.5f);
}