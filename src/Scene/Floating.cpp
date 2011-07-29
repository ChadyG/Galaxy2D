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
	(*_val)["Repeat"] = Json::Value(m_repeat);
	(*_val)["Speed"] = Json::Value(m_speed);
	
	std::list< b2Transform >::iterator pit;
	for (pit = m_points.begin(); pit != m_points.end(); ++pit) {
		Json::Value val;
		val[0u] = pit->position.x;
		val[1u] = pit->position.y;
		val[2u] = pit->GetAngle();
		(*_val)["Points"].append(val);
	}
}

void Floating::initWith(Json::Value _val)
{
	m_loop = _val.get("Loop", true).asBool();
	m_repeat = _val.get("Repeat", true).asBool();
	m_speed = _val.get("Speed", 0.5).asDouble();
	
	for (int i = 0; i < _val["Points"].size(); ++i) {
		m_points.push_back( 
			b2Transform(
			  b2Vec2( _val["Points"][i].get(0u, 0.0).asDouble(),
			  _val["Points"][i].get(1u, 0.0).asDouble()),
			  b2Mat22(_val["Points"][i].get(2u, 0.0).asDouble()))
		);
	}

	m_curPoint = m_points.begin();
	m_nextPoint = m_points.begin();
	m_nextPoint++;
}


void Floating::update() 
{
	//Initialize
	if (m_body == NULL) {
		m_body = &((PhysComponent*)m_Obj->getComponent("Physics"))->getBody();

		b2Vec2 dvec = m_nextPoint->position - m_curPoint->position;
		dvec.Normalize();
		m_body->SetLinearVelocity(m_speed * dvec);
	}
	if (m_finished)
		return;

	//check for proximity to next point
	b2Vec2 pos = m_body->GetWorldCenter();
	float ang = m_body->GetAngle();
	double dtot = Gosu::distance(m_curPoint->position.x, m_curPoint->position.y, m_nextPoint->position.x, m_nextPoint->position.y);
	double d = Gosu::distance(pos.x, pos.y, m_nextPoint->position.x, m_nextPoint->position.y);
	if (d <= 0.01 || (d > m_ldist && d <= 1.0)) {
		//find new next point
		if (m_forward) {
			m_curPoint++;
			m_nextPoint++;
			if (m_nextPoint == m_points.end()) {
				if (m_loop) {
					m_curPoint = m_points.begin();
					m_nextPoint = m_points.begin();
					m_nextPoint++;
				}else if(m_repeat) {
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
					m_curPoint = m_points.end();
					m_nextPoint = m_points.end();
					m_nextPoint--;
				}else if(m_repeat) {
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
	
	//Move along path
	if (m_nextPoint != m_points.end()) {
		b2Vec2 dvec = m_nextPoint->position - pos;
		dvec.Normalize();
		m_body->SetLinearVelocity(m_speed * dvec);
		
		if (dtot > 0.1 || dtot < -0.1) {
			float tang = m_nextPoint->GetAngle() - m_curPoint->GetAngle();
			float mul = ((dtot-d)/dtot);
			mul = mul < 0.0f ? 0.0f : mul;
			float nang = m_curPoint->GetAngle() + mul*tang;
			float dang = nang - ang;
			m_body->SetAngularVelocity(Gosu::clamp(5.0f * dang, -2.0f, 2.0f));
		}
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
