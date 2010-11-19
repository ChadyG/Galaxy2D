/*
   AdventureState.h
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

#include <Gosu/Gosu.hpp>
#include <MUGE.h>

//#include "Scene/Scene.h"
#include "Scene/GalPlayer.h"
#include "Scene/PlanetObj.h"
#include "Physics/ContactListener.h"

class Core;

/**
 * State for Adventure gameplay
 * 
 * Must be able to load from a save, store to a save, and start from scratch.
 * If just starting, show opening sequence then load first level.
 * While playing, is able to swap out levels as player progresses.
 * Pause menu control can go here as well.  Allow player to save, quit and likely
 * open up some options to control.
 */
class AdventureState : public GameState
{
public:
	AdventureState( std::wstring _config );
	
	void init();
	void cleanup();
	
	void pause();
	void resume();
	
	void update();
	void draw() const;
	
	void save();
	
private:
	std::wstring m_ConfigFile;
	//Player persists across level loads, so he lives here
	//PlayerData m_Player;
	
	//soon to be defunct
	//boost::scoped_ptr< Scene > m_Scene;

	boost::scoped_ptr<SceneGraph> m_Graph;


	Gosu::Color m_canvasColor;

	Camera_Parallax m_Camera;
	boost::scoped_ptr<RenderManager> m_rendMan;
	boost::scoped_ptr<AudioManager> m_audMan;
	
	// Scene stuff
	GalPlayer *m_Player, *m_CharBill, *m_CharBob;
	b2Vec2 m_PlayerPos;
	double m_Grav;
	//std::list< PlanetObj* > m_Planets;
	std::list< SceneObject* > m_Planets;
	std::list< SceneObject* > m_Objects;
	//std::list< WarpTrigger > m_Warps;
	
	// Physics data
	AdventureListener m_ContactListener;
	boost::scoped_ptr<b2World> m_World;
	float m_TimeStep;
	int m_VelIterations, m_PosIterations;

	//	Pixel transformation data
	// Focus is the level coordinates of the center of the screen
	// Zoom is a zooming factor for all layers
	// Scale is the x/y scale to transform from level coordinates to screen
	// Width and Height are screen size
	double m_Focus[2], m_Offset[2], m_Extents[2];
	double m_Zoom;
	double m_Rot, m_Orientation;
	int m_Scale;
	int m_Width, m_Height;

	//save stuff (sqlite?)
};