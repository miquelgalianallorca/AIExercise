#pragma once

#include "steering.h"

class SteeringObstacleAvoidance : public Steering {
public:
    SteeringObstacleAvoidance()  {}
    ~SteeringObstacleAvoidance() {}

    bool ReadObstacles(const char* filename);
    void GetAcceleration(Character &character, Params &params, USVec2D &outLinearAcceleration, float &outAngularAcceleration);

    void DrawDebug();

private:
    struct Obstacle {
        Obstacle(USVec2D _center, float _radius) :
            center(_center),
            radius(_radius),
            closestPointInCharDir(USVec2D(0.f,0.f)),
            characterObstacleVector(USVec2D(0.f,0.f))
        {}

        USVec2D center;
        float radius;
        USVec2D closestPointInCharDir;
        
        
        USVec2D characterObstacleVector;
    };
    std::vector<Obstacle> obstacles;

    USVec2D characterLocation;
    //USVec2D characterDirection;
    USVec2D characterAheadVector;
};