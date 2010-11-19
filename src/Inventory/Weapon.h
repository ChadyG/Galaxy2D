/*
	Weapon.h
	My Unnamed Game Engine
 
	Created by Chad on 5/14/10.
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
#include <Gosu/Gosu.hpp>
#include <Box2D/Box2D.h>
#include <map>
#include <list>
#include <MUGE.h>
#include "../Scene/GalPlayer.h"
#include "../Scene/Projectile.h"


/**
* 
*/
class Weapon
{
public:
	Weapon() {}
	
	virtual void init(GalPlayer* _player) = 0;
	
	virtual void onFire() = 0;
	virtual void onEquip() = 0;
	virtual void onUnEquip() = 0;
	
	virtual void update() = 0;
	
protected:
	GalPlayer* m_Player;
};



struct Bullet 
{
	b2Body* bod;
	Sprite* sprite;
	int deathTimer;
};

class Cannon : public Weapon 
{
public:
	Cannon();
	
	void init(GalPlayer* _player);
	
	void onFire();
	void onEquip();
	void onUnEquip();

	void update();
	
private:

	std::list<Projectile*> m_Bullets;
	int m_shotTimer;
	int m_shootRate;
};

class MachineGun : public Weapon 
{
public:
	MachineGun();
	
	void init(GalPlayer* _player);
	
	void onFire();
	void onEquip();
	void onUnEquip();
	
	void update();
	
private:
	std::list<Projectile*> m_Bullets;
	//std::list<Bullet> m_Bullets;
	int m_shotTimer;
	int m_shootRate;
};