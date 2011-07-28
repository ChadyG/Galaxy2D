/*
	PauseMenu.h
	Galaxy2D
 
	Created by Chad on 7/25/2011
	Copyright 2011 BlitThis! studios. All rights reserved.
 */

#include "UI/GUIObjects.h"
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
	PauseMenu() {}

	void update();

	void draw();

	enum State {
		Inventory = 0,
		Map,
		Status,
		Minigames
	};
private:
	bool m_Visible;

	State m_State;

	UISheet *m_UI;
	UIWindow *m_InvWin, *m_MapWin, *m_StatWin, *m_GameWin;

};