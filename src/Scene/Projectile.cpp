/*
	Projectile.cpp
	My Unnamed Game Engine
 
	Created by Chad on 5/16/10.
	Copyright 2010 BlitThis! studios. All rights reserved.

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 */

#include <MUGE.h>

#include "Projectile.h"
#include "PlanetObj.h"
#include "GalPlayer.h"
#include "../Physics/QueryCallback.h"
#include "Scene/SceneGraph.h"

Projectile::Projectile(bool _hasGravity)
	: m_Valid(true), m_deathTimer(-1), m_explodable(false), m_Power(0.f), m_ricochet(0), m_lifetime(-1)
{
	m_hasGravity = _hasGravity;
	m_Obj = SceneGraph::getCurrentContext()->createObject("Bullet[1]");
}

Projectile::~Projectile()
{
	SceneGraph::getCurrentContext()->deleteObject(m_Obj);
	m_World->DestroyBody(m_Body);
}

void Projectile::init(b2Body *_body, b2World *_world)
{
	m_Obj->addComponent(this);

	m_Body = _body;  
	m_World = _world;  
	m_Body->SetUserData(m_Obj);
	m_Position = m_Body->GetWorldCenter();
	m_Body->ResetMassData();
}

void Projectile::setExplode(int _radius, float _power)
{
	m_explodable = true;
	m_radius = _radius;
	m_Power = _power;
}

void Projectile::onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
	if (m_ricochet == 0) {
		if (m_deathTimer == -1)
			m_deathTimer = 20;
	}else{
		--m_ricochet;
	}
}

void Projectile::onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
}

void Projectile::onMessage(std::string _message)
{
	if (_message == "explode" && m_deathTimer == -1) {
		m_deathTimer = 6;
	}
}

void Projectile::encodeWith(Json::Value *_val)
{
}
void Projectile::initWith(Json::Value _val)
{
}

void Projectile::findGround() 
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

void Projectile::update()
{
	b2Vec2 dir;
	dir.SetZero();
	if (m_Valid) {
		if (m_hasGravity)
			findGround();
		m_Position = m_Body->GetWorldCenter();
		dir = m_Body->GetLinearVelocity();

		if (m_lifetime > 0)
			m_lifetime--;
		if (m_lifetime == 0 && m_deathTimer == -1) 
			m_deathTimer = 20;

		if (m_deathTimer > 0)
			m_deathTimer--;
		if (m_deathTimer == 5 && m_explodable) {
			m_ExplosSprite = RenderManager::getCurrentContext()->createSprite(0, Gosu::resourcePrefix() + L"Images/explosion.png");
			m_ExplosSprite->setX(m_Position.x);
			m_ExplosSprite->setY(m_Position.y);
			m_ExplosSprite->setAngle(Gosu::angle(0.0, 0.0, dir.x, dir.y) - 90.0);
			AudioManager::getCurrentContext()->createSample(Gosu::resourcePrefix() + L"Sound/explode.wav", "Explosion");
			AudioManager::getCurrentContext()->playStereoSample("Explosion", m_Position.x, m_Position.y);
		}
		if (m_deathTimer == 0) {
			m_Valid = false;
			m_Sprite->setVisible(false);
			RenderManager::getCurrentContext()->deleteSprite(m_Sprite);
			if (m_explodable) {
				m_ExplosSprite->setVisible(false);
				RenderManager::getCurrentContext()->deleteSprite(m_ExplosSprite);
				//query world for surrounding shapes
				list_QueryCallback callback;
				callback.filter = 0xFFFF;
				b2AABB aabb;
				aabb.lowerBound.Set(m_Position.x - m_radius, m_Position.y - m_radius);
				aabb.upperBound.Set(m_Position.x + m_radius, m_Position.y + m_radius);
				m_World->QueryAABB(&callback, aabb);

				std::list< b2Fixture* >::iterator it;
				for (it = callback.fixtures.begin(); it != callback.fixtures.end(); ++it) {
					SceneObject *obj = (SceneObject*)(*it)->GetBody()->GetUserData();
					if (obj != NULL) {
						obj->onMessage("explode");
					}
					b2Vec2 d = (*it)->GetBody()->GetWorldCenter() - m_Position;
					float pow = m_Power*Gosu::clamp((m_Power - d.Length())/m_Power, 0.f, 1.f);
					d.Normalize();
					(*it)->GetBody()->ApplyForce(pow*d, (*it)->GetBody()->GetWorldCenter());
				}
			}
			//m_World->DestroyBody(m_Body);
		}else{
			m_Sprite->setX(m_Position.x);
			m_Sprite->setY(m_Position.y);
			m_Sprite->setAngle( Gosu::angle(0.0, 0.0, dir.x, dir.y) - 90.0 );
		}
	}
}