/*
   MainMenuState.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 11/12/08.
  
 Copyright 2009 BlitThis! studios.
 */
 
#include <Gosu/Gosu.hpp>
#include <list>
#include "Core/GameState.h"

class Core;

/**
 * State for main menu
 * central hub to move into other major game states
 *
 * This is a good place to toy around with things, as it is the first
 * piece of the game the player can interact with.
 */
class MainMenuState : public GameState 
{
public:
	MainMenuState( std::wstring _config );
	
	void init();
	void cleanup();
	
	void pause();
	void resume();
	
	void update();
	void draw() const;
	
private:
	boost::scoped_ptr<Gosu::Image> m_MenuScreen;
	boost::scoped_ptr<Gosu::Sample> m_CursorMove;
	boost::scoped_ptr<Gosu::Sample> m_CursorSelect;
	boost::scoped_ptr<Gosu::Song> m_Music;
	boost::scoped_ptr<Gosu::Font> m_Font;
	int m_CursorPos, m_lastCursorPos;
	double m_MouseX, m_MouseY;
	bool m_Held, m_msLeftHeld;
	
	double m_width, m_height, m_units;

	enum eMenuState
	{
		eMenu,
		eAbout,
		eScores,
		eCredits
	};
	/*
	struct ring {
		int alpha;
		int x;
		int y;
		int radius;
	};

	std::list<ring> m_rings;
	
	bool m_inStart, m_inExit;
	int m_ringCounter, m_ringCounter2;
	*/
	eMenuState m_State;
};