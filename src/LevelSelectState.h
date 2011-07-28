/*
   LevelSelectState.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 12/9/10.
  
 Copyright 2010 BlitThis! studios.
 */

#include <Gosu/Gosu.hpp>
#include <MUGE.h>

struct LevelNode
{
	Sprite *image;
	LevelNode *up, *left, *right, *down, *prev, *next;
	bool open;
	std::string condition, name;
};

/*
* Two levels:
*	World select
*	Level select
*
*/

class LevelSelectState : public GameState
{
public:
	LevelSelectState( std::wstring _config );
	
	void init();
	void cleanup();
	
	void pause();
	void resume();
	
	void update();
	void draw() const;
	
	void save();

	void chooseWorld();
	void chooseLevel();
	
private:

	void checkFlags();

	std::wstring m_ConfigFile;

	LevelNode *m_nodes;
	LevelNode *m_curNode;
	SpriteSheet* m_PlayerAnim;

	Camera_Parallax m_Camera;
	RenderManager m_rendMan;
	InputManager m_inMan;
	
	bool m_selectWorld;

	double m_Focus[2], m_Offset[2], m_Extents[2];
	double m_Zoom;
	double m_Rot, m_Orientation;
	int m_Scale;
	int m_Width, m_Height;
};