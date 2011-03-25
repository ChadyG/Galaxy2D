/*
*  LevelSelectState.h
*  Galaxy 2D Platformer
*
*  Created by Chad on 12/9/10.
*  Copyright 2009 BlitThis! All rights reserved.
*
*/

#include "LevelSelectState.h"
#include "AdventureState.h"

LevelSelectState::LevelSelectState( std::wstring _config )
{
	m_ConfigFile = _config;
	m_Engine = Core::getCurrentContext();
	
	m_nodes = NULL;
	m_curNode = NULL;

	m_selectWorld = true;

	//Render settings
	m_Width = m_Engine->graphics().width();
	m_Height = m_Engine->graphics().height();
	m_Zoom = 1.0;
	m_Rot = 0.0;
	m_Orientation = 0.0;
	m_Scale = 10.0;
}

void LevelSelectState::init()
{
	m_Camera.addLayer(0, 1.0);
	m_Camera.addLayer(1, 1.0);
	m_rendMan.setCamera( &m_Camera );
	RenderManager::setCurrentContext( &m_rendMan ); 

	m_Focus[0] = 0.0;
	m_Focus[1] = 0.0;

	m_rendMan.setScreen( m_Width, m_Height, m_Scale );
	m_rendMan.setCamera( m_Focus[0], m_Focus[1], m_Zoom, m_Rot);

	Json::Value jVal;
	Json::Reader reader;
	reader.parseFile( Gosu::narrow(Gosu::resourcePrefix() + L"Data/" + m_ConfigFile + L".json"), jVal);
	
	std::map<std::string, LevelNode*> nodemap;
	int i;
	for (i = 0; i < jVal["Nodes"].size(); ++i) {
		LevelNode *node = new LevelNode;
		node->name = jVal["Nodes"][i].get("Name", "").asString();
		std::wstring file = Gosu::resourcePrefix() + L"Images/" + Gosu::widen( jVal["Nodes"][i].get("Image", "defaultimg.png").asString() );
		node->image = m_rendMan.createSprite( 0, file);
		node->image->setX(jVal["Nodes"][i]["Position"].get(0u, 0.0 ).asDouble());
		node->image->setY(jVal["Nodes"][i]["Position"].get(1u, 0.0 ).asDouble());
		node->condition = jVal["Nodes"][i].get("Condition", "").asString();

		node->up = NULL;
		node->down = NULL;
		node->left = NULL;
		node->right = NULL;

		node->open = false;

		nodemap[node->name] = node;
		if (m_nodes) m_nodes->prev = node;
		node->next = m_nodes;
		node->prev = NULL;
		m_nodes = node;

		if (m_curNode == NULL) m_curNode = node;
	}

	//loop again for connections
	for (i = 0; i < jVal["Nodes"].size(); ++i) {
		LevelNode *node = nodemap[jVal["Nodes"][i].get("Name", "").asString()];
		node->up = nodemap[jVal["Nodes"][i].get("Up", "").asString()];
		node->down = nodemap[jVal["Nodes"][i].get("Down", "").asString()];
		node->left = nodemap[jVal["Nodes"][i].get("Left", "").asString()];
		node->right = nodemap[jVal["Nodes"][i].get("Right", "").asString()];
	}

	m_Engine->getData()["level2.complete"] = false;
	checkFlags();

	//Player image for current position
	m_PlayerAnim = m_rendMan.createSpriteSheet( 1,
		Gosu::resourcePrefix() + L"Images/character.png", 
		24,
		32,
		20);
	m_PlayerAnim->setX(m_curNode->image->posX());
	m_PlayerAnim->setY(m_curNode->image->posY());
}

void LevelSelectState::cleanup()
{
	while (m_nodes) {
		LevelNode *node = m_nodes->next;
		delete m_nodes;
		m_nodes = node;
	}
}

void LevelSelectState::update()
{
	m_rendMan.update();
	InputManager* input = InputManager::getCurrentContext();
	
	if (input->query("Menu.Up") == InputManager::actnBegin && m_curNode->up && m_curNode->up->open) {
		m_curNode = m_curNode->up;
	}
	if (input->query("Menu.Down") == InputManager::actnBegin && m_curNode->down && m_curNode->down->open) {
		m_curNode = m_curNode->down;
	}
	if (input->query("Menu.Left") == InputManager::actnBegin && m_curNode->left && m_curNode->left->open) {
		m_curNode = m_curNode->left;
	}
	if (input->query("Menu.Right") == InputManager::actnBegin && m_curNode->right && m_curNode->right->open) {
		m_curNode = m_curNode->right;
	}

	if (input->query("Menu.Select") == InputManager::actnBegin) {
		//Load level
		AdventureState *state = new AdventureState( Gosu::widen(m_curNode->name) );
		m_Engine->pushState( state );
	}
	
	m_PlayerAnim->setX(m_curNode->image->posX());
	m_PlayerAnim->setY(m_curNode->image->posY());
}

void LevelSelectState::draw() const
{
	m_Engine->graphics().drawQuad( 0, 0, 0xFF6644FF, 
		m_Width, 0, 0xFF6644FF,
		0, m_Height, 0xFF6644FF,
		m_Width, m_Height, 0xFF6644FF, -10);

	m_rendMan.doRender();
}

void LevelSelectState::resume()
{
	
}

void LevelSelectState::pause()
{
	
}

void LevelSelectState::checkFlags()
{
	LevelNode *node = m_nodes;
	while (node) {
		node->open = m_Engine->getData().get(node->condition,true).asBool();
		//Darken unavailable nodes
		//do this via sprite sheets later with desaturated variant
		if (node->open) {
			node->image->setColorMod(Gosu::Color::WHITE);
		}else{
			node->image->setColorMod(0xFFAAAAAA);
		}
		node = node->next;
	}
}