#include "stdafx.h"
#include "steeringAlign.h"
#include "character.h"

void SteeringAlign::GetAcceleration(
    Character &character,
    const Params &params,
    USVec2D &outLinearAcceleration,
    float &outAngularAcceleration)
{
    characterLocation = character.GetLoc();
    characterRotation = character.GetRot();

    desiredAngularVelocity = params.targetRotation - characterRotation;
    
    // Limit to range [-PI, PI]
    if (desiredAngularVelocity > 180)
        desiredAngularVelocity -= 2 * 180;
    else if (desiredAngularVelocity < -180)
        desiredAngularVelocity += 2 * 180;
    
    if (desiredAngularVelocity > params.max_angular_velocity)
        desiredAngularVelocity = params.max_angular_velocity;
    
    if (desiredAngularVelocity < params.angular_arrive_radius) {
        float factor = desiredAngularVelocity / params.arrive_radius;
        desiredAngularVelocity = params.max_angular_velocity * factor;
    }

    desiredAngularAcceleration = desiredAngularVelocity - character.GetAngularVelocity();
    if (desiredAngularAcceleration > params.max_angular_acceleration)
        desiredAngularAcceleration = params.max_angular_acceleration;

    outAngularAcceleration = desiredAngularAcceleration;
}

void SteeringAlign::DrawDebug() {
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    /*gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
    MOAIDraw::DrawLine(characterLocation,
        characterLocation + desiredLinearVelocity);

    gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);
    MOAIDraw::DrawLine(characterLocation,
        characterLocation + desiredLinearAcceleration);
*/
}