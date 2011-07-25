/*
 Floating.h
 Mizzou Game Engine
 
 Created by Chad Godsey on 11/15/10.
 
 Copyright 2009-2010 BlitThis!
 */
 
#include <Scene/SceneObject.h>


class Floating : public Component
{
public:
	Floating(SceneObject *_obj);
	
	//Returns the name of this component type
	std::string name() { return std::string("Floating"); }
	
	//logic update callback
	void update();

	/// Physics callback
	void onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);
	void onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);
	bool PreSolve(SceneObject *_other, b2Contact *_contact, const b2Manifold *_manifold) {return true;}

	/// Message passing
	void onMessage(std::string _message);

	//Serialization/Deserialization
	void encodeWith(Json::Value *_val);
	void initWith(Json::Value _val);
	
private:
	std::list< b2Vec2 > m_points;
	std::list< b2Vec2 >::iterator m_curPoint, m_nextPoint;

	bool m_loop, m_repeat, m_forward, m_finished;
	double m_speed, m_ldist;
	b2Body *m_body;
};

/// Builder for Trigger component
class Floatcom_maker : public Component_maker
{
public:
	Floatcom_maker() : Component_maker("Floating") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static Floatcom_maker s_RegisteredMaker;
};
