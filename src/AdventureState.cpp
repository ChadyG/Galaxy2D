/*
   AdventureState.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 3/9/09.
  
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

#include "AdventureState.h"

//Temp includes
#include <list>
#include <string>

/**
 *
 */

AdventureState::AdventureState( std::wstring _config )
{
	m_ConfigFile = _config;
	m_Engine = Core::getCurrentContext();

	//
	//=====BEGIN SCENE PASTE=====
	m_TimeStep = 1.0f / 50.0f;
	m_VelIterations = 10;
	m_PosIterations = 10;
	
	m_Width = m_Engine->graphics().width();
	m_Height = m_Engine->graphics().height();
	m_Zoom = 1.0;
	m_Rot = 0.0;
	m_Orientation = 0.0;
	
	m_rendMan.reset();
	m_audMan.reset();
	m_World.reset();
	//=====END SCENE PASTE=====
	//
}

//This should take a parameter for new player or existing
void AdventureState::init()
{	
	m_Engine = Core::getCurrentContext();
	m_Engine->showCursor( true );
	
	//m_Scene.reset( new Scene(m_ConfigFile));
	//m_Scene->registerManagers();


	//scenegraph + register
	//rendermanager + register
	//audiomanager + register

	//physics world + register on scenegraph
	
	//m_Scene->initPlayer( );//&m_Player );

	//
	//=====BEGIN SCENE PASTE=====
	m_Graph.reset( new SceneGraph() );
	m_rendMan.reset( new RenderManager() );
	m_audMan.reset( new AudioManager() );
	m_rendMan->setCamera( &m_Camera );

	SceneGraph::setCurrentContext( m_Graph.get() );
	RenderManager::setCurrentContext( m_rendMan.get() ); 
	AudioManager::setCurrentContext( m_audMan.get() );
	
	// box2D stuff
	b2Vec2 gravity( 0.0f, 0.0f);
	bool do_sleep = true;

	m_World.reset( new b2World(gravity, do_sleep));
	m_World->SetContactListener( &m_ContactListener );

	m_Graph->setPhysics( m_World.get() );

	m_VelIterations = 10;
	m_PosIterations = 10;

	
	//
	// Read in JSON encoded file
	
	int i, layer;
	std::string tString;

	Json::Value jVal;
	Json::Reader reader;
	reader.parseFile( Gosu::narrow(Gosu::resourcePrefix() + L"Data/" + m_ConfigFile + L".json"), jVal);
	
	// Player spawn
	m_PlayerPos.Set((float32)jVal["PlayerSpawn"].get(0u, 0.0).asDouble(), (float32)jVal["PlayerSpawn"].get(1u, 0.0).asDouble());
	

	//m_Player.init();
	//
	//=====BEGIN SCENE PASTE===== PlayerInit
	//m_CharBill = new GalPlayer();
	//m_CharBill->init();
	//m_CharBill->setPhysics( m_PlayerPos.x, m_PlayerPos.y, m_World.get());
	//m_CharBill->setLayer( 0 );

	m_CharBob = new GalPlayer();
	m_CharBob->init(GalPlayer::char_bill);
	m_CharBob->setPhysics( m_PlayerPos.x, m_PlayerPos.y, m_World.get());
	m_CharBob->setLayer( 0 );
	
	m_Player = m_CharBob;
	//=====END SCENE PASTE=====
	//


	// Background color
	m_canvasColor = Gosu::Color( 
		jVal["CanvasColor"].get(0u, 255).asInt(),
		jVal["CanvasColor"].get(1u, 255).asInt(),
		jVal["CanvasColor"].get(2u, 255).asInt(),
		jVal["CanvasColor"].get(3u, 255).asInt());
	
	// Coordinate transform stuff for world -> screen
	m_Scale = jVal.get("Scale", 1).asInt();
	
	// Configure render/audio manager for screen transformation
	m_rendMan->setScreen( m_Width, m_Height, m_Scale );
	m_audMan->setScreen( m_Width, m_Height, m_Scale );
	InputManager::getCurrentContext()->setScreen( m_Width, m_Height, m_Scale );

	// Configure camera stuff
	// Set up layer scales
	for (i = 0; i < jVal["Layers"].size(); ++i) {
		m_Camera.addLayer(jVal["Layers"][i]["Layer"].asInt(), jVal["Layers"][i]["Scale"].asDouble());
	}
	m_Focus[0] = m_PlayerPos.x;
	m_Focus[1] = m_PlayerPos.y - 4.0;
	
	m_rendMan->setCamera( m_Focus[0], m_Focus[1], m_Zoom, m_Rot);
	m_audMan->setCamera( m_Focus[0], m_Focus[1], m_Zoom, m_Rot);
	
	// Set screen offset from world focus point
	CameraTransform camtrans = m_Camera.worldToScreen(m_Focus[0], m_Focus[1], 1);
	m_Offset[0] = camtrans.x;
	m_Offset[1] = camtrans.y;
	//m_Offset[0] = m_Focus[0]*m_Scale*m_Zoom - m_Width/2;
	//m_Offset[1] = m_Height - m_Focus[1]*m_Scale*m_Zoom - m_Height/2;

	// Set up warp points
	/*for (i = 0; i < jVal["Warps"].size(); ++i) {
		WarpTrigger twarp;	
	
		twarp.trigger.setExtents( 
			jVal["Warps"][i]["Position"].get(0u, 0.0).asDouble(),
			jVal["Warps"][i]["Position"].get(1u, 0.0).asDouble(),
			jVal["Warps"][i]["Extent"].get(0u, 1.0).asDouble(),
			jVal["Warps"][i]["Extent"].get(1u, 1.0).asDouble());
		twarp.level = jVal.get("Level", "default").asString();
		
		m_Warps.push_back( twarp );
	}*/
	
	// Dig down to the actual scene objects
	m_Graph->loadFile( m_ConfigFile );

	//m_Graph->writeFile( L"loltest" );
	
	//Pass off song creation/management to manager
	//  this way anyone can pause the music if needed
	m_audMan->createSong(
		Gosu::resourcePrefix() + L"Sound/" + Gosu::widen( jVal.get("Music", "song.mp3").asString()), 
		"Background");
	m_audMan->playSong("Background", true);
	//=====END SCENE PASTE=====
	//
}

