/*
   MainMenuState.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 2/14/08.
  
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
 
#include "MainMenuState.h"
#include "AdventureState.h"
#include "Core/Core.h"

/**
*
*/

MainMenuState::MainMenuState( std::wstring _config )
{

}

void MainMenuState::init()
{	
	m_Engine = Core::getCurrentContext();

	m_Font.reset( new Gosu::Font(m_Engine->graphics(), Gosu::defaultFontName(), 20));

	std::wstring filename = Gosu::resourcePrefix() + L"Images/Menu/menu_planets.png";
	m_MenuScreen.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	
	//Sounds!
	filename = Gosu::resourcePrefix() + L"Sound/cursor_move.wav";
	m_CursorMove.reset(new Gosu::Sample(m_Engine->audio(), filename));
	filename = Gosu::resourcePrefix() + L"Sound/takin_drugz.wav";
	m_CursorSelect.reset(new Gosu::Sample(m_Engine->audio(), filename));
	filename = Gosu::resourcePrefix() + L"Sound/music.ogg";
	m_Music.reset(new Gosu::Song(m_Engine->audio(), filename));

	m_State = eMenu;
	m_Held = false;
	m_msLeftHeld = false;
	m_CursorPos = 0;
	m_lastCursorPos = 0;
	m_units = 10.0;
	m_width = m_Engine->graphics().width();
	m_height = m_Engine->graphics().height();
	/*m_inStart = false;
	m_inExit = false;
	m_ringCounter = 0;
	m_ringCounter2 = 0;*/
	m_Engine->showCursor( true );
}

void MainMenuState::cleanup()
{
	m_MenuScreen.reset(0);
	m_Engine->showCursor( false );
}

void MainMenuState::pause()
{

}

void MainMenuState::resume()
{

}

void MainMenuState::update()
{
	//keep that music playing!
	if (!m_Music->playing()) {
		m_Music->play();
	}
	
	InputManager* input = InputManager::getCurrentContext();
	m_MouseX = m_Engine->input().mouseX();
	m_MouseY = m_Engine->input().mouseY();

	if (input->query("Menu.MouseSelect") == InputManager::actnBegin) {
		m_CursorSelect->play();
		if (m_CursorPos == 0) {
			m_Engine->pushState( new AdventureState( std::wstring(L"galaxy_mario2") ) );
		}
		if (m_CursorPos == 1) {
			m_State = eScores;
		}
		if (m_CursorPos == 2) m_State = eAbout;
		if (m_CursorPos == 3) m_Engine->close();
	}
	//change option to select
	if (input->query("Menu.Down") == InputManager::actnBegin) {
		m_CursorMove->play();
		if (m_CursorPos < 3) m_CursorPos++;
	}
	if (input->query("Menu.Up") == InputManager::actnBegin) {
		m_CursorMove->play();
		if (m_CursorPos > 0) m_CursorPos--;
	}

	//if (m_MouseX 


	/*
	std::list<ring>::iterator riter = m_rings.begin();
	while (riter != m_rings.end()) {
		if (riter->alpha <= 0) {
			riter = m_rings.erase(riter);
			continue;
		}else{
			riter->alpha--;
		}
		riter++;
	}
	m_ringCounter--;
	m_ringCounter2--;
	
	// Mouse stuff
	bool lClick = false;
	m_mousePos.x = (float32)m_Engine->input().mouseX();
	m_mousePos.y = (float32)m_Engine->input().mouseY();
	if (Gosu::distance(m_mousePos.x, m_mousePos.y, 520, 367) < 64) {
		m_CursorPos = 0;
		if (!m_inStart && m_ringCounter <= 0) {
			ring nring;
			nring.alpha = 255;
			nring.radius = 64;
			nring.x = 520;
			nring.y = 367;
			m_rings.push_back(nring);
			m_ringCounter = m_Engine->getUpdateRate()*2;
		}
		m_inStart = true;
		if (input->query("Menu.MouseSelect") == InputManager::actnBegin) {
			m_Engine->pushState( new AdventureState( std::wstring(L"galaxy_mario2") ) );
		}
	}else
		m_inStart = false;

	if (Gosu::distance(m_mousePos.x, m_mousePos.y, 130, 350) < 64) {
		m_CursorPos = 1;
		if (!m_inExit && m_ringCounter2 <= 0) {
			ring nring;
			nring.alpha = 255;
			nring.radius = 32;
			nring.x = 130;
			nring.y = 350;
			m_rings.push_back(nring);
			m_ringCounter2 = m_Engine->getUpdateRate()*2;
		}
		m_inExit = true;
		if (input->query("Menu.MouseSelect") == InputManager::actnBegin) {
			m_Engine->popState();
		}
	}else
		m_inExit = false;
	
	if (input->query("Menu.Select") == InputManager::actnBegin) {
		lClick = true;
	}
	*/
	// Selection sounds
	if (m_lastCursorPos != m_CursorPos) {
		m_CursorMove->play();
		m_lastCursorPos = m_CursorPos;
	}
}

void MainMenuState::draw() const
{
	m_MenuScreen->draw(0,0,0);
	/*
	std::list<ring>::const_iterator riter;
	for (riter = m_rings.begin(); riter != m_rings.end(); riter++) {
		int radius = (255 - riter->alpha)/2;
		for (int a = 0; a < 36; a++) {
			Gosu::Color fgr2(Gosu::clamp(riter->alpha - 100, 0, 255), 0, 255, 0);
			Gosu::Color fgr(riter->alpha, 0, 255, 0);
			m_Engine->graphics().drawQuad(
				riter->x + Gosu::offsetX(a*10, riter->radius + radius*1.2f), riter->y + Gosu::offsetY(a*10, riter->radius + radius*1.2f), fgr2,
				riter->x + Gosu::offsetX((a+1)*10, riter->radius + radius*1.2f), riter->y + Gosu::offsetY((a+1)*10, riter->radius + radius*1.2f), fgr2,
				riter->x + Gosu::offsetX((a+1)*10, riter->radius + radius), riter->y + Gosu::offsetY((a+1)*10, riter->radius + radius), fgr,
				riter->x + Gosu::offsetX(a*10, riter->radius + radius), riter->y + Gosu::offsetY(a*10, riter->radius + radius), fgr, 2);
			m_Engine->graphics().drawQuad(
				riter->x + Gosu::offsetX(a*10, riter->radius + radius), riter->y + Gosu::offsetY(a*10, riter->radius + radius), fgr,
				riter->x + Gosu::offsetX((a+1)*10, riter->radius + radius), riter->y + Gosu::offsetY((a+1)*10, riter->radius + radius), fgr,
				riter->x + Gosu::offsetX((a+1)*10, riter->radius + radius*0.8f), riter->y + Gosu::offsetY((a+1)*10, riter->radius + radius*0.8f), fgr2,
				riter->x + Gosu::offsetX(a*10, riter->radius + radius*0.8f), riter->y + Gosu::offsetY(a*10, riter->radius + radius*0.8f), fgr2, 2);
		}
	}
	*/
}
