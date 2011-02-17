/*
   LevelSelectState.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 12/9/10.
  
 Copyright 2010 BlitThis! studios.

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