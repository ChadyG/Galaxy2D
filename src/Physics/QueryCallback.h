/*
	QueryCallback.h
	My Unnamed Game Engine
 
	Created by Chad Godsey on 4/27/10.
	Copyright 2010 BlitThis! studios.
 */
 
#include <Box2D/Box2D.h>
#include <list>

#ifndef QUERYCALLBACK_H
#define QUERYCALLBACK_H


/**
* Used by Box2D to provide physics callbacks
* derived functions are called during CCD and 
* should only be used to buffer contact lists
* as they may be called multiple times for a 
* single collision
*/

class list_QueryCallback : public b2QueryCallback
{
public:
	list_QueryCallback() {}

	bool ReportFixture(b2Fixture* fixture) { if (fixture->GetFilterData().categoryBits & filter) fixtures.push_back(fixture); return true;}
	
	std::list< b2Fixture* > fixtures;
	uint16 filter;
};

#endif