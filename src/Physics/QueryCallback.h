/*
   QueryCallback.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 4/27/10.
  
  
 Copyright 2010 BlitThis! studios.

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