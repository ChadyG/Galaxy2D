/*
   Main.cpp
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
 
#include <Gosu/Gosu.hpp>
#include <MUGE.h>
#include <json/json.h>

#include "TitleState.h"
#include "AdventureState.h"

int main(int argc, char* argv[])
{
	// Let's make the game creation a little more dynamic
	std::string str, str2;

	Json::Value json;
	Json::Reader reader;
	reader.parseFile( Gosu::narrow(Gosu::resourcePrefix() + L"Data/main.json"), json);
	
	// Retrieve window size parameters
	int width = json["WindowSize"].get(0u, 640).asInt();
	int height =json["WindowSize"].get(1u, 480).asInt();
	// Framerate and fullscreen stuff
	double updateInterval = json.get("UpdateInterval", 20.0).asDouble();
	bool fullscreen = json.get("FullScreen", false).asBool();
	
	// Create the game object
    Core win(width, height, fullscreen, updateInterval);
	// Must set context otherwise subsystems will fail
	win.setCurrentContext(&win);

	// Input stuff!
	InputManager* iman = InputManager::getCurrentContext();
	for (int i = 0; i < json["ButtonMaps"].size(); ++i) {
		str = json["ButtonMaps"][i].get("Type", "action").asString();
		if (str == "action") {
			str = json["ButtonMaps"][i].get("Action", "default").asString();
			iman->createAction(str);
			if (json["ButtonMaps"][i].isMember("Key")) {
				//There is only one key to be bound
				str2 = json["ButtonMaps"][i].get("Key", "kbA").asString();
				iman->bindAction(str, InputManager::strToButton(str2));
			}else{
				//There are multiple keys to be bound
				for (int j = 0; j < json["ButtonMaps"][i]["Keys"].size(); ++j) {
					str2 = json["ButtonMaps"][i]["Keys"].get(j,"kbA").asString();
					iman->bindAction(str, InputManager::strToButton(str2));
				}
			}
			continue;
		}
		if (str == "chord") {
			str = json["ButtonMaps"][i].get("Type", "action").asString();
			iman->createChord(str);
			if (json["ButtonMaps"][i].isMember("Key")) {
				//There is only one key to be bound
				str2 = json["ButtonMaps"][i].get("Key", "kbA").asString();
				iman->addChord(str, InputManager::strToButton(str2));
			}else{
				//There are multiple keys to be bound
				for (int j = 0; j < json["ButtonMaps"][i]["Keys"].size(); ++j) {
					str2 = json["ButtonMaps"][i]["Keys"].get(j,"kbA").asString();
					iman->addChord(str, InputManager::strToButton(str2));
				}
			}
			continue;
		}
		if (str == "sequence") {
			int t = json["ButtonMaps"][i].get("Threshold", 1).asInt();
			iman->createSequence(str, t);
			if (json["ButtonMaps"][i].isMember("Key")) {
				//There is only one key to be bound
				str2 = json["ButtonMaps"][i].get("Key", "kbA").asString();
				iman->pushSequence(str, InputManager::strToButton(str2));
			}else{
				//There are multiple keys to be bound
				for (int j = 0; j < json["ButtonMaps"][i]["Keys"].size(); ++j) {
					str2 = json["ButtonMaps"][i]["Keys"].get(j,"kbA").asString();
					iman->pushSequence(str, InputManager::strToButton(str2));
				}
			}
			continue;
		}
	}

	// Command line arguments
	bool mapload = false;
	for (int i=0; i < argc; i++) {
		std::string str(argv[i]);
		if (strcmp(argv[i], "-map ") == 0 && i != argc) {
			AdventureState *state = new AdventureState( Gosu::widen(std::string(argv[i+1])) );
			win.pushState( state );
			mapload = true;
		}
	}

	// Create and add a title state to our game object
	if (!mapload) {
		TitleState *state = new TitleState( Gosu::widen(json.get("StartState", "AdventureState").asString()) );
		win.pushState( state );
	}

	// Start the game!
    win.show();

	// Exit
    return 0;
}
