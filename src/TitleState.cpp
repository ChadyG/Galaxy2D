/*
   TitleState.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 11/12/08.
  
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
