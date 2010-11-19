/*
   GalPlayer.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 7/12/09.
  
 Copyright 2009 BlitThis! studios.

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

#include <MUGE.h>
#include <Box2D.h>
#include "GalPlayer.h"
#include "PlanetObj.h"
#include "../Inventory/Weapon.h"
#include "../Physics/QueryCallback.h"

GalPlayer::GalPlayer()
{
	m_AnimState = NULL;
	//m_shootRate = 5;
	//m_shotTimer = m_shootRate;

	m_JumpTotTime = 25;
	m_jumpTimer = 0;
	m_SpeedSum = 1.f;
	m_TickAvg = 10;

	m_Ang = 0.0;
	m_FaceRight = true;
	m_Dir.Set(1.0f, 0.0f);
	m_Mass = 0.f;
	m_Speed = 6.0f; //Speed to walk
	m_RunSpeed = 8.0f; // Speed to run
	m_MaxSpeed = 8.0f; // Max velocity
	m_RunAccel = 2.0f;//50
	m_JumpAccel = 25.f;//10
	m_MaxJump = 15.f;
	m_MaxSpinVel = 20.f;
	m_SpinAccel = 15.f;
	m_JumpOffVel = 0.f;
	m_FlipAccel = 2.0f;

	m_numContacts = 0;
	m_numGroundContacts = 0;

	m_onGround = false;
	m_touching = false;
	m_moveJump = false;
}

void GalPlayer::setPhysics( double _x, double _y, b2World* _world)
{
	m_Position.x = (float32)_x;
	m_Position.y = (float32)_y;

	m_World = _world;
	
	//This is pretty stupid
	if (m_Character == char_bob) {
		// Head
		b2CircleShape hDef;
		b2FixtureDef hfix;
		hDef.m_radius = 0.5f;
		hDef.m_p = b2Vec2(-1.0f, 0.0f);
		hfix.density = 0.0f;
		hfix.friction = 0.3f;
		hfix.restitution = 0.3f;
		hfix.filter.categoryBits = 0x0010;
		hfix.filter.maskBits = 0x0FF0;
		hfix.shape = &hDef;

		// Body
		b2PolygonShape pDef;
		pDef.SetAsBox(1.3f, 1.0f);
		b2FixtureDef bfix;
		bfix.density = 1.0f;
		bfix.friction = 0.2f;
		bfix.restitution = 0.2f;
		bfix.filter.categoryBits = 0x0020;
		bfix.filter.maskBits = 0x0FF0;
		bfix.shape = &pDef;

		// Foot
		b2PolygonShape ffDef;
		ffDef.SetAsBox(0.3f, 1.0f, b2Vec2(1.15f, 0.0f), 0.0f);
		b2FixtureDef fffix;
		fffix.density = 0.0f;
		fffix.friction = 0.2f;
		fffix.restitution = 0.2f;
		fffix.filter.categoryBits = 0x0020;
		fffix.filter.maskBits = 0x0FFF;
		fffix.shape = &ffDef;

		// Foot (sensor)
		b2PolygonShape fDef;
		fDef.SetAsBox(0.25f, 0.5f, b2Vec2(1.25f, 0.0f), 0.0f);
		b2FixtureDef ffix;
		ffix.filter.categoryBits = 0x0040;
		ffix.filter.maskBits = 0x0FFF;
		ffix.isSensor = true;
		ffix.shape = &fDef;

		// Near (sensor)
		//b2PolygonShape nDef;
		//nDef.SetAsBox(1.5f, 1.5f);
		b2CircleShape cDef;
		cDef.m_radius = 4.5f;
		cDef.m_p = b2Vec2(0.0f, 0.0f);
		b2FixtureDef nfix;
		nfix.filter.categoryBits = 0x0080;
		nfix.filter.maskBits = 0x0FFF;
		nfix.isSensor = true;
		nfix.shape = &cDef;

		b2BodyDef bDef;
		bDef.userData = this;
		bDef.type = b2_dynamicBody;
		bDef.position.Set( (float32)_x, (float32)_y );
		bDef.linearDamping = 0.75f;
		bDef.angularDamping = 1.85f;
		bDef.angle = Gosu::pi/2.0;
		//bDef.fixedRotation = true;
		m_Body = m_World->CreateBody( &bDef );
		m_Body->CreateFixture( &hfix );
		m_Body->CreateFixture( &bfix );
		m_Body->CreateFixture( &ffix );
		m_Body->CreateFixture( &fffix );
	}else{
		// Circle body
		b2CircleShape cbody;
		cbody.m_radius = 1.3f;
		cbody.m_p = b2Vec2(0.0f, 0.0f);

		b2FixtureDef cfix;
		cfix.density = 0.75f;
		cfix.friction = 0.3f;
		cfix.restitution = 0.3f;
		cfix.filter.categoryBits = 0x0010;
		cfix.filter.maskBits = 0x0FFF;
		cfix.filter.groupIndex = 1;
		cfix.shape = &cbody;

		// Outer sensor
		b2CircleShape outer;
		outer.m_radius = 1.5f;
		outer.m_p = b2Vec2(0.0f, 0.0f);

		b2FixtureDef ofix;
		ofix.density = 0.0f;
		ofix.friction = 0.3f;
		ofix.restitution = 0.3f;
		ofix.isSensor = true;
		ofix.filter.categoryBits = 0x0010;
		ofix.filter.maskBits = 0x0FFF;
		ofix.filter.groupIndex = 3;
		ofix.shape = &outer;

		b2BodyDef bDef;
		bDef.userData = this;
		bDef.type = b2_dynamicBody;
		bDef.position.Set( (float32)_x, (float32)_y );
		bDef.linearDamping = 0.5f;
		bDef.angularDamping = 1.85f;
		//bDef.fixedRotation = true;
		m_Body = m_World->CreateBody( &bDef );
		m_Body->CreateFixture( &cfix );
		m_Body->CreateFixture( &ofix );
	}

	m_Mass = m_Body->GetMass();
}

void GalPlayer::init(character _char)
{
	m_Character = _char;
	m_EquippedWeapon = new Cannon();
	m_EquippedWeapon->init(this);
	m_Weapons.push_back(m_EquippedWeapon);

	m_SecondaryWeapon = new MachineGun();
	m_SecondaryWeapon->init(this);
	m_Weapons.push_back(m_SecondaryWeapon);

	RenderManager* rendMan = RenderManager::getCurrentContext();
	if (m_Character == char_bob) {
		m_Speed = 6.0f; //Speed to walk
		m_RunSpeed = 12.0f; // Speed to run
		m_MaxSpeed = 12.0f; // Max velocity
		m_RunAccel = 2.0f;//50
		m_JumpAccel = 35.f;//10
		m_MaxJump = 15.f;

		m_Anims["Walk"] = rendMan->createSpriteSheet( 0,
				Gosu::resourcePrefix() + L"Images/character2.png", 
				24,
				32,
				20);
		m_Anims["WalkLeft"] = rendMan->createSpriteSheet( 0,
				Gosu::resourcePrefix() + L"Images/character_left2.png", 
				24,
				32,
				20);

		m_Message = NULL;//rendMan->createMessage(L"Test Message", m_Position.x, m_Position.y, true);
	}else{
		m_Speed = 6.0f; //Speed to walk
		m_RunSpeed = 10.0f; // Speed to run
		m_MaxSpeed = 10.0f; // Max velocity
		m_RunAccel = 2.0f;//50
		m_JumpAccel = 50.f;//10
		m_MaxJump = 15.f;

		m_Anims["Walk"] = rendMan->createSpriteSheet( 0,
				Gosu::resourcePrefix() + L"Images/character.png", 
				24,
				32,
				20);
		m_Anims["WalkLeft"] = rendMan->createSpriteSheet( 0,
				Gosu::resourcePrefix() + L"Images/character_left.png", 
				24,
				32,
				20);

		m_Message = NULL;//rendMan->createMessage(L"I must help these people, for science!", m_Position.x, m_Position.y, true);
	}
	m_Anims["Walk"]->setVisible(true);
	m_Anims["WalkLeft"]->setVisible(false);
	m_AnimState = m_Anims["Walk"];

	AudioManager::getCurrentContext()->createSample(Gosu::resourcePrefix() + L"Sound/jump.wav", "Jump");
}

void GalPlayer::swapChar(character _char)
{
	m_AnimState->setVisible(false);
	init(_char);
	m_World->DestroyBody(m_Body);
	setPhysics(m_Position.x+0.5, m_Position.y, m_World);
}

void GalPlayer::setLayer(Gosu::ZPos _z)
{
	m_Layer = _z;
	std::map< std::string, SpriteSheet*>::iterator iAnim;
	for (iAnim = m_Anims.begin(); iAnim != m_Anims.end(); iAnim++) {
		(*iAnim).second->setLayer( m_Layer );
	}
}

void GalPlayer::setGravity( b2Vec2 _gravity )
{
	m_Gravity = _gravity;
	m_Gravity.Normalize();
	m_GravMag = _gravity.Length();
}

void GalPlayer::onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold) 
{
	if (m_Character == char_bob) {
		bool collideFeet = false, collideOther = false;
		if (_fix->GetFilterData().categoryBits == 0x0040) 
				collideFeet = true;
			else
				collideOther = true;

		if (collideFeet) {
			m_onGround = true;
			m_numGroundContacts++;
		}
		if (collideOther) {
			m_touching = true;
			m_numContacts++;
		}
	}else{//GalPlayer is a bill
		m_numContacts++;
		if (_other->hasComponent("Transform")) {
			TransformComponent* tc = (TransformComponent*)_other->getComponent("Transform");
			b2Vec2 d(tc->getPositionX() - m_Position.x,
				tc->getPositionY() - m_Position.y);
			float dot = b2Dot(d, m_Gravity);
			if (dot > 0.5f)
				m_onGround = true;
			else
				m_touching = true;
		}
	}
}

void GalPlayer::onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold) 
{
	if (m_Character == char_bob) {
		bool collideFeet = false, collideOther = false;
		if (_fix->GetFilterData().categoryBits == 0x0040) 
			collideFeet = true;
		else
			collideOther = true;

		if (collideFeet) {
			m_numGroundContacts--;
			if (m_numGroundContacts <= 0) {
				m_onGround = false;
				m_JumpOffVel = 0.f;
				m_numGroundContacts = 0;
			}
		}
		if (collideOther) {
			m_numContacts--;
			if (m_numContacts <= 0) {
				m_touching = false;
				m_numContacts = 0;
			}
		}
	}else{
		m_numContacts--;
		if (m_numContacts <= 0) {
			m_onGround = false;
			m_JumpOffVel = 0.f;
			m_touching = false;
			m_numContacts = 0;
		}
	}
}

void GalPlayer::onMessage(std::string _message)
{

}

void GalPlayer::findGround() 
{
	//query world for surrounding shapes
	list_QueryCallback callback;
	callback.filter = 0x0100;
	b2AABB aabb;
	aabb.lowerBound.Set(m_Position.x-50.0f, m_Position.y-50.0f);
	aabb.upperBound.Set(m_Position.x+50.0f, m_Position.y+50.0f);
	m_World->QueryAABB(&callback, aabb);

	// loop over finding nearest point
	float nearest = 100.f;
	if (!callback.fixtures.empty()) {
		b2Shape *nearPlanet = callback.fixtures.front()->GetShape(), *curPlanet;
		b2CircleShape mshape;
		mshape.m_radius = 1.0f;
		b2Transform mtransform;
		mtransform.Set( m_Position, 0.f);

		std::list< b2Fixture* >::iterator it;
		for (it = callback.fixtures.begin(); it != callback.fixtures.end(); ++it) {
			curPlanet = (*it)->GetShape();
		
			b2DistanceInput input;
			input.proxyA.Set(&mshape);
			input.proxyB.Set(curPlanet);
			input.transformA = mtransform;
			input.transformB = (*it)->GetBody()->GetTransform();
			input.useRadii = true;
			b2SimplexCache cache;
			cache.count = 0;
			b2DistanceOutput output;
			b2Distance(&output, &cache, &input);

			if (output.distance < nearest) {
				nearPlanet = curPlanet;
				nearest = output.distance;
				b2RayCastOutput rback;
				b2RayCastInput rin;
				rin.p1 = output.pointA;
				rin.p2 = output.pointB;
				rin.maxFraction = 2.f;
				nearPlanet->RayCast(&rback, rin, (*it)->GetBody()->GetTransform());
				m_GravityNormal = rback.normal;
				m_Gravity = output.pointB - m_Position;
				m_Gravity.Normalize();
				SceneObject* obj = (SceneObject*)(*it)->GetBody()->GetUserData();
				PlanetComp* plan = (PlanetComp*)(obj->getComponent("Planet"));
				m_Gravity = plan->getMass() * 2.0f * m_Gravity;//((PlanetComp*)((SceneObject*)(*it)->GetBody()->GetUserData())->getComponent("Transform"))->getMass() * 2.0f * m_Gravity;
			}
		}
		if (m_Gravity.Length() < 10.0)
			m_Gravity = (10.0f/m_Gravity.Length()) * m_Gravity;
		if (m_Gravity.Length() > 20.0)
			m_Gravity = (20.0f/m_Gravity.Length()) * m_Gravity;
	}else{
		m_Gravity.SetZero();
	}
	// when body found, cast two rays some arc length apart on either side to find tangent
	//float arcspan = 1.f / nearest;
	// we now have gravity
	// could improve by pulling towards on large distance, and doing tangent on closer
	m_Body->ApplyForce( 4.0f * m_Gravity, m_Position );
	setGravity(0.5f * m_Gravity);
}

void GalPlayer::update(bool _focus)
{
	InputManager* input = InputManager::getCurrentContext();
	m_Position = m_Body->GetWorldCenter();
	findGround();

	b2Vec2 mouse = input->getMouse();
	//==================================================
	// Orientation correction

	// desired angle
	
	float spin = 0.f;
	if (m_Character == char_bob) {
		float ang_desire = std::fmod( std::atan2(m_Gravity.y, m_Gravity.x), (float)Gosu::pi * 2.0f);
		//(Gosu::angle( -m_Gravity.x, -m_Gravity.y, 0.0, 0.0) - 90) * Gosu::pi/180.0;
		//Check angle
	
		float ang = std::fmod(m_Body->GetAngle(), 2.f * (float)Gosu::pi);
		//find difference to upright (-gravity)
		spin = ang_desire - ang;
		if (spin > Gosu::pi) {
			spin = ang_desire - (ang + 2.0f * (float)Gosu::pi);
		}
		if (spin < -Gosu::pi) {
			spin = ang_desire - (ang - 2.0f * (float)Gosu::pi);
		}
		//float angvel = m_Body->GetAngularVelocity();
		//apply torque to correct
		spin = spin * m_SpinAccel;
		spin = Gosu::clamp(spin, -m_MaxSpinVel, m_MaxSpinVel);
		if (m_touching) {
			m_Body->ApplyTorque(spin * m_Mass);
		}
	}
	
	//==================================================
	// Start Walk/Run
	m_Velocity = m_Body->GetLinearVelocity();
	m_CurSpeed = m_Velocity.Length();

	if (input->query("Play.MoveLeft") == InputManager::actnActive && _focus) {
		m_AnimState->setVisible(false);
		m_AnimState = m_Anims["WalkLeft"];
		m_AnimState->setVisible(true);
		m_FaceRight = false;

		if (m_onGround) {
			m_moveJump = true;
			//set desired velocity higher if running
			//find difference to desired
			float push;
			if (input->query("Play.Run") == InputManager::actnActive) 
				push = (m_CurSpeed > m_RunSpeed) ? 0.f : (m_RunSpeed - m_CurSpeed);
			else
				push = (m_CurSpeed > m_Speed) ? 0.f : (m_Speed - m_CurSpeed);
			push = Gosu::clamp(push, -m_MaxSpeed, m_MaxSpeed);
			//ApplyImpulse to move toward desired velocity
			m_Body->ApplyForce( push * m_GravMag * m_Mass * m_RunAccel * b2Vec2(-m_Gravity.y, m_Gravity.x), m_Position );
		}else{
			m_Body->ApplyForce( m_RunAccel * 15.f * b2Vec2(-m_Gravity.y, m_Gravity.x), m_Position );
			if (!m_moveJump && m_Character == char_bob)
				m_Body->ApplyTorque(-m_FlipAccel * m_RunSpeed);
		}
	}

	if (input->query("Play.MoveRight") == InputManager::actnActive && _focus) {
		m_AnimState->setVisible(false);
		m_AnimState = m_Anims["Walk"];
		m_AnimState->setVisible(true);
		m_FaceRight = true;
		
		if (m_onGround) {
			m_moveJump = true;
			//set desired velocity higher if running
			//find difference to desired
			float push;
			if (input->query("Play.Run") == InputManager::actnActive) 
				push = (m_CurSpeed > m_RunSpeed) ? 0.f : (m_RunSpeed - m_CurSpeed);
			else
				push = (m_CurSpeed > m_Speed) ? 0.f : (m_Speed - m_CurSpeed);
			push = Gosu::clamp(push, -m_MaxSpeed, m_MaxSpeed);
			//ApplyImpulse to move toward desired velocity
			m_Body->ApplyForce( push * m_GravMag * m_Mass * m_RunAccel * b2Vec2(m_Gravity.y, -m_Gravity.x), m_Position );
		}else{
			m_Body->ApplyForce( m_RunAccel * 15.f * b2Vec2(m_Gravity.y, -m_Gravity.x), m_Position );
			if (!m_moveJump && m_Character == char_bob)
				m_Body->ApplyTorque(m_FlipAccel * m_RunSpeed);
		}
	}
	if (input->query("Play.MoveRight") == InputManager::actnIdle && 
		input->query("Play.MoveLeft") == InputManager::actnIdle && _focus) {
		m_moveJump = false;
		m_Body->SetAngularVelocity(0.f);
	}

	//Facing direction stuff
	//m_Dir.Set( mouse.x - m_Position.x, mouse.y - m_Position.y);
	//m_Dir.Normalize();
	double dAng = Gosu::clamp(
		Gosu::angle( mouse.x, mouse.y, m_Position.x, m_Position.y),
		10.0, 350.0);
	m_Dir.Set(-Gosu::offsetX(dAng, 1.0),
			  -Gosu::offsetY(dAng, 1.0));
	//if (m_FaceRight) {
	//	m_Dir.Set( m_Gravity.y, -m_Gravity.x);
	//}else{
	//	m_Dir.Set(-m_Gravity.y, m_Gravity.x);
	//}
	//==================================================
	// Start Jump

	if (m_jumpTimer >= 0)
		m_jumpTimer--;
	if (input->query("Play.Jump") == InputManager::actnBegin && _focus) {
		b2Vec2 vel = m_Velocity;
		vel.Normalize();
		if (m_onGround && b2Dot(vel, m_Gravity) > -0.5f) {
			m_JumpOffVel = (std::min)( m_MaxJump, m_Body->GetLinearVelocity().Length());
			float jumpMod = 1.0f + (m_JumpOffVel/m_RunSpeed)/2.0f;
			float imp = (std::min)(m_JumpAccel * jumpMod, 50.f);
			m_Body->ApplyLinearImpulse( imp * b2Vec2(-m_Gravity.x, -m_Gravity.y), m_Position );
			m_jumpTimer = m_JumpTotTime;
			AudioManager::getCurrentContext()->playStereoSample("Jump", m_Position.x, m_Position.y);
		}else
		if (m_touching) {
			m_Body->ApplyLinearImpulse( 4.f * m_Mass  * b2Vec2(-m_Gravity.x, -m_Gravity.y), m_Position );
			m_Body->ApplyTorque(m_Mass * spin);
		}
	}
	if (input->query("Play.Jump") == InputManager::actnActive && m_jumpTimer >= 0 && _focus) {
		m_Body->ApplyForce( m_JumpAccel * b2Vec2(-m_Gravity.x, -m_Gravity.y), m_Position );
	}
	if (input->query("Play.Jump") == InputManager::actnFinish && m_jumpTimer >= 0 && _focus) {
		m_jumpTimer = -1;
	}

	//==================================================
	// Start Weapons

	//if (m_shotTimer >= 0)
	//	m_shotTimer--;
	if (input->query("Play.Swap") == InputManager::actnBegin && _focus) {
		Weapon* weap = m_SecondaryWeapon;
		m_SecondaryWeapon = m_EquippedWeapon;
		m_EquippedWeapon = weap;
	}
	if (input->query("Play.Fire") == InputManager::actnActive && _focus) {
		//shoot
		m_EquippedWeapon->onFire();
	}
	
	if (input->query("Play.SecondaryFire") == InputManager::actnActive && _focus) {
		//shoot the other one!
		m_SecondaryWeapon->onFire();
	}

	//==================================================
	// Start Graphics

	//Set the frame speed of spritesheet based on current speed
	//Rolling average stuff
	m_SpeedSum += m_CurSpeed;
	m_TickAvg--;
	if (m_TickAvg == 0) {
		float sp = m_SpeedSum/10.0f;
		if (sp < 0.5f) {
			m_AnimState->setSpeed( 0.f );
			m_AnimState->setFrame(0);
		}else
		m_AnimState->setSpeed( sp );
		m_SpeedSum = 0;
		m_TickAvg = 10;
	}

	//Update visuals with current position
	m_AnimState->setX( m_Position.x );
	m_AnimState->setY( m_Position.y );
	if (m_Message) {
		m_Message->setX(m_Position.x);
		m_Message->setY(m_Position.y);
	}
	
	/*if (input->query("Play.MoveRight") == InputManager::actnBegin) {
		m_Message->setMessage(L" Right!");
	}
	if (input->query("Play.MoveLeft") == InputManager::actnBegin) {
		m_Message->setMessage( L" Left!");
	}
	if (input->query("Play.Jump") == InputManager::actnBegin) {
		m_Message->setMessage(L" Jump!");
	}
	if (input->query("Play.Fire") == InputManager::actnBegin) {
		m_Message->setMessage(L" Fire!");
	}*/

	//Orient to gravity (circle based physics)
	//float aang = ang * (180.0f / (float)Gosu::pi) - 90.0f;
	if (m_Character == char_bill) {
		double ang =  Gosu::angle( m_Gravity.x, m_Gravity.y, 0.0, 0.0);
		double diff = ang - m_Ang;
		if (diff > 180.0) {
			m_Ang += 360.0;
			diff = ang - m_Ang;
		}
		if (diff < -180.0) {
			m_Ang -= 360.0;
			diff = ang - m_Ang;
		}
		m_Ang += diff/15.0;
		m_AnimState->setAngle( m_Ang );
	}else{
		m_AnimState->setAngle( m_Body->GetAngle() * (180.0f / (float)Gosu::pi) - 90.0f );
	}

	//==================================================
	// Tell weapons to update
	std::list<Weapon*>::iterator wit;
	for (wit = m_Weapons.begin(); wit != m_Weapons.end(); wit++) {
		(*wit)->update();
	}
}

void GalPlayer::encodeWith(Json::Value *_val) 
{
}

void GalPlayer::initWith(Json::Value _val) 
{
}