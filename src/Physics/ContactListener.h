/*
	ContactListener.h
	My Unnamed Game Engine
 
	Created by Chad Godsey on 11/12/08.
	Copyright 2009 BlitThis! studios.
 */
 
#include <Box2D/Box2D.h>
#include <vector>

#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

class SceneObject;

enum mugeContactState {
	eConStart,
	eConPersist,
	eConFinish,
	eConDefunct
};

struct mugeContact
{
	SceneObject *obj1, *obj2;
	b2Fixture *fix1, *fix2;
	b2Manifold manifold;
	mugeContactState state;
};

/**
* Used by Box2D to provide physics callbacks
* derived functions are called during CCD and 
* should only be used to buffer contact lists
* as they may be called multiple times for a 
* single collision
*/
/*
class MenuListener : public b2ContactListener
{
	std::vector< b2ContactPoint > m_Contacts;
public:
	void Add(const b2ContactPoint* point);
	void Persist(const b2ContactPoint* point);
	void Remove(const b2ContactPoint* point);
	void Result(const b2ContactResult* point);
	int Update();
};
*/

class AdventureListener : public b2ContactListener
{
	std::vector< mugeContact > m_Contacts;
	int m_count, m_curID;
public:
	AdventureListener() : m_count(0), m_curID(0) {}

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact); 
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	//void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	void Update();

	void clear() { m_Contacts.clear(); }
};

#endif