/*
 Floating.h
 Mizzou Game Engine
 
 Created by Chad Godsey on 11/23/10.
 
 Copyright 2009-2010 BlitThis!
 
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

#include <Scene/SceneObject.h>


class AISimple : public Component
{
public:
	AISimple(SceneObject *_obj);
	
	//Returns the name of this component type
	std::string name() { return std::string("AISimple"); }
	
	void findGround();
	//logic update callback
	void update();
	
	/// Physics callback
	void onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);
	void onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);
	
	/// Message passing
	void onMessage(std::string _message);
	
	//Serialization/Deserialization
	void encodeWith(Json::Value *_val);
	void initWith(Json::Value _val);
	
private:
	b2Body *m_Body;
	b2World *m_World;
};

/// Builder for Trigger component
class AISimplecom_maker : public Component_maker
{
public:
	AISimplecom_maker() : Component_maker("AISimple") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static AISimplecom_maker s_RegisteredMaker;
};
