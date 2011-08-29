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
#include "GalPlayer.h"
#include "../Physics/QueryCallback.h"
#include "boost/lexical_cast.hpp"

AISimple::AISimple(SceneObject *_obj)
: Component(_obj), m_moveLeft(true)
{
}

void AISimple::encodeWith(Json::Value *_val)
{
}

void AISimple::initWith(Json::Value _val)
{
	m_health = _val.get("Health", 5.0).asDouble();
	m_hurtTimer = 0;

	m_moveLeft = _val["moveLeft"].asBool();
	m_World = SceneGraph::getCurrentContext()->getPhysics();

	b2CircleShape body;
	body.m_radius = 1.0f;
	body.m_p = b2Vec2(0.0f, 0.0f);

	b2FixtureDef fix;
	fix.density = 0.75f;
	fix.friction = 0.3f;
	fix.restitution = 0.3f;
	fix.filter.categoryBits = 0x0200;
	fix.filter.maskBits = 0xFFFF;
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
	callback.filter = 0xFFFF;
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
			SceneObject* obj = (SceneObject*)(*it)->GetBody()->GetUserData();

			if (obj->hasComponent("Planet")) {
			
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
	if (m_health > 0.0) {
		if (m_moveLeft)
			roll = -roll;
		m_Body->ApplyTorque(roll);
		//==================================================
		// Start Hurt

		if (m_hurtTimer >= 0) {
			m_hurtTimer--;
			if (m_hurtTimer%4==0) {
				m_Sprite->setColorMod(0xff999999);
			}else{
				m_Sprite->setColorMod(0xffffffff);
			}
		}
		if (m_hurtTimer == 0 ) {
			m_hurtTimer = 0;
			m_Sprite->setColorMod(0xffffffff);
		}
	}else{
		m_Sprite->setColorMod(0xff999999);
	}

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
	unsigned int pIndex = _message.find("(", 0);
	unsigned int param = 0;

	if(pIndex != std::string::npos) {
		param = boost::lexical_cast< int >(_message.substr(pIndex + 1, 
			_message.find(")", pIndex) - pIndex - 1));
		_message = _message.substr(0, pIndex);
	}

	if (_message == "onHurt") {
		m_health -= param;
		m_hurtTimer = 25;
	}
}


AISimplecom_maker AISimplecom_maker::s_RegisteredMaker;

Component* AISimplecom_maker::makeComponent(SceneObject *_obj)
{
	return new AISimple(_obj);
}

//===================================================
//===================================================
//	AI Flying


AIFlying::AIFlying(SceneObject *_obj)
: Component(_obj), m_forward(true), m_finished(false), m_homing(false)
{
}

void AIFlying::encodeWith(Json::Value *_val)
{
	(*_val)["Health"] = Json::Value(m_health);
	(*_val)["Loop"] = Json::Value(m_loop);
	(*_val)["Repeat"] = Json::Value(m_repeat);
	(*_val)["Speed"] = Json::Value(m_speed);
	
	std::list< b2Vec2 >::iterator pit;
	for (pit = m_points.begin(); pit != m_points.end(); ++pit) {
		Json::Value val;
		val[0u] = pit->x;
		val[1u] = pit->y;
		(*_val)["Points"].append(val);
	}
}

void AIFlying::initWith(Json::Value _val)
{
	m_health = _val.get("Health", 5.0).asDouble();
	m_hurtTimer = 0;

	m_loop = _val.get("Loop", true).asBool();
	m_repeat = _val.get("Repeat", true).asBool();
	m_speed = _val.get("Speed", 0.5).asDouble();
	m_proxDist = _val.get("Proximity", 5.0).asDouble();
	m_proxMax = _val.get("ProximityMax", 20.0).asDouble();

	for (int i = 0; i < _val["Points"].size(); ++i) {
		m_points.push_back( 
			b2Vec2( _val["Points"][i].get(0u, 0.0).asDouble(),
			_val["Points"][i].get(1u, 0.0).asDouble())
		);
	}

	m_isPath = !m_points.empty();
	

	m_curPoint = m_points.begin();
	m_nextPoint = m_points.begin();
	m_nextPoint++;


	m_World = SceneGraph::getCurrentContext()->getPhysics();

	b2CircleShape body;
	body.m_radius = 1.0f;
	body.m_p = b2Vec2(0.0f, 0.0f);

	b2FixtureDef fix;
	fix.density = 0.75f;
	fix.friction = 0.3f;
	fix.restitution = 0.3f;
	fix.filter.categoryBits = 0x0200;
	fix.filter.maskBits = 0xFFFF;
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

void AIFlying::findGround() 
{
	b2Vec2 pos = m_Body->GetWorldCenter();
	b2Vec2 Gravity;
	b2Vec2 GravityNormal;
	
	//query world for surrounding shapes
	list_QueryCallback callback;
	callback.filter = 0xFFFF;
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
			SceneObject* obj = (SceneObject*)(*it)->GetBody()->GetUserData();

			if (obj->hasComponent("Planet")) {
			
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
	if (m_health <= 0.0)
		m_Body->ApplyForce( Gravity, pos );
}

void AIFlying::findPoint() 
{
	/*
		Two states
			path following, homing
		path follow is trivial
		homing will continue to target player 

		Both have cutoff point in distance to player for switching, 
		path follow has shorter distance to start homing.  Homing will 
		stop when player is far enough away, then will move immediately back to path.

		Alternate option is to continue homing in to last target position,
		then decide from there.  Playtest to see which is more appropriate
		in difficulty and fun.
	*/
	if (m_isPath && !m_homing) {
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
	}else{//not a path
		//fly randomly
	}
}

void AIFlying::findPlayer() 
{
	b2Vec2 pos = m_Body->GetWorldCenter();
	
	//query world for surrounding shapes
	list_QueryCallback callback;
	callback.filter = 0xFFFF;
	b2AABB aabb;
	aabb.lowerBound.Set(pos.x-50.0f, pos.y-50.0f);
	aabb.upperBound.Set(pos.x+50.0f, pos.y+50.0f);
	m_World->QueryAABB(&callback, aabb);

	bool found = false;
	double dist = (m_homing ? m_proxMax : m_proxDist);
	
	// loop over finding nearest point
	float nearest = 100.f;
	if (!callback.fixtures.empty()) {
		b2Shape *nearPlayer = callback.fixtures.front()->GetShape(), *curPlayer;
		b2CircleShape mshape;
		mshape.m_radius = 1.0f;
		b2Transform mtransform;
		mtransform.Set( pos, 0.f);
		
		std::list< b2Fixture* >::iterator it;
		for (it = callback.fixtures.begin(); it != callback.fixtures.end(); ++it) {
			curPlayer = (*it)->GetShape();
			SceneObject* obj = (SceneObject*)(*it)->GetBody()->GetUserData();

			if (obj->Name() == "Player") {

				b2DistanceInput input;
				input.proxyA.Set(&mshape);
				input.proxyB.Set(curPlayer);
				input.transformA = mtransform;
				input.transformB = (*it)->GetBody()->GetTransform();
				input.useRadii = true;
				b2SimplexCache cache;
				cache.count = 0;
				b2DistanceOutput output;
				b2Distance(&output, &cache, &input);
			
				if (output.distance < nearest && output.distance < dist) {
					found = true;

					nearPlayer = curPlayer;
					nearest = output.distance;
					b2RayCastOutput rback;
					b2RayCastInput rin;
					rin.p1 = output.pointA;
					rin.p2 = output.pointB;
					rin.maxFraction = 2.f;
					nearPlayer->RayCast(&rback, rin, (*it)->GetBody()->GetTransform());

					SceneObject* obj = (SceneObject*)(*it)->GetBody()->GetUserData();
					GalPlayer* play = (GalPlayer*)(obj);

					m_homingPoints.push_back( play->getPosition() );

					m_nextPoint = --m_homingPoints.end();
					
					m_homing = true;
				}
			}
		}
	}
	if (!found && m_homing == true) {
		m_homing = false;
		m_curPoint = m_points.begin();
		m_nextPoint = m_points.begin();
		m_nextPoint++;
	}
}

void AIFlying::update() 
{
	findGround();
	findPlayer();

	if (m_Body == NULL) {
		m_Body = &((PhysComponent*)m_Obj->getComponent("Physics"))->getBody();

		b2Vec2 dvec = (*m_nextPoint) - (*m_curPoint);
		dvec.Normalize();
		//m_Body->SetLinearVelocity(m_speed * dvec);
		m_Body->ApplyLinearImpulse(m_speed * dvec, m_Body->GetWorldCenter());
	}
	if (m_finished)
		return;
	
	//check for proximity to next point
	b2Vec2 pos = m_Body->GetWorldCenter();
	if (m_health > 0.0) {
		double d = Gosu::distance(pos.x, pos.y, m_nextPoint->x, m_nextPoint->y);
		if (d <= 0.01 || (d > m_ldist && d <= 1.0)) {
			//find new next point
			findPoint();
		}
	
		if (m_homing || m_nextPoint != m_points.end()) {
			b2Vec2 dvec = (*m_nextPoint) - pos;
			dvec.Normalize();
			//m_Body->SetLinearVelocity(m_speed * dvec);
			m_Body->ApplyLinearImpulse(0.1 * m_speed * dvec, m_Body->GetWorldCenter());
		}
		m_ldist = d;

		//==================================================
		// Start Hurt

		if (m_hurtTimer >= 0) {
			m_hurtTimer--;
			if (m_hurtTimer%4==0) {
				m_Sprite->setColorMod(0xff999999);
			}else{
				m_Sprite->setColorMod(0xffffffff);
			}
		}
		if (m_hurtTimer == 0 ) {
			m_hurtTimer = 0;
			m_Sprite->setColorMod(0xffffffff);
		}
	}else{
		m_Sprite->setColorMod(0xff999999);
	}


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
void AIFlying::onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
}
void AIFlying::onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
}

/// Message passing
void AIFlying::onMessage(std::string _message)
{
	unsigned int pIndex = _message.find("(", 0);
	unsigned int param = 0;

	if(pIndex != std::string::npos) {
		param = boost::lexical_cast< int >(_message.substr(pIndex + 1, 
			_message.find(")", pIndex) - pIndex - 1));
		_message = _message.substr(0, pIndex);
	}

	if (_message == "onHurt") {
		m_health -= param;
		m_hurtTimer = 25;
	}
}


AIFlyingcom_maker AIFlyingcom_maker::s_RegisteredMaker;

Component* AIFlyingcom_maker::makeComponent(SceneObject *_obj)
{
	return new AIFlying(_obj);
}
