#include "stdafx.h"
#include "steeringArrive.h"
#include "character.h"

void SteeringArrive::GetAcceleration(
    Character &character,
    const USVec2D &targetLocation,
    USVec2D &outLinearAcceleration,
    float &outAngularAcceleration)
{
    characterLocation = character.GetLoc();

    desiredLinearVelocity = targetLocation - character.GetLoc();
    desiredLinearVelocity.NormSafe();
    desiredLinearVelocity.Scale(character.GetMaxVelocity());

    desiredLinearAcceleration = desiredLinearVelocity - character.GetLinearVelocity();
    desiredLinearAcceleration.NormSafe();
    desiredLinearAcceleration.Scale(character.GetMaxAcceleration());

    outLinearAcceleration = desiredLinearAcceleration;
}

void SteeringArrive::DrawDebug() {
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
    MOAIDraw::DrawLine(characterLocation,
        characterLocation + desiredLinearVelocity);

    gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);
    MOAIDraw::DrawLine(characterLocation,
        characterLocation + desiredLinearAcceleration);
}