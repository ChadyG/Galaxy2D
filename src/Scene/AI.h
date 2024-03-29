/*
 Floating.h
 Mizzou Game Engine
 
 Created by Chad Godsey on 11/23/10.
 
 Copyright 2009-2010 BlitThis!
 */

#include <MUGE.h>


class AISimple : public Component
{
public:
	AISimple(SceneObject *_obj);
	
	//Returns the name of this component type
	std::string name() { return std::string("AISimple"); }
	
	void findGround();
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
	b2Body *m_Body;
	b2World *m_World;
	Sprite *m_Sprite;
	
	double m_health;
	int m_hurtTimer;
	
	bool m_moveLeft;
};

/// Builder for Trigger component
class AISimplecom_maker : public Component_maker
{
public:
	AISimplecom_maker() : Component_maker("AISimple") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static AISimplecom_maker s_RegisteredMaker;
};


class AIFlying : public Component
{
public:
	AIFlying(SceneObject *_obj);
	
	//Returns the name of this component type
	std::string name() { return std::string("AIFlying"); }
	
	void findGround();
	void findPoint();
	void findPlayer();
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
	b2Body *m_Body;
	b2World *m_World;
	Sprite *m_Sprite;
	
	std::list< b2Vec2 > m_points, m_homingPoints;
	std::list< b2Vec2 >::iterator m_curPoint, m_nextPoint;

	bool m_loop, m_repeat, m_forward, m_finished, m_isPath, m_homing;
	double m_health, m_speed, m_ldist, m_proxDist, m_proxMax;
	int m_hurtTimer;
};

/// Builder for Trigger component
class AIFlyingcom_maker : public Component_maker
{
public:
	AIFlyingcom_maker() : Component_maker("AIFlying") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static AIFlyingcom_maker s_RegisteredMaker;
};
