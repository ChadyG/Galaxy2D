/*
*  Floating.cpp
*  Galaxy 2D Platformer
*
*  Created by Chad on 12/15/10.
*  Copyright 2009-2010 BlitThis! All rights reserved.
*
*/
 
//#include <limits>
#include "Floating.h"

//std::numeric_limits<double>::max())
 
Floating::Floating(SceneObject *_obj)
	: Component(_obj), m_forward(true), m_finished(false), m_body(0), m_ldist(1000)
{
}

void Floating::encodeWith(Json::Value *_val)
{
	(*_val)["Loop"] = Json::Value(m_loop);
	(*_val)["Speed"] = Json::Value(m_speed);
	
	std::list< b2Vec2 >::iterator pit;
	for (pit = m_points.begin(); pit != m_points.end(); ++pit) {
		Json::Value val;
		val[0u] = pit->x;
		val[1u] = pit->y;
		(*_val)["Points"].append(val);
	}
}

void Floating::initWith(Json::Value _val)
{
	m_loop = _val.get("Loop", true).asBool();
	m_speed = _val.get("Speed", 0.5).asDouble();
	
	for (int i = 0; i < _val["Points"].size(); ++i) {
		m_points.push_back( 
			b2Vec2( _val["Points"][i].get(0u, 0.0).asDouble(),
			_val["Points"][i].get(1u, 0.0).asDouble())
		);
	}

	m_curPoint = m_points.begin();
	m_nextPoint = m_points.begin();
	m_nextPoint++;
}


void Floating::update() 
{
	if (m_body == NULL) {
		m_body = &((PhysComponent*)m_Obj->getComponent("Physics"))->getBody();

		b2Vec2 dvec = (*m_nextPoint) - (*m_curPoint);
		dvec.Normalize();
		m_body->SetLinearVelocity(m_speed * dvec);
	}
	if (m_finished)
		return;
	//check for proximity to next point
	b2Vec2 pos = m_body->GetWorldCenter();
	double d = Gosu::distance(pos.x, pos.y, m_nextPoint->x, m_nextPoint->y);
	if (d <= 0.01 || (d > m_ldist && d <= 1.0)) {
		//find new next point
		if (m_forward) {
			m_curPoint++;
			m_nextPoint++;
			if (m_nextPoint == m_points.end()) {
				if (m_loop) {
					m_forward = false;
					m_nextPoint = m_curPoint;
					m_nextPoint--;
				}else{
					m_finished = true;
				}
			}
		}else{
			if (m_nextPoint == m_points.begin()) {
				if (m_loop) {
					m_forward = true;
					m_curPoint = m_nextPoint;
					m_nextPoint++;
				}else{
					m_finished = true;
				}
			}else{
				m_curPoint--;
				m_nextPoint--;
			}
		}
	}
	
	if (m_nextPoint != m_points.end()) {
		b2Vec2 dvec = (*m_nextPoint) - pos;
		dvec.Normalize();
		m_body->SetLinearVelocity(m_speed * dvec);
	}
	m_ldist = d;
}

	/// Physics callback
void Floating::onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
}
void Floating::onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
}

	/// Message passing
void Floating::onMessage(std::string _message)
{
}


Floatcom_maker Floatcom_maker::s_RegisteredMaker;

Component* Floatcom_maker::makeComponent(SceneObject *_obj)
{
	return new Floating(_obj);
}
