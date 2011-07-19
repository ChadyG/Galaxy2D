/*
*  Hazard.h
*  Galaxy 2D Platformer
*
*  Created by Chad on 3/7/11.
*  Copyright 2009 BlitThis! All rights reserved.
*
*/
 
#include <MUGE.h>
#include <Scene/SceneObject.h>


class Hazard : public Component
{
public:
	Hazard(SceneObject *_obj);
	
	//Returns the name of this component type
	std::string name() { return std::string("Hazard"); }
	
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
	double m_damage;
	b2Body *m_body;
};

/// Builder for Trigger component
class Hazcom_maker : public Component_maker
{
public:
	Hazcom_maker() : Component_maker("Hazard") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static Hazcom_maker s_RegisteredMaker;
};
