/*
 Floating.h
 Mizzou Game Engine
 
 Created by Chad Godsey on 11/15/10.
 
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


class Floating : public Component
{
public:
	Floating(SceneObject *_obj);
	
	//Returns the name of this component type
	std::string name() { return std::string("Floating"); }
	
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
	std::list< b2Vec2 > m_points;
	std::list< b2Vec2 >::iterator m_curPoint, m_nextPoint;

	bool m_loop, m_forward, m_finished;
	double m_speed, m_ldist;
	b2Body *m_body;
};

/// Builder for Trigger component
class Floatcom_maker : public Component_maker
{
public:
	Floatcom_maker() : Component_maker("Floating") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static Floatcom_maker s_RegisteredMaker;
};
