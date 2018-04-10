#pragma once

#include "steeringAlign.h"

class SteeringAlignToMovement : public Steering {
public:
    SteeringAlignToMovement()  {}
    ~SteeringAlignToMovement() {}
    
    void GetAcceleration(
        Character &character,
        Params &params,
        USVec2D &outLinearAcceleration,
        float &outAngularAcceleration);

    void DrawDebug();

private:
    SteeringAlign steeringAlign;
};