void AdventureState::cleanup()
{
	// here is where level progress saving would occur
	//m_Scene.reset(0);
}

void AdventureState::update()
{
	//
	// Clear current contacts (IMPORTANT! Box2D sends EndContact on body deletion, I DO NOT WANT)
	m_ContactListener.clear();
	// Step physics simulation
	m_World->Step(m_TimeStep, m_VelIterations, m_PosIterations);
	m_ContactListener.Update();
	
	m_Player->update(true);
	//m_CharBill->update( (m_Player == m_CharBill) );
	//m_CharBob->update( (m_Player == m_CharBob) );
	b2Vec2 m_PlayerPos = m_Player->getPosition();

	m_audMan->doPlay();
	m_rendMan->update();
	InputManager* input = InputManager::getCurrentContext();

/*
	if (input->query("Play.UseMario") == InputManager::actnBegin) {
		//m_Player->swapChar(Player::char_bill);
		m_Player = m_CharBill;
	}
	if (input->query("Play.UseLuigi") == InputManager::actnBegin) {
		//m_Player->swapChar(Player::char_bob);
		m_Player = m_CharBob;
	}
	*/
	// Galaxy Camera stuff
	b2Vec2 gravity = m_Player->getGravity();
	
	m_Grav = -Gosu::angle( gravity.x, gravity.y, 0.0, 0.0);
	double diff = m_Grav - m_Rot;
	if (diff > 180.0) {
		m_Rot += 360.0;
		diff = m_Grav - m_Rot;
	}
	if (diff < -180.0) {
		m_Rot -= 360.0;
		diff = m_Grav - m_Rot;
	}
	m_Rot += diff/40.0;
	// end Galaxy
	
	float zoom = 0.5 + (20.0 - m_Player->getSpeed())/20.0;
	m_Zoom += (zoom - m_Zoom)/100.0;
	m_Zoom = Gosu::clamp(m_Zoom, 0.95, 1.25);

	// We need to know where to draw 
	m_Focus[0] = m_PlayerPos.x;
	m_Focus[1] = m_PlayerPos.y - 2.0;
	
	// Set screen offset from world focus point
	m_Offset[0] = m_Focus[0]*m_Scale*m_Zoom - m_Width/2;
	m_Offset[1] = m_Height - m_Focus[1]*m_Scale*m_Zoom - m_Height/2;

	m_rendMan->setCamera( m_Focus[0], m_Focus[1], m_Zoom, m_Rot);
	m_audMan->setCamera( m_Focus[0], m_Focus[1], m_Zoom, m_Rot);
	input->setCamera( m_Focus[0], m_Focus[1], m_Zoom, m_Rot);
	
	// Begin Scene object stuff

	b2AABB space;
	space.lowerBound = b2Vec2(m_Focus[0] - m_Width/2, m_Focus[1] - m_Height/2);
	space.upperBound = b2Vec2(m_Focus[0] + m_Width/2, m_Focus[1] + m_Height/2);
	
	// Update scene objects
	// Do callbacks for areas
	m_Graph->update();
	/*
	std::map< Gosu::ZPos, SpriteLayer >::iterator itL;
	for (itL = m_Layers.begin(); itL != m_Layers.end(); ++itL) {
		std::list< Trigger >::iterator it;
		for (it = itL->second.triggers.begin(); it != itL->second.triggers.end(); it++) {
			if (it->overlap(space)) {
				if (!it->inCamera()) {
					it->onEnterCamera();
				}
			}else{
				if (it->inCamera()) {
					it->onLeaveCamera();
				}
			}
			
			if (it->pointIn(m_PlayerPos)) {
				if (!it->playerIn()) {
					it->onPlayerEnter();
				}
			}else{
				if (it->playerIn()) {
					it->onPlayerLeave();
				}
			}
		}
		// update sceneobjects
		// re-order by Y value if desired
	}
	*/
	// Warp points
	/*std::list< WarpTrigger >::iterator it;
	for (it = m_Warps.begin(); it != m_Warps.end(); ++it) {
		if (it->trigger.overlap(space)) {
			if (!it->trigger.inCamera()) {
				it->trigger.onEnterCamera();
			}
		}else{
			if (it->trigger.inCamera()) {
				it->trigger.onLeaveCamera();
			}
		}
		
		if (it->trigger.pointIn(m_PlayerPos)) {
			if (!it->trigger.playerIn()) {
				it->trigger.onPlayerEnter();
				//level loading
				//loadFile(Gosu::widen(it->level));
				//initPlayer( );
				break;
			}
		}else{
			if (it->trigger.playerIn()) {
				it->trigger.onPlayerLeave();
			}
		}
	}*/
}

void AdventureState::draw() const
{
	//m_Scene->draw();
	// Canvas color
	m_Engine->graphics().drawQuad( 0, 0, m_canvasColor, 
		m_Width, 0, m_canvasColor,
		0, m_Height, m_canvasColor,
		m_Width, m_Height, m_canvasColor, -10);
	
	// Render all
	m_rendMan->doRender();
}

void AdventureState::resume()
{
	
}

void AdventureState::pause()
{
	
}