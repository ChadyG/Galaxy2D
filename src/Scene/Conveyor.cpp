/*
*  Conveyor.cpp
*  Galaxy 2D Platformer
*
*  Created by Chad on 06/10/11.
*  Copyright 2011 BlitThis! All rights reserved.
*
*/
 
//#include <limits>
#include "Conveyor.h"

//std::numeric_limits<double>::max())
 
Conveyor::Conveyor(SceneObject *_obj)
	: Component(_obj), m_enabled(true), m_conveyorSpeed(1.0)
{
}

void Conveyor::encodeWith(Json::Value *_val)
{
	(*_val)["Speed"] = Json::Value(m_conveyorSpeed);
	(*_val)["Enabled"] = Json::Value(m_enabled);
}

void Conveyor::initWith(Json::Value _val)
{
	m_conveyorSpeed = _val.get("Speed", true).asDouble();
	m_enabled = _val.get("Enabled", true).asDouble();
	m_direction.x = _val["Direction"].get(0u,true).asDouble();
	m_direction.y = _val["Direction"].get(1u,true).asDouble();
}


void Conveyor::update() 
{
	//if we have a reference to the player, we want to push him along
	if (m_thePlayer) {
		m_thePlayer->m_Body->ApplyLinearImpulse( m_conveyorSpeed*m_direction, m_thePlayer->m_Body->GetWorldCenter() );
	}
}

	/// Physics callback
void Conveyor::onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
	//keep it rollin rollin rollin
	if (_other->Name() == "Player") {
		m_thePlayer = (GalPlayer*)_other;
	}
}
void Conveyor::onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
	if (_other->Name() == "Player") {
		m_thePlayer = NULL;
	}
}

	/// Message passing
void Conveyor::onMessage(std::string _message)
{
}


Conveyorcom_maker Conveyorcom_maker::s_RegisteredMaker;

Component* Conveyorcom_maker::makeComponent(SceneObject *_obj)
{
	return new Conveyor(_obj);
}
