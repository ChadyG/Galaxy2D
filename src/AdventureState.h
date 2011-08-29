/*
   AdventureState.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 3/9/09.
  
 Copyright 2009 BlitThis! studios.
 */

#include <Gosu/Gosu.hpp>
#include <MUGE.h>

//#include "Scene/Scene.h"
#include "Scene/GalPlayer.h"
#include "Scene/PlanetObj.h"
#include "UI/PauseMenu.h"
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

	SceneGraph m_Graph;

	Gosu::Color m_canvasColor;

	Camera_Parallax m_Camera;
	RenderManager m_rendMan;
	AudioManager m_audMan;

	// UI stuff
	PauseMenu m_PauseUI;
	
	// Scene stuff
	GalPlayer *m_Player;
	b2Vec2 m_PlayerPos;
	double m_Grav;
	
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