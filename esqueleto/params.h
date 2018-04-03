#ifndef __PARAMS_H__
#define __PARAMS_H__

struct Params
{
    float max_velocity;
    float max_acceleration;
    float arrive_radius;
    float dest_radius;
    USVec2D targetPosition;

    float max_angular_velocity;
    float max_angular_acceleration;
    float angular_arrive_radius;
    float angular_dest_radius;
    float targetRotation;
};

bool ReadParams(const char* filename, Params& params);

#endif