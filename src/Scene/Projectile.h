/*
	Projectile.h
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