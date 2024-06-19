#pragma once

namespace common {

struct Vec2 {
    float x, y;
};

struct Vec3 {
    float x, y, z;
};

struct Player {
    int health;
    int team;
    bool self;

    common::Vec3 pos;
};

}