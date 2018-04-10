#include "stdafx.h"
#include "steeringPathFollow.h"
#include "character.h"
#include <tinyxml.h>

void SteeringPathFollow::GetAcceleration(
    Character &character,
    Params &params,
    USVec2D &outLinearAcceleration,
    float &outAngularAcceleration)
{

}

void SteeringPathFollow::DrawDebug() {
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
    gfxDevice.SetPenColor(0.0f, 1.0f, 1.0f, 0.5f);
    gfxDevice.SetPenWidth(3.f);
    // Draw Path
    MOAIDraw::DrawPoint(0.0f, 0.0f);
    cout << path.size() << endl;
    for (int i = 0; i < path.size(); i++) {
        MOAIDraw::DrawEllipseFill(path.at(i).mX, path.at(i).mY,
            10.f, 10.f, 10);
        if (i > 0) {
            MOAIDraw::DrawLine(path.at(i - 1), path.at(i));
        }
    }
}

bool SteeringPathFollow::ReadPath(const char *filename) {
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

    TiXmlElement* child = hDoc.FirstChild("root").
        FirstChild("points").FirstChild("point").ToElement();
    for( child; child; child=child->NextSiblingElement() )
    {
        int x, y;
        child->Attribute("x", &x);
        child->Attribute("y", &y);
        path.push_back(USVec2D(x, y));
    }
}