/*
 Conveyor.h
 Mizzou Game Engine
 
 Created by Chad Godsey on 06/10/11.
 
 Copyright 2011 BlitThis!
 */
 
#include <Scene/SceneObject.h>
#include "GalPlayer.h"


class Conveyor : public Component
{
public:
	Conveyor(SceneObject *_obj);
	
	//Returns the name of this component type
	std::string name() { return std::string("Conveyor"); }
	
	//logic update callback
	void update();

	/// Physics callback
	void onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);
	void onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);
	bool PreSolve(SceneObject *_other, b2Contact *_contact, b2Manifold *_manifold) {return true;}

	/// Message passing
	void onMessage(std::string _message);

	//Serialization/Deserialization
	void encodeWith(Json::Value *_val);
	void initWith(Json::Value _val);
	
private:
	bool m_enabled;
	double m_conveyorSpeed;
	b2Vec2 m_direction;
	GalPlayer *m_thePlayer;
};

/// Builder for Trigger component
class Conveyorcom_maker : public Component_maker
{
public:
	Conveyorcom_maker() : Component_maker("Conveyor") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static Conveyorcom_maker s_RegisteredMaker;
};
