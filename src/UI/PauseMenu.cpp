/*
	PauseMenu.h
	Galaxy2D
 
	Created by Chad on 8/1/2011
	Copyright 2011 BlitThis! studios. All rights reserved.
 */
#include <Gosu/Gosu.hpp>
#include <MUGE.h>
#include "PauseMenu.h"

PauseMenu::PauseMenu()
 : m_State(Inventory), m_Visible(false), m_switchTimerMax(20), m_switchTimer(-1)
{
	m_UI = new UISheet(Core::getCurrentContext()->graphics(), Core::getCurrentContext()->input());
	m_UI->giveFocus();

	// Create the movable window container
	windowDef wDef;
	wDef.height = 400;
	wDef.width = 668;
	wDef.tileWidth = 32;
	wDef.tileHeight = 32;
	wDef.canClose = false;
	wDef.canDrag = false;

	m_InvWin = m_UI->createWindow(wDef);
	m_InvWin->giveFocus();
	m_InvWin->hide();
	m_curWin = m_InvWin;

	
	texAreaDef tADef;
	tADef.justify = UITextArea::Center;
	tADef.width = -1;
	tADef.x = 300;
	tADef.y = 30;
	m_InvText = m_InvWin->createTextArea( tADef );
	m_InvText->setText( L"Inventory" );
	

	m_MapWin = m_UI->createWindow(wDef);
	m_MapWin->giveFocus();
	m_MapWin->hide();
	m_lastWin = m_MapWin;
	
	m_MapText = m_MapWin->createTextArea( tADef );
	m_MapText->setText( L"Map" );
	

	m_StatWin = m_UI->createWindow(wDef);
	m_StatWin->giveFocus();
	m_StatWin->hide();
	
	m_StatText = m_StatWin->createTextArea( tADef );
	m_StatText->setText( L"Status" );
	

	m_GameWin = m_UI->createWindow(wDef);
	m_GameWin->giveFocus();
	m_GameWin->hide();
	
	m_GameText = m_GameWin->createTextArea( tADef );
	m_GameText->setText( L"Games" );
}

void PauseMenu::update()
{
	int windowWidth = Core::getCurrentContext()->graphics().width();

	InputManager* input = InputManager::getCurrentContext();
	if (input->query("Play.Pause") == InputManager::actnBegin) {
		if (m_Visible) {
			m_Visible = false;
			m_curWin->hide();
			m_curWin->takeFocus();
		}else{
			m_Visible = true;
			m_curWin->show();
			m_curWin->giveFocus();
		}
	}

	if (m_Visible) {
		if (input->query("Menu.PageLeft") == InputManager::actnBegin && m_switchTimer == -1) {
			m_lastWin->hide();
			decState();
			m_switchTimer = m_switchTimerMax;
			m_curWin->show();
		}
		if (input->query("Menu.PageRight") == InputManager::actnBegin && m_switchTimer == -1) {
			m_lastWin->hide();
			incState();
			m_switchTimer = m_switchTimerMax;
			m_curWin->show();
		}
		
		if (m_switchTimer > 0) {
			int offset = windowWidth - (m_switchTimer/(float)m_switchTimerMax)*windowWidth;
			int scrollfrom = -windowWidth + 50;
			if (m_switchRight) {
				scrollfrom = windowWidth + 50;
				offset = -offset;
			}
			m_lastWin->setPosition(50 + offset, 50);
			m_curWin->setPosition(scrollfrom + offset, 50);

			m_switchTimer--;
		}
		if (m_switchTimer == 0) {
			m_curWin->setPosition(50, 50);
			m_switchTimer = -1;
			m_lastWin->hide();
		}
	}else{
		m_switchTimer = -1;
	}
}

void PauseMenu::draw() const
{
	m_UI->draw(20);
}