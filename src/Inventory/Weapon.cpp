/*
	Weapon.h
	Galaxy2D
 
	Created by Chad on 5/14/10.
	Copyright 2010 BlitThis! studios. All rights reserved.
 */
#include "Weapon.h"
#include "../Scene/PlanetObj.h"
#include "../Scene/GalPlayer.h"
#include "../Physics/QueryCallback.h"

Cannon::Cannon() 
{

}

void Cannon::init(GalPlayer* _player)
{
	m_Player = _player;
	m_shootRate = 20;
	m_shotTimer = m_shootRate;
}

void Cannon::onFire()
{
	if (m_shotTimer <= 0) {
		//shoot
		m_shotTimer = m_shootRate;

		b2BodyDef bDef;
		bDef.userData = this;
		bDef.type = b2_dynamicBody;
		bDef.position.Set( m_Player->m_Position.x + 2.0 * m_Player->m_Dir.x, m_Player->m_Position.y + 2.0 * m_Player->m_Dir.y );
		bDef.angle = Gosu::angle(0.0, 0.0, m_Player->m_Dir.x, m_Player->m_Dir.y) * ((float)Gosu::pi / 180.f);

		b2Body *bod = m_Player->m_World->CreateBody( &bDef );

		b2CircleShape buleet;
		buleet.m_radius = 0.25f;

		b2FixtureDef bfix;
		bfix.density = 0.0f;
		bfix.friction = 0.3f;
		bfix.restitution = 0.3f;
		bfix.filter.categoryBits = 0x1000;
		bfix.filter.maskBits = 0xFF0F;
		bfix.shape = &buleet;

		bod->CreateFixture( &bfix );
		
		Projectile *nbull = new Projectile(true);
		nbull->init(bod, m_Player->m_World);
		nbull->setExplode(10, 1000);
		Sprite *sprite = RenderManager::getCurrentContext()->createSprite(0, Gosu::resourcePrefix() + L"Images/Bullet-Bill.png");
		sprite->setX(m_Player->m_Position.x + 2.0 * m_Player->m_Dir.x);
		sprite->setY(m_Player->m_Position.y + 2.0 * m_Player->m_Dir.y);
		nbull->setSprite( sprite);

		bod->ApplyForce(1000.f * m_Player->m_Dir, bod->GetWorldCenter());

		m_Bullets.push_back(nbull);
	}
}

void Cannon::onEquip()
{

}

void Cannon::onUnEquip()
{

}

void Cannon::update()
{
	if (m_shotTimer >= 0)
		m_shotTimer--;

	//update bullets
	std::list<Projectile*>::iterator bit;
	//for (bit = m_Bullets.begin(); bit != m_Bullets.end(); bit++) {
	//	(*bit)->update();
	//}
	//Delete old bullets
	std::list<Projectile*>::iterator sit = m_Bullets.begin();
	while (sit != m_Bullets.end()) {
		if (!(*sit)->isValid()) {
			delete (*sit);
			sit = m_Bullets.erase(sit);
			continue;
		}
		sit++;
	}
}



MachineGun::MachineGun() 
{

}

void MachineGun::init(GalPlayer* _player)
{
	m_Player = _player;
	m_shootRate = 5;
	m_shotTimer = m_shootRate;
}

void MachineGun::onFire()
{
	if (m_shotTimer <= 0) {
		//shoot
		m_shotTimer = m_shootRate;
		//Bullet nbull;
		//nbull.sprite = RenderManager::getCurrentContext()->createSprite(0, Gosu::resourcePrefix() + L"Images/Bullet.png");
		//nbull.sprite->setX(m_Player->m_Position.x + 2.0 * m_Player->m_Dir.x);
		//nbull.sprite->setY(m_Player->m_Position.y + 2.0 * m_Player->m_Dir.y);

		b2CircleShape buleet;
		buleet.m_radius = 0.25f;

		b2FixtureDef bfix;
		bfix.density = 0.0f;
		bfix.friction = 0.3f;
		bfix.restitution = 0.3f;
		bfix.filter.categoryBits = 0x1000;
		bfix.filter.maskBits = 0xFF0F;
		bfix.shape = &buleet;

		b2BodyDef bDef;
		bDef.userData = this;
		bDef.type = b2_dynamicBody;
		bDef.position.Set( m_Player->m_Position.x + 2.0 * m_Player->m_Dir.x, m_Player->m_Position.y + 2.0 * m_Player->m_Dir.y );
		bDef.angle = Gosu::angle(0.0, 0.0, m_Player->m_Dir.x, m_Player->m_Dir.y) * ((float)Gosu::pi / 180.f);

		b2Body *bod = m_Player->m_World->CreateBody( &bDef );
		bod->CreateFixture( &bfix );

		Projectile *nbull = new Projectile(false);
		nbull->init(bod, m_Player->m_World);
		nbull->setRicochet(2);
		nbull->setLifetime(30 * Core::getCurrentContext()->getFPS()); // 30 seconds
		Sprite *sprite = RenderManager::getCurrentContext()->createSprite(0, Gosu::resourcePrefix() + L"Images/Bullet.png");
		sprite->setX(m_Player->m_Position.x + 2.0 * m_Player->m_Dir.x);
		sprite->setY(m_Player->m_Position.y + 2.0 * m_Player->m_Dir.y);
		nbull->setSprite( sprite);

		bod->ApplyForce(800.f * m_Player->m_Dir, bod->GetWorldCenter());

		m_Bullets.push_back(nbull);
	}
}

void MachineGun::onEquip()
{

}

void MachineGun::onUnEquip()
{

}

void MachineGun::update()
{
	if (m_shotTimer >= 0)
		m_shotTimer--;

	/*//update bullets
	std::list<Bullet>::iterator bit;
	for (bit = m_Bullets.begin(); bit != m_Bullets.end(); bit++) {
		b2Vec2 bpos = bit->bod->GetWorldCenter();
		bit->sprite->setX(bpos.x);
		bit->sprite->setY(bpos.y);
		bit->sprite->setAngle( bit->bod->GetAngle() * (180.0f / (float)Gosu::pi) - 90.0f );
	}*/

	
	//update bullets
	std::list<Projectile*>::iterator bit;
	//for (bit = m_Bullets.begin(); bit != m_Bullets.end(); bit++) {
	//	(*bit)->update();
	//}
	//Delete old bullets
	std::list<Projectile*>::iterator sit = m_Bullets.begin();
	while (sit != m_Bullets.end()) {
		if (!(*sit)->isValid()) {
			delete (*sit);
			sit = m_Bullets.erase(sit);
			continue;
		}
		sit++;
	}
}