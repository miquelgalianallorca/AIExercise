#include "stdafx.h"
#include "steeringAlignToMovement.h"
#include "character.h"

#define RADTODEG 57.2958

void SteeringAlignToMovement::GetAcceleration(
    Character &character,
    Params &params,
    USVec2D &outLinearAcceleration,
    float &outAngularAcceleration)
{
    float angle = atan2f(character.GetLinearVelocity().mY,
        character.GetLinearVelocity().mX);
    params.targetRotation = angle * RADTODEG;
    steeringAlign.GetAcceleration(character, params,
        outLinearAcceleration, outAngularAcceleration);
}

void SteeringAlignToMovement::DrawDebug() {
    steeringAlign.DrawDebug();
}