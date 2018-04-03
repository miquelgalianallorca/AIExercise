#pragma once

#include "steering.h"

class SteeringArrive : public Steering {
public:
    SteeringArrive()  {}
    ~SteeringArrive() {}

    void GetAcceleration(
        Character &character,
        const Params &params,
        USVec2D &outLinearAcceleration,
        float &outAngularAcceleration);

    void DrawDebug();

private:
    USVec2D characterLocation;
    USVec2D desiredLinearVelocity;
    USVec2D desiredLinearAcceleration;
};