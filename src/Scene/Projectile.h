/*
	Projectile.h
	My Unnamed Game Engine
 
	Created by Chad on 5/16/10.
	Copyright 2010 BlitThis! studios. All rights reserved.
 */

#include <Box2D/Box2D.h>
#include "Scene/SceneObject.h"
 
class SpriteSheet;
class Sprite;

/**
* 
*/
class Projectile : public Component
{
public:
	Projectile(bool _hasGravity);
	~Projectile();

	void init(b2Body *_body, b2World *_world);
	
	void setExplode(int _radius, float _power); 
	
	void setRicochet(int _count) { m_ricochet = _count; }
	void setLifetime(int _time) { m_lifetime = _time; }

	bool isValid() { return m_Valid; }

	void update();

	void setSprite(Sprite* _sprite) { m_Sprite = _sprite; }

	/// Physics callback
	void onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);
	void onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);
	bool PreSolve(SceneObject *_other, b2Contact *_contact, const b2Manifold *_manifold) {return true;}

	/// Message passing
	void onMessage(std::string _message);

	void encodeWith(Json::Value *_val);
	void initWith(Json::Value _val);

	std::string name() { return std::string("Projectile"); } 
private:
	void findGround();

	bool m_Valid;
	bool m_hasGravity;
	bool m_explodable;
	float m_Power;
	int m_radius;
	int m_deathTimer;
	int m_lifetime;
	int m_ricochet;
	b2World *m_World;

	Sprite* m_ExplosSprite;
	
	b2Body* m_Body; 
	Sprite* m_Sprite;
	b2Vec2 m_Position;
};