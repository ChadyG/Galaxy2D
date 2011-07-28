/*
 PlanetObj.h
 Mizzou Game Engine
 
 Created by Chad Godsey on 7/12/09.
 
 Copyright 2009 Mizzou Game Design.
 */
 
#include <Scene/SceneObject.h>

class PlanetComp : public Component
{
public:
	PlanetComp(SceneObject *_obj);
	
	//Returns the name of this component type
	std::string name() { return std::string("Planet"); }
	
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


	double getMass() { return m_Mass; }
	
	// Query stuff
	double getGravity( double x, double y);
	
private:
	double m_Mass;

};

/// Builder for Trigger component
class Planetcom_maker : public Component_maker
{
public:
	Planetcom_maker() : Component_maker("Planet") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static Planetcom_maker s_RegisteredMaker;
};

/*
class PlanetObj
{
public:
	PlanetObj();
	
	// CALL SET POSITION FIRST
	void setPhysics(double _radius, b2Body *_body, b2World *_world);
	void setPhysics(double _radius, b2World *_world);

	double getMass() { return m_Mass; }
	
	void update();
	
	// Query stuff
	double getGravity( double x, double y);
	
private:
	SceneObject m_Obj;
	RenderComponent m_Rend;
	TransformComponent m_Trans;
	PhysComponent m_Phys;

	double m_Mass;

};
*/