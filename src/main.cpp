/*
   Main.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 11/12/08.
  
 Copyright 2009 BlitThis! studios.
 */
 
#include <Gosu/Gosu.hpp>
#include <MUGE.h>
#include <json/json.h>

#include "TitleState.h"
#include "AdventureState.h"
#include "LevelSelectState.h"

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
	iman->initWith(json["ButtonMaps"]);
	
	// Command line arguments
	bool mapload = false;
	for (int i=0; i < argc; i++) {
		std::string str(argv[i]);
		if (strcmp(argv[i], "-map ") == 0 && i != argc) {
			LevelSelectState *lstate = new LevelSelectState( L"world1" );
			AdventureState *state = new AdventureState( Gosu::widen(std::string(argv[i+1])) );
			//win.pushState( lstate );
			win.pushState( state );
			mapload = true;
		}
	}

	// Create and add a title state to our game object
	if (!mapload) {
		TitleState *state = new TitleState( Gosu::widen(json.get("StartState", "TitleState").asString()) );
		win.pushState( state );
	}

	// Start the game!
    win.show();

	// Exit
    return 0;
}
