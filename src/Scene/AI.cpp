/*
 *  AI.cpp
 *  Galaxy2D
 *
 *  Created by Chad on 11/23/10.
 *  Copyright 2010 BlitThis! All rights reserved.
 *
 */


#include "AI.h"
#include "PlanetObj.h"
#include "../Physics/QueryCallback.h"

AISimple::AISimple(SceneObject *_obj)
: Component(_obj)
{
}

void AISimple::encodeWith(Json::Value *_val)
{
}

void AISimple::initWith(Json::Value _val)
{
}

void AISimple::findGround() 
{
	b2Vec2 pos = m_Body->GetWorldCenter();
	b2Vec2 Gravity;
	b2Vec2 GravityNormal;
	
	//query world for surrounding shapes
	list_QueryCallback callback;
	callback.filter = 0x0100;
	b2AABB aabb;
	aabb.lowerBound.Set(pos.x-50.0f, pos.y-50.0f);
	aabb.upperBound.Set(pos.x+50.0f, pos.y+50.0f);
	m_World->QueryAABB(&callback, aabb);
	
	// loop over finding nearest point
	float nearest = 100.f;
	if (!callback.fixtures.empty()) {
		b2Shape *nearPlanet = callback.fixtures.front()->GetShape(), *curPlanet;
		b2CircleShape mshape;
		mshape.m_radius = 1.0f;
		b2Transform mtransform;
		mtransform.Set( pos, 0.f);
		
		std::list< b2Fixture* >::iterator it;
		for (it = callback.fixtures.begin(); it != callback.fixtures.end(); ++it) {
			curPlanet = (*it)->GetShape();
			
			b2DistanceInput input;
			input.proxyA.Set(&mshape);
			input.proxyB.Set(curPlanet);
			input.transformA = mtransform;
			input.transformB = (*it)->GetBody()->GetTransform();
			input.useRadii = true;
			b2SimplexCache cache;
			cache.count = 0;
			b2DistanceOutput output;
			b2Distance(&output, &cache, &input);
			
			if (output.distance < nearest) {
				nearPlanet = curPlanet;
				nearest = output.distance;
				b2RayCastOutput rback;
				b2RayCastInput rin;
				rin.p1 = output.pointA;
				rin.p2 = output.pointB;
				rin.maxFraction = 2.f;
				nearPlanet->RayCast(&rback, rin, (*it)->GetBody()->GetTransform());
				GravityNormal = rback.normal;
				Gravity = output.pointB - pos;
				Gravity.Normalize();
				SceneObject* obj = (SceneObject*)(*it)->GetBody()->GetUserData();
				PlanetComp* plan = (PlanetComp*)(obj->getComponent("Planet"));
				Gravity = plan->getMass() * 2.0f * Gravity;//((PlanetComp*)(*it)->GetBody()->GetUserData())->getMass() * 2.0f * Gravity;
			}
		}
		if (Gravity.Length() < 10.0)
			Gravity = (10.0f/Gravity.Length()) * Gravity;
		if (Gravity.Length() > 20.0)
			Gravity = (20.0f/Gravity.Length()) * Gravity;
	}else{
		Gravity.SetZero();
	}
	// when body found, cast two rays some arc length apart on either side to find tangent
	//float arcspan = 1.f / nearest;
	// we now have gravity
	// could improve by pulling towards on large distance, and doing tangent on closer
	m_Body->ApplyForce( Gravity, pos );
}

void AISimple::update() 
{
}

/// Physics callback
void AISimple::onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
}
void AISimple::onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
}

/// Message passing
void AISimple::onMessage(std::string _message)
{
}


AISimplecom_maker AISimplecom_maker::s_RegisteredMaker;

Component* AISimplecom_maker::makeComponent(SceneObject *_obj)
{
	return new AISimple(_obj);
}
