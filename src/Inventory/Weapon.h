/*
	Weapon.h
	Galaxy2D
 
	Created by Chad on 5/14/10.
	Copyright 2010 BlitThis! studios. All rights reserved.
 */
#include <Gosu/Gosu.hpp>
#include <Box2D/Box2D.h>
#include <map>
#include <list>
#include <MUGE.h>
#include "../Scene/GalPlayer.h"
#include "../Scene/Projectile.h"


class Inventory;
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
	friend class Inventory;
	Weapon *m_prev, *m_next;
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