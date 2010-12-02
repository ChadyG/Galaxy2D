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
: Component(_obj), m_moveLeft(true)
{
}

void AISimple::encodeWith(Json::Value *_val)
{
}

void AISimple::initWith(Json::Value _val)
{
	m_moveLeft = _val["moveLeft"].asBool();
	m_World = SceneGraph::getCurrentContext()->getPhysics();

	b2CircleShape body;
	body.m_radius = 1.0f;
	body.m_p = b2Vec2(0.0f, 0.0f);

	b2FixtureDef fix;
	fix.density = 0.75f;
	fix.friction = 0.3f;
	fix.restitution = 0.3f;
	fix.filter.categoryBits = 0x0010;
	fix.filter.maskBits = 0x0FFF;
	fix.filter.groupIndex = 1;
	fix.shape = &body;

	b2BodyDef bDef;
	bDef.userData = m_Obj;
	bDef.type = b2_dynamicBody;
	bDef.position.Set( (float32)_val["Position"][0u].asDouble(), (float32)_val["Position"][1u].asDouble() );
	bDef.linearDamping = 0.5f;
	bDef.angularDamping = 1.85f;
	//bDef.fixedRotation = true;
	m_Body = m_World->CreateBody( &bDef );
	m_Body->CreateFixture( &fix );
	
	RenderManager* rendMan = RenderManager::getCurrentContext();
	m_Sprite = rendMan->createSprite( 0, Gosu::resourcePrefix() + L"Images/baddy.png");
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
	findGround();
	double roll = 10.f;
	if (m_moveLeft)
		roll = -roll;
	m_Body->ApplyTorque(roll);

	b2Vec2 pos = m_Body->GetPosition();
	m_Sprite->setX(pos.x);
	m_Sprite->setY(pos.y);
	m_Sprite->setAngle(m_Body->GetAngle() * (180.0f / (float)Gosu::pi));

	if (m_Obj->hasComponent("Transform")) {
		TransformComponent* tc = (TransformComponent*)m_Obj->getComponent("Transform");
		tc->setPosition((double)pos.x, (double)pos.y);
		tc->setRotation( m_Body->GetAngle() * (180.0f / (float)Gosu::pi));
	}
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
