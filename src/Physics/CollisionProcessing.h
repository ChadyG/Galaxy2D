/*
	CollisionProcessing.h
	My Unnamed Game Engine
 
	Created by Chad Godsey on 5/16/10.
	Copyright 2010 BlitThis! studios.
 */
 
#include <Box2D/Box2D.h>

#ifndef COLLISIONPROCESSING_H
#define COLLISIONPROCESSING_H

class CollisionProcessing
{
public:
	CollisionProcessing() {}

	virtual void onColStart(b2Body *_bod, b2Manifold _man) = 0;
	virtual void onColFinish(b2Body *_bod, b2Manifold _man) = 0;
};

#endif