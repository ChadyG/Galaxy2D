/*
*  SceneObject.h
*  Galaxy 2D Platformer
*
*  Created by Chad on 1/9/08.
*  Copyright 2009 Mizzou Game Design. All rights reserved.
*
*/
 
#include "PlanetObj.h"

 
PlanetComp::PlanetComp(SceneObject *_obj)
	: Component(_obj), m_Mass(0.0)
{
}

void PlanetComp::encodeWith(Json::Value *_val)
{
	(*_val)["Gravity"] = m_Mass;
}

void PlanetComp::initWith(Json::Value _val)
{
	m_Mass = _val.get("Gravity", 0.0).asDouble();
}

// Query stuff
double PlanetComp::getGravity( double x, double y)
{
	TransformComponent* tc = (TransformComponent*)m_Obj->getComponent("Transform");
	b2Vec2 pos;
	pos.x = tc->getPositionX();
	pos.y = tc->getPositionY();
	double dist = Gosu::distance( pos.x, pos.y, x, y);
	return m_Mass/(dist*dist);
}

void PlanetComp::update() {}

	/// Physics callback
void PlanetComp::onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
}
void PlanetComp::onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
}

	/// Message passing
void PlanetComp::onMessage(std::string _message)
{
}


Planetcom_maker Planetcom_maker::s_RegisteredMaker;

Component* Planetcom_maker::makeComponent(SceneObject *_obj)
{
	return new PlanetComp(_obj);
}

