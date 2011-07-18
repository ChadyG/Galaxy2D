/*
	contactListener.cpp
	My Unnamed Game Engine
 
	Created by Chad on 11/12/08.
	Copyright 2009 BlitThis! studios. All rights reserved.
 */
#include "ContactListener.h"
#include <Box2D/Box2D.h>

#include "Scene/SceneObject.h"
#include "../Scene/GalPlayer.h"

void AdventureListener::BeginContact(b2Contact* contact)
{
	m_count++;

	b2Manifold* man = contact->GetManifold();

	const b2Filter& filter1 = contact->GetFixtureA()->GetFilterData();
	const b2Filter& filter2 = contact->GetFixtureB()->GetFilterData();
	
	if (filter1.categoryBits & filter2.maskBits || 
		filter2.categoryBits & filter1.maskBits) {
		mugeContact mCon;
		mCon.state = eConStart;
		mCon.manifold.localNormal = man->localNormal;
		mCon.manifold.localPoint = man->localPoint;
		mCon.obj1 = (SceneObject*)contact->GetFixtureA()->GetBody()->GetUserData();
		mCon.obj2 = (SceneObject*)contact->GetFixtureB()->GetBody()->GetUserData();
		mCon.fix1 = contact->GetFixtureA();
		mCon.fix2 = contact->GetFixtureB();
		m_Contacts.push_back( mCon );
	}
}

void AdventureListener::EndContact(b2Contact* contact)
{
	m_count--;

	b2Manifold* man = contact->GetManifold();

	mugeContact mCon;
	mCon.state = eConFinish;
	mCon.manifold.localNormal = man->localNormal;
	mCon.manifold.localPoint = man->localPoint;
	mCon.obj1 = (SceneObject*)contact->GetFixtureA()->GetBody()->GetUserData();
	mCon.obj2 = (SceneObject*)contact->GetFixtureB()->GetBody()->GetUserData();
	mCon.fix1 = contact->GetFixtureA();
	mCon.fix2 = contact->GetFixtureB();
	m_Contacts.push_back( mCon );
}

//TODO: do I want to add callbacks to SceneObject to filter these?
void AdventureListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	const b2Filter& filter1 = contact->GetFixtureA()->GetFilterData();
	const b2Filter& filter2 = contact->GetFixtureB()->GetFilterData();

	
	SceneObject *obj1 = (SceneObject*)contact->GetFixtureA()->GetBody()->GetUserData();
	SceneObject *obj2 = (SceneObject*)contact->GetFixtureB()->GetBody()->GetUserData();

	//Is this all I need?
	//would be a virtual call to sceneobject inheritors, then filter through components
	//if (!obj1->PreSolve(obj2, contact, oldManifold) || !obj2->PreSolve(obj1, contact, oldManifold)) {
	//	contact->SetEnabled(false);
	//}

	//Detect player collision on platforms
	if (filter1.categoryBits & 0x00F0 &&
  		filter2.categoryBits & 0x000F) {
		GalPlayer *play = (GalPlayer*)contact->GetFixtureA()->GetBody()->GetUserData();
		SceneObject *obj = (SceneObject*)contact->GetFixtureB()->GetBody()->GetUserData();
		float mdot = 0.f;
		float dot = b2Dot(play->getVelocity(), play->getGravity());
		//Is the platform beneath the player?
		if (oldManifold->type == b2Manifold::e_faceA) {
			mdot = b2Dot(oldManifold->localNormal, play->getGravity());
		}
		if (oldManifold->type == b2Manifold::e_faceB) {
			mdot = b2Dot(oldManifold->localNormal, -play->getGravity());
		}
		if (dot < 0.0f || mdot < 0.0f) {
			contact->SetEnabled(false);
		}
	}
	if (filter1.categoryBits & 0x000F &&
		filter2.categoryBits & 0x00F0){
		GalPlayer *play = (GalPlayer*)contact->GetFixtureB()->GetBody()->GetUserData();
		SceneObject *obj = (SceneObject*)contact->GetFixtureA()->GetBody()->GetUserData();
		float mdot = 0.f;
		float dot = b2Dot(play->getVelocity(), play->getGravity());
		//Is the platform beneath the player?
		if (oldManifold->type == b2Manifold::e_faceA) {
			mdot = b2Dot(oldManifold->localNormal, -play->getGravity());
		}
		if (oldManifold->type == b2Manifold::e_faceB) {
			mdot = b2Dot(oldManifold->localNormal, play->getGravity());
		}
		if (dot < 0.0f || mdot < 0.0f) {
			contact->SetEnabled(false);
		}
	}
}
void AdventureListener::Update()
{
	// Callbacks for objects
	std::vector< mugeContact >::iterator ic;
	//b2FilterData filter;
	for (ic = m_Contacts.begin(); ic != m_Contacts.end(); ++ic) {
		if (ic->state == eConStart) {
			if (ic->obj1 != NULL)
				ic->obj1->onColStart(ic->fix1, ic->obj2, ic->manifold);
			if (ic->obj2 != NULL)
				ic->obj2->onColStart(ic->fix2, ic->obj1, ic->manifold);
		}
		if (ic->state == eConFinish) {
			if (ic->obj1 != NULL)
				ic->obj1->onColFinish(ic->fix1, ic->obj2, ic->manifold);
			if (ic->obj2 != NULL)
				ic->obj2->onColFinish(ic->fix2, ic->obj1, ic->manifold);
		}
	}
	// Remove defunct contacts
	m_Contacts.clear();
}