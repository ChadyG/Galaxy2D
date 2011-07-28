/*
 OneWayPlatform.h
 Mizzou Game Engine
 
 Created by Chad Godsey on 7/22/11.
 
 Copyright 2009-2011 BlitThis!
 */
 
#include <Scene/SceneObject.h>


class OneWayPlatform : public Component
{
public:
	OneWayPlatform(SceneObject *_obj);
	
	//Returns the name of this component type
	std::string name() { return std::string("OneWayPlatform"); }
	
	//logic update callback
	void update();

	/// Physics callback
	void onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);
	void onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);
	bool PreSolve(SceneObject *_other, b2Contact *_contact, const b2Manifold *_manifold);

	/// Message passing
	void onMessage(std::string _message);

	//Serialization/Deserialization
	void encodeWith(Json::Value *_val);
	void initWith(Json::Value _val);
	
private:
	std::list< b2Vec2 > m_points;
	std::list< b2Vec2 >::iterator m_curPoint, m_nextPoint;

	bool m_enabled, m_dropthrough;
};

/// Builder for Trigger component
class OneWaycom_maker : public Component_maker
{
public:
	OneWaycom_maker() : Component_maker("OneWayPlatform") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static OneWaycom_maker s_RegisteredMaker;
};
