/*
*  OneWayPlatform.cpp
*  Galaxy 2D Platformer
*
*  Created by Chad on 7/22/11.
*  Copyright 2009-2011 BlitThis! All rights reserved.
*
*/
 
//#include <limits>
#include "OneWayPlatform.h"
#include "GalPlayer.h"

//std::numeric_limits<double>::max())
 
OneWayPlatform::OneWayPlatform(SceneObject *_obj)
	: m_enabled(true), m_dropthrough(false)
{
	m_Obj = _obj;
}

void OneWayPlatform::encodeWith(Json::Value *_val)
{
	(*_val)["Enabled"] = Json::Value(m_enabled);
	(*_val)["DropThrough"] = Json::Value(m_dropthrough);

}

void OneWayPlatform::initWith(Json::Value _val)
{
	m_enabled = _val.get("Enabled", true).asBool();
	m_dropthrough = _val.get("DropThrough", true).asBool();
}


void OneWayPlatform::update() 
{
}

	/// Physics callback
void OneWayPlatform::onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
}
void OneWayPlatform::onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
}
bool OneWayPlatform::PreSolve(SceneObject *_other, b2Contact *_contact, const b2Manifold *_manifold)
{
	const b2Filter& filter1 = _contact->GetFixtureA()->GetFilterData();
	const b2Filter& filter2 = _contact->GetFixtureB()->GetFilterData();

	
	SceneObject *obj1 = (SceneObject*)_contact->GetFixtureA()->GetBody()->GetUserData();
	SceneObject *obj2 = (SceneObject*)_contact->GetFixtureB()->GetBody()->GetUserData();
	GalPlayer *play;

	float mdot = 0.f;
	if (obj1 == m_Obj) {
		play = (GalPlayer*)obj2;

		//Is the platform beneath the player?
		if (_manifold->type == b2Manifold::e_faceA) {
			mdot = b2Dot(_manifold->localNormal, -play->getGravity());
		}
		if (_manifold->type == b2Manifold::e_faceB) {
			mdot = b2Dot(_manifold->localNormal, play->getGravity());
		}
	}else{
		play = (GalPlayer*)obj1;
		if (_manifold->type == b2Manifold::e_faceA) {
			mdot = b2Dot(_manifold->localNormal, play->getGravity());
		}
		if (_manifold->type == b2Manifold::e_faceB) {
			mdot = b2Dot(_manifold->localNormal, -play->getGravity());
		}
	}
	
	float dot = b2Dot(play->getVelocity(), play->getGravity());
	if (dot < 0.0f || mdot < 0.0f) {
		return false;
	}

	return true;
}

	/// Message passing
void OneWayPlatform::onMessage(std::string _message)
{
}


OneWaycom_maker OneWaycom_maker::s_RegisteredMaker;

Component* OneWaycom_maker::makeComponent(SceneObject *_obj)
{
	return new OneWayPlatform(_obj);
}
