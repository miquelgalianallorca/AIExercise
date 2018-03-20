#pragma once

class Character;

class Steering {
public:
    Steering() {}
    virtual ~Steering() {}

    virtual void GetAcceleration(
        Character &character,
        const USVec2D &targetLocation,
        USVec2D &outLinearAcceleration,
        float &outAngularAcceleration) = 0;
    virtual void DrawDebug() = 0;
};