/*
   TitleState.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 11/12/08.
  
 Copyright 2009 BlitThis! studios.
 */
 
#include "TitleState.h"
#include "MainMenuState.h"
#include <MUGE.h>
#include <json/json.h>

/**
*
*/

TitleState::TitleState( std::wstring _config )
{
	m_ConfigFile = _config;
}

void TitleState::init()
{
	m_Engine = Core::getCurrentContext();

	Json::Value json;
	Json::Reader reader;
	reader.parseFile( Gosu::narrow(Gosu::resourcePrefix() + L"Data/" + m_ConfigFile + L".json"), json);

	std::string tString;
	std::wstring filename;
	
	//OVERKILL!
	for (int i = 0; i < json["Screens"].size(); ++i) {
		tString = json["Screens"][i].get("Image", "default.png").asString();
		filename = Gosu::resourcePrefix() + L"Images/" + Gosu::widen(tString);
		screen ts;
		ts.image = new Gosu::Image(m_Engine->graphics(), filename, false);
		ts.duration = (int)json["Screens"][i].get("Duration", 2.0).asDouble() * 50;
		ts.decayTime = (int)json["Screens"][i].get("DecayTime", 2.0).asDouble() * 50;
		ts.fadeTo = Gosu::Color(json["Screens"][i]["FadeTo"].get(0u, 0).asInt(),
			json["Screens"][i]["FadeTo"].get(1u, 0).asInt(),
			json["Screens"][i]["FadeTo"].get(2u, 0).asInt());

		m_Screens.push_back( ts );
	}
	
	m_curScreen = m_Screens.begin();
	counter = m_curScreen->duration;
	fade = false;
}

void TitleState::cleanup()
{
	m_Screens.clear();
}

void TitleState::pause()
{

}

void TitleState::resume()
{

}

void TitleState::update()
{
	if (counter <= 0 || m_Engine->input().down(Gosu::kbReturn)) {
		if (fade) {
			m_curScreen++;
			if (m_curScreen == m_Screens.end()) {
				MainMenuState *state = new MainMenuState( std::wstring(L"MainMenuState") );
				m_Engine->changeState( state );
				return;
			}
			counter = m_curScreen->duration;
			fade = false;
		}else{
			counter = m_curScreen->decayTime;
			fade = true;
		}
	}
	--counter;
}

void TitleState::draw() const
{
	if (m_curScreen != m_Screens.end()) {
		if (fade) {
			m_curScreen->image->draw(0,0,0, 1,1, 
				Gosu::interpolate(m_curScreen->fadeTo, Gosu::Colors::white, ((double)counter)/m_curScreen->decayTime));
		}else
			m_curScreen->image->draw(0,0,0, 1,1);
	}
}
