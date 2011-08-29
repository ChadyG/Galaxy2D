/*
	PauseMenu.h
	Galaxy2D
 
	Created by Chad on 7/25/2011
	Copyright 2011 BlitThis! studios. All rights reserved.
 */

#include "UI/GUIObjects.h"

#ifndef PAUSEMENU_H
#define PAUSEMENU_H
/*
Menus
	Show player stats
	Show map
	Show objectives
	Minigame loading

Inventory page
	show equippables
	show consumables
Player Status
	show status boosting whatever
	show stats
Map
	a map
	current objectives below map
Minigames
	phone menu sort of dealie
*/

class PauseMenu
{
public:
	PauseMenu();

	void update();

	void draw() const;

	void incState();
	void decState();

	enum State {
		Inventory = 0,
		Map,
		Status,
		Minigames
	};
private:
	bool m_Visible, m_switchRight;
	int m_switchTimer, m_switchTimerMax;

	State m_State;

	UISheet *m_UI;
	UIWindow *m_InvWin, *m_MapWin, *m_StatWin, *m_GameWin, *m_curWin, *m_lastWin;
	UITextArea *m_InvText, *m_MapText, *m_StatText, *m_GameText;
};

inline void PauseMenu::incState()
{
	int i = m_State;
	if( ++i > 3 ) i = 0;
	m_State = static_cast<PauseMenu::State>(i);

	m_switchRight = true;
	m_lastWin = m_curWin;
	switch (m_State) {
	case Inventory:
		m_curWin = m_InvWin;
		break;
	case Map:
		m_curWin = m_MapWin;
		break;
	case Status:
		m_curWin = m_StatWin;
		break;
	case Minigames:
		m_curWin = m_GameWin;
	}
}

inline void PauseMenu::decState()
{
	int i = m_State;
	if( --i < 0 ) i = 3;
	m_State = static_cast<PauseMenu::State>(i);

	m_switchRight = false;
	m_lastWin = m_curWin;
	switch (m_State) {
	case Inventory:
		m_curWin = m_InvWin;
		break;
	case Map:
		m_curWin = m_MapWin;
		break;
	case Status:
		m_curWin = m_StatWin;
		break;
	case Minigames:
		m_curWin = m_GameWin;
	}
}

#endif
