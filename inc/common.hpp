#pragma once

namespace common {

struct Vec2 {
    float x, y;
};

struct Vec3 {
    float x, y, z;
};

struct Player {
    int32_t health;
    int32_t team;
    bool self;

    common::Vec3 pos;
};

} // namespace common
