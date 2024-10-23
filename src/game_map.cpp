#include "../inc/game_map.hpp"

GameMap::GameMap() : mMapName("<empty>"), mMapCoords({ 0.f, 0.f, 0.f }) {}

GameMap::GameMap(std::string mapName) {
    this->setScale(0.25f, 0.25f);

    mMapName = mapName;
    mMapCoords = mMapCoordsDict[mapName];
    loadRadarImg();
}

void GameMap::setMap(std::string mapName) {
    mMapName = mapName;
    mMapCoords = mMapCoordsDict[mapName];
    loadRadarImg();
}

std::string GameMap::getMapName() const {
    return mMapName;
}

common::Vec3 GameMap::getMapCoords() const {
    return mMapCoords;
}

common::Vec2 GameMap::getWorldPosToRadarPos(const common::Vec3 playerPos) const {
    float xPos = abs(playerPos.x - mMapCoords.x) / mMapCoords.z - 10;
    float yPos = abs(playerPos.y - mMapCoords.y) / mMapCoords.z - 10;

    return { xPos, yPos };
}

void GameMap::loadRadarImg() {
    bool mapExists = mMapCoordsDict.find(mMapName) != mMapCoordsDict.end();

    if (!mapExists) {
        mMapName = "<empty>";
        mMapCoords = mMapCoordsDict["<empty>"];

        if (!mMapTexture.loadFromFile("radar_imgs/no_map.png")) {
            std::cerr << "Failed to load radar image for map: no_map.png";
        }
    }
    else {
        std::string tempMapName = (mMapName == "<empty>" ? "no_map" : mMapName);
        std::string radarImgDir = "radar_imgs/" + tempMapName + ".png";

        if (!mMapTexture.loadFromFile(radarImgDir)) {
            std::cerr << "Failed to load radar image for map: " << mMapName << std::endl;
        }
    }

    this->setTexture(mMapTexture);
}
