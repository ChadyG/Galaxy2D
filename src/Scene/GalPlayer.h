/*
	Player.h
	My Unnamed Game Engine
 
	Created by Chad on 11/12/08.
	Copyright 2009 BlitThis! studios. All rights reserved.

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
#ifndef GALPLAYER_H
#define GALPLAYER_H

#include <Gosu/Gosu.hpp>
#include <Box2D/Box2D.h>
#include <map>
#include <list>
#include <MUGE.h>
 
class SpriteSheet;
class Sprite;
class RenderManager;
class Weapon;
class MachineGun;
class Cannon;
class MessageBubble;

/**
* Our Player class
* to detect if the player is on the ground (jumping)
* we will attach two shapes to the physics body, one for
* the torso and one for the feet.  We can then check the
* contact points on the feet and see if the player is 
* on the ground or touching a wall etc. by examining the
* collision normal.
* 
*/
//TODO: create a separate player object as a data model
class PlayerData
{
	bool m_touching;
	bool m_onGround;
	int m_jumpTimer;
	bool m_moveJump;
	
	float m_SpeedSum;
	int m_TickAvg;
	
	float m_Mass;
	float m_CurSpeed;	//current Speed
	b2Vec2 m_Velocity;
	float m_Speed;		// max walk speed
	float m_RunSpeed;	// max run speed
	float m_MaxSpeed;	// speed modifier limit 
	float m_RunAccel;  // power-up mod
	float m_JumpMove; // power-up mod
	float m_JumpAccel;  // power-up mod (?)
	float m_MaxJumpAccel; // power-up mod
	float m_MaxJump;  // power-up mod
	float m_MaxSpinVel;
	float m_SpinAccel;  // power-up mod
	float m_JumpOffVel;  // current jump velocity from run speed
	float m_FlipAccel;  // power-up mod
	int m_JumpTotTime;  // total time for jumping
	int m_numContacts;
	int m_numGroundContacts;
	
	b2Body *m_Body;
	b2World *m_World;
	b2Vec2 m_Gravity;
	b2Vec2 m_GravityNormal;
	float m_GravMag;
	double m_Ang;
	bool m_FaceRight;
	b2Vec2 m_Dir;
};

class GalPlayer : public Player
{
public:
	GalPlayer();

	enum character {
		char_bill = 0,
		char_bob
	};

	void init(){init(char_bill);}
	void init(character _char);

	void swapChar(character _char);
	
	void setLayer( Gosu::ZPos _z );

	/// Does not check if state exists
	void setAnimState( std::string _state ) { m_AnimState = m_Anims[_state]; }

	void clearAnimations() { m_Anims.clear(); m_AnimState = NULL; }
	
	void setPhysics( double _x, double _y, b2World* _world);
	b2Vec2 getPosition() { return m_Position; }
	
	void setGravity( b2Vec2 _gravity );
	b2Vec2 getGravity() { return m_Gravity; }

	float getSpeed() { return m_CurSpeed; }
	b2Vec2 getVelocity() { return m_Velocity; }
	
	void onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);
	void onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);
	void onMessage(std::string _message);
	
	void update(){}
	void update(bool _focus = true);

	void encodeWith(Json::Value *_val);
	void initWith(Json::Value _val);
	
	friend class Weapon;
	friend class MachineGun;
	friend class Cannon;
protected:

	character m_Character;
	b2Vec2 m_Position;

	void findGround();

	//Message Bubble
	MessageBubble *m_Message;

	//Image Data
	std::map< std::string, SpriteSheet*> m_Anims;
	SpriteSheet* m_AnimState;
	Gosu::ZPos m_Layer;

	//inventory
	std::list<Weapon*> m_Weapons;
	Weapon* m_EquippedWeapon, *m_SecondaryWeapon;


	//PlayerData m_Data;
	//Start data
	bool m_touching;
	bool m_onGround;
	int m_jumpTimer;
	bool m_moveJump;

	float m_SpeedSum;
	int m_TickAvg;

	float m_Mass;
	float m_CurSpeed;	//current Speed
	b2Vec2 m_Velocity;
	float m_Speed;		// max walk speed
	float m_RunSpeed;	// max run speed
	float m_MaxSpeed;	// speed modifier limit
	float m_RunAccel;
	float m_JumpMove; // power-up mod
	float m_JumpAccel;
	float m_MaxJumpAccel; // power-up mod
	float m_MaxJump;
	float m_MaxSpinVel;
	float m_SpinAccel;
	float m_JumpOffVel;
	float m_FlipAccel; 
	int m_JumpTotTime;
	int m_numContacts;
	int m_numGroundContacts;
	
	b2Body *m_Body;
	b2World *m_World;
	b2Vec2 m_Gravity;
	b2Vec2 m_GravityNormal;
	float m_GravMag;
	double m_Ang;
	bool m_FaceRight;
	b2Vec2 m_Dir;
	//End Data
	
};

#endif