#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <map>

#include "common.hpp"

class GameMap : public sf::Sprite {
public:
    GameMap();
    GameMap(std::string);

    void setMap(std::string);

    std::string getMapName() const;

    common::Vec3 getMapCoords() const;

    common::Vec2 getWorldPosToRadarPos(const common::Vec3) const;

private:
    std::string mMapName;
    common::Vec3 mMapCoords;

    sf::Texture mMapTexture;

    std::map<std::string, common::Vec3> mMapCoordsDict = {
        { "cs_italy",    { -2647.f, 2592.f, 4.6f  }},
        { "cs_office",   { -1838.f, 1858.f, 4.1f  }},
        { "de_ancient",  { -2953.f, 2164.f, 5.f   }},
        { "de_anubis",   { -2796.f, 3328.f, 5.22f }},
        { "de_dust",     { -2850.f, 4073.f, 6.f   }},
        { "de_dust2",    { -2476.f, 3239.f, 4.4f  }},
        { "de_inferno",  { -2087.f, 3870.f, 4.9f  }},
        { "de_mirage",   { -3230.f, 1713.f, 5.f   }},
        { "de_nuke",     { -3453.f, 2887.f, 7.f   }},
        { "de_overpass", { -4831.f, 1781.f, 5.2f  }},
        { "de_vertigo",  { -3168.f, 1762.f, 4.f   }},
        { "<empty>",     {     0.f,    0.f, 1.f   }}
    };

    void loadRadarImg();
};
