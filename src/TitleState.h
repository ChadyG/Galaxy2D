/*
   TitleState.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 11/12/08.
  
 Copyright 2009 BlitThis! studios.
 */
 
#include <list>
#include <Gosu/Gosu.hpp>
#include <Core/GameState.h>

class Core;

/**
* State for title screen and other middleware screens
* goes into main menu
*/
class TitleState : public GameState
{
public:
	TitleState( std::wstring _config );
	
	void init();
	void cleanup();
	
	void pause();
	void resume();
	
	void update();
	void draw() const;
	
private:
	struct screen
	{
		Gosu::Image *image;
		int decayTime, duration;
		Gosu::Color fadeTo;
	};
	std::wstring m_ConfigFile;
	std::list<screen> m_Screens;
	std::list<screen>::iterator m_curScreen;
	int counter;
	bool fade;
};
