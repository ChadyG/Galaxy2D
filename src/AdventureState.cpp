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
	
	m_Engine->getData()["test2"] = Json::Value(true);
	
	Json::StyledWriter writer;
	writer.writeFile( Gosu::narrow(Gosu::resourcePrefix() + L"Data/adventuretest.json"), m_Engine->getData() );

	//Physics stuff
	m_TimeStep = 1.0f / 50.0f;
	m_VelIterations = 10;
	m_PosIterations = 10;
	
	//Render settings
	m_Width = m_Engine->graphics().width();
	m_Height = m_Engine->graphics().height();
	m_Zoom = 1.0;
	m_Rot = 0.0;
	m_Orientation = 0.0;
	
	//Meh
	m_World.reset();
}

void AdventureState::init()
{	
	m_Engine = Core::getCurrentContext();
	m_Engine->showCursor( true );

	//
	//Register camera to managers
	m_rendMan.setCamera( &m_Camera );
	m_audMan.setCamera( &m_Camera );
	InputManager::getCurrentContext()->setCamera( &m_Camera );

	SceneGraph::setCurrentContext( &m_Graph );
	RenderManager::setCurrentContext( &m_rendMan ); 
	AudioManager::setCurrentContext( &m_audMan );
	
	// box2D stuff
	b2Vec2 gravity( 0.0f, 0.0f);
	bool do_sleep = true;

	m_World.reset( new b2World(gravity, do_sleep));
	m_World->SetContactListener( &m_ContactListener );

	m_Graph.setPhysics( m_World.get() );

	m_VelIterations = 10;
	m_PosIterations = 10;

	
	//
	// Read in JSON encoded file
	//FIXME: using level file here

	Json::Value jVal;
	Json::Reader reader;
	reader.parseFile( Gosu::narrow(Gosu::resourcePrefix() + L"Data/" + m_ConfigFile + L".json"), jVal);
	
	// Player spawn
	m_PlayerPos.Set(
		(float32)jVal["PlayerSpawn"].get(0u, 0.0).asDouble(), 
		(float32)jVal["PlayerSpawn"].get(1u, 0.0).asDouble());
	

	//
	//=====PlayerInit
	m_Player = new GalPlayer();
	m_Player->init(GalPlayer::char_bill);
	m_Player->setPhysics( m_PlayerPos.x, m_PlayerPos.y, m_World.get());
	m_Player->setLayer( 0 );
	//=====
	//


	// Background color
	m_canvasColor = Gosu::Color( 
		jVal["CanvasColor"].get(0u, 255).asInt(),
		jVal["CanvasColor"].get(1u, 255).asInt(),
		jVal["CanvasColor"].get(2u, 255).asInt(),
		jVal["CanvasColor"].get(3u, 255).asInt());
	
	// Coordinate transform stuff for world -> screen
	m_Scale = jVal.get("Scale", 1).asInt();
	
	// Configure camera for screen transformation
	m_Camera.setExtents( m_Width, m_Height);
	m_Camera.setScale( m_Scale );

	// Configure camera stuff
	// Set up layer scales
	int i;
	for (i = 0; i < jVal["Layers"].size(); ++i) {
		m_Camera.addLayer(jVal["Layers"][i]["Layer"].asInt(), jVal["Layers"][i]["Scale"].asDouble());
	}
	m_Focus[0] = m_PlayerPos.x;
	m_Focus[1] = m_PlayerPos.y - 4.0;
	
	m_rendMan.setCamera( m_Focus[0], m_Focus[1], m_Zoom, m_Rot);
	
	// Set screen offset from world focus point
	CameraTransform camtrans = m_Camera.worldToScreen(m_Focus[0], m_Focus[1], 1);
	m_Offset[0] = camtrans.x;
	m_Offset[1] = camtrans.y;
	
	// Create all scene objects from file
	m_Graph.loadFile( m_ConfigFile );
	
	//Pass off song creation/management to manager
	//  this way anyone can pause the music if needed
	m_audMan.createSong(
		Gosu::resourcePrefix() + L"Sound/" + Gosu::widen( jVal.get("Music", "song.mp3").asString()), 
		"Background");
	m_audMan.playSong("Background", true);
}

void AdventureState::cleanup()
{
	// here is where level progress saving would occur
	//m_Scene.reset(0);
}

void AdventureState::update()
{
	//
	// Clear current contacts 
	m_ContactListener.clear();
	
	// Step physics simulation
	m_World->Step(m_TimeStep, m_VelIterations, m_PosIterations);
	m_ContactListener.Update();
	
	m_Player->update(true);
	b2Vec2 m_PlayerPos = m_Player->getPosition();

	m_audMan.update();
	m_rendMan.update();
	InputManager* input = InputManager::getCurrentContext();

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
	
	//zoom based on movement speed
	float zoom = 0.5 + (20.0 - m_Player->getSpeed())/20.0;
	m_Zoom += (zoom - m_Zoom)/100.0;
	m_Zoom = Gosu::clamp(m_Zoom, 0.95, 1.25);

	// Focus camera on player
	m_Focus[0] = m_PlayerPos.x;
	m_Focus[1] = m_PlayerPos.y - 2.0;
	
	// Set screen offset from world focus point
	m_Offset[0] = m_Focus[0]*m_Scale*m_Zoom - m_Width/2;
	m_Offset[1] = m_Height - m_Focus[1]*m_Scale*m_Zoom - m_Height/2;

	//shortcut camera parameter setting
	m_rendMan.setCamera( m_Focus[0], m_Focus[1], m_Zoom, m_Rot);

	// Begin Scene object stuff
	
	m_Graph.update();

	//This trigger stuff is old and needs to be wrapped up somewhere in the engine
	//b2AABB space;
	//space.lowerBound = b2Vec2(m_Focus[0] - m_Width/2, m_Focus[1] - m_Height/2);
	//space.upperBound = b2Vec2(m_Focus[0] + m_Width/2, m_Focus[1] + m_Height/2);
	
	// Update scene objects
	// Do callbacks for areas
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
	// Canvas color
	m_Engine->graphics().drawQuad( 0, 0, m_canvasColor, 
		m_Width, 0, m_canvasColor,
		0, m_Height, m_canvasColor,
		m_Width, m_Height, m_canvasColor, -10);
	
	// Render all
	//m_rendMan->setColor(m_colorMod);
	m_rendMan.doRender();
}

void AdventureState::resume()
{
	
}

void AdventureState::pause()
{
	
}