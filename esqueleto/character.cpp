#include <stdafx.h>
#include "character.h"
#include <tinyxml.h>

#include <params.h>

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f)
{
	RTTI_BEGIN
		RTTI_EXTEND (MOAIEntity2D)
	RTTI_END
}

Character::~Character()
{

}

void Character::OnStart()
{
    ReadParams("params.xml", mParams);
    steeringPathFollow.ReadPath("path.xml");
    steeringObstacleAvoidance.ReadObstacles("obstacles.xml");
}

void Character::OnStop()
{
}

void Character::OnUpdate(float step)
{
    // Update acceleration with steerings
    float angularAcceleration = 0.f;
    
    avoidanceAcceleration = USVec2D(0.f, 0.f);
    followingAcceleration = USVec2D(0.f, 0.f);
    steeringObstacleAvoidance.GetAcceleration(*this, mParams, avoidanceAcceleration, angularAcceleration);
    steeringPathFollow.GetAcceleration(*this, mParams, followingAcceleration, angularAcceleration);

    // Combine steerings
    USVec2D linearAcceleration = avoidanceAcceleration + followingAcceleration;
    
    // Update velocity with acceleration
    USVec2D newLinearVel = GetLinearVelocity() + linearAcceleration * step;
    if (newLinearVel.Length() > mParams.max_velocity) {
        newLinearVel.NormSafe();
        newLinearVel.Scale(mParams.max_velocity);
    }
    SetLinearVelocity(newLinearVel.mX, newLinearVel.mY);
    SetAngularVelocity(GetAngularVelocity() + angularAcceleration * step);
    
    // Update location and rotation
    SetLoc(GetLoc() + GetLinearVelocity() * step);
    SetRot(GetRot() + GetAngularVelocity() * step);
}

void Character::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);
    gfxDevice.SetPenWidth(3.f);
    
    // Draw origin
	MOAIDraw::DrawPoint(0.0f, 0.0f);
        
    // Draw steering
    steeringObstacleAvoidance.DrawDebug();
    steeringPathFollow.DrawDebug();

    // Draw movement
    gfxDevice.SetPenColor(0.f, 1.f, 0.f, 1.f);
    MOAIDraw::DrawLine(GetLoc(), GetLoc() + GetLinearVelocity());
    gfxDevice.SetPenWidth(1.f);
    MOAIDraw::DrawLine(GetLoc(), GetLoc() + avoidanceAcceleration);
    MOAIDraw::DrawLine(GetLoc(), GetLoc() + followingAcceleration);
}





// Lua configuration

void Character::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIEntity2D::RegisterLuaFuncs(state);
	
	luaL_Reg regTable [] = {
		{ "setLinearVel",			_setLinearVel},
		{ "setAngularVel",			_setAngularVel},
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

int Character::_setLinearVel(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")
	
	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetLinearVelocity(pX, pY);
	return 0;	
}

int Character::_setAngularVel(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")
	
	float angle = state.GetValue<float>(2, 0.0f);
	self->SetAngularVelocity(angle);

	return 0;
}
	