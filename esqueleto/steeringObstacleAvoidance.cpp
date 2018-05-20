#include "stdafx.h"
#include "steeringObstacleAvoidance.h"
#include "character.h"
#include <tinyxml.h>

void SteeringObstacleAvoidance::GetAcceleration(
    Character &character,
    Params &params,
    USVec2D &outLinearAcceleration,
    float &outAngularAcceleration)
{
    characterLocation = character.GetLoc();
    desiredLinearAcceleration = outLinearAcceleration;

    USVec2D characterDirection = character.GetLinearVelocity();
    characterDirection.NormSafe(); //np
    characterAheadVector = characterDirection * params.look_ahead; //lp
    
    for (auto& obstacle : obstacles) {
        // Projection of movement in perpendicular of center
        obstacle.characterObstacleVector = obstacle.center - characterLocation; //r0
        float dot = obstacle.characterObstacleVector.Dot(characterDirection); //proj
        obstacle.closestPointInCharDir = characterLocation + characterDirection * dot; //cercano
        
        // Check if current movement will cause a collision
        float diff = (obstacle.closestPointInCharDir - obstacle.center).Length();
        if (diff < obstacle.radius + params.char_radius) {
            // Ignore if it's behind
            if (dot > 0) {
                // Calculate steering
                USVec2D avoidVelocity = obstacle.closestPointInCharDir - obstacle.center;
                avoidVelocity.NormSafe();                
                // Steering strength (Penetration based)
                avoidVelocity.Scale(params.max_velocity / diff);
                
                desiredLinearAcceleration += avoidVelocity;
            }
        }
    }

    outLinearAcceleration = desiredLinearAcceleration;
}

void SteeringObstacleAvoidance::DrawDebug() {
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
    gfxDevice.SetPenColor(1.f, 1.f, 1.f, 1.f);
    gfxDevice.SetPenWidth(3.f);
    
    // Draw Obstacles - White
    for (auto obstacle : obstacles) {
        MOAIDraw::DrawEllipseOutline(obstacle.center.mX, obstacle.center.mY, obstacle.radius, obstacle.radius, 24);
        MOAIDraw::DrawLine(characterLocation, characterLocation + obstacle.characterObstacleVector);
    }

    // Draw collisions - Red
    gfxDevice.SetPenColor(1.f, 0.f, 0.f, 1.f);
    for (auto obstacle : obstacles) {
        MOAIDraw::DrawEllipseFill(obstacle.closestPointInCharDir.mX, obstacle.closestPointInCharDir.mY, 5.f, 5.f, 8);
        MOAIDraw::DrawPoint(obstacle.closestPointInCharDir);
        MOAIDraw::DrawLine(obstacle.closestPointInCharDir, obstacle.center);
    }

    // Character - Yellow
    gfxDevice.SetPenWidth(3.f);
    gfxDevice.SetPenColor(1.f, 1.f, 0.f, 1.f);
    MOAIDraw::DrawLine(characterLocation, characterLocation + characterAheadVector);
}

bool SteeringObstacleAvoidance::ReadObstacles(const char *filename) {
    // Load XML
    TiXmlDocument doc(filename);
    if (!doc.LoadFile())
    {
        fprintf(stderr, "Couldn't read params from %s", filename);
        return false;
    }
    TiXmlHandle hDoc(&doc);
    TiXmlElement* pElem = hDoc.FirstChildElement().Element();
    if (!pElem)
    {
        fprintf(stderr, "Invalid format for %s", filename);
        return false;
    }

    TiXmlElement* child = hDoc.FirstChild("root").FirstChild("obstacles").FirstChild("obstacle").ToElement();
    for (child; child; child=child->NextSiblingElement()) {
        float x, y, r;
        child->Attribute("x", &x);
        child->Attribute("y", &y);
        child->Attribute("r", &r);
        obstacles.push_back(Obstacle(USVec2D(x, y), r));
    }

    return true;
}
