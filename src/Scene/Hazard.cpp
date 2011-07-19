/*
*  Hazard.cpp
*  Galaxy 2D Platformer
*
*  Created by Chad on 3/7/11.
*  Copyright 2009 BlitThis! All rights reserved.
*
*/

#include "Hazard.h"
#include "boost/lexical_cast.hpp"

//std::numeric_limits<double>::max())
 
Hazard::Hazard(SceneObject *_obj)
	: Component(_obj)
{
}

void Hazard::encodeWith(Json::Value *_val)
{
	(*_val)["Damage"] = Json::Value(m_damage);
}

void Hazard::initWith(Json::Value _val)
{
	m_damage = _val.get("Damage", 1.0).asDouble();
}


void Hazard::update() 
{
}

	/// Physics callback
void Hazard::onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
	_other->onMessage("onHurt(" + boost::lexical_cast<std::string>(m_damage) + ")");
}
void Hazard::onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
}

	/// Message passing
void Hazard::onMessage(std::string _message)
{
}


Hazcom_maker Hazcom_maker::s_RegisteredMaker;

Component* Hazcom_maker::makeComponent(SceneObject *_obj)
{
	return new Hazard(_obj);
}
