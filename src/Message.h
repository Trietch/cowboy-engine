#pragma once

// TODO TODO TODO TODO TODO
// TODO TODO TODO TODO TODO
// TODO TODO TODO TODO TODO
// Create multiple messages class, this one
// is trash af


#include <vector>
#include <iostream>

#ifdef __APPLE__
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

#include "structs.h"

class System;

enum Type
{
	 TEST
	,HELLO
	,HELLO_ACK
	,INIT_GL
	,DRAW
	,ASK_ENTITIES_DRAW
	,RESIZE_GL
	,INIT_DRAWABLE
	,FREE_DRAWABLE
};

class Message
{
public:

	Message(Type type)
		: _type{type}
	{};

	Message(Type type, System * system)
		: _type{type}
		, _system{system}
	{};

	Message(Type type, int width, int height)
		: _type{type}
		, _width{width}
		, _height{height}
	{};

	Message(Type type, std::uint64_t componentID, Shape shape)
		: _type{type}
		, _componentID{componentID}
		, _shape{shape}
	{};


	const Type & type() const;
	System * system() const;
	int width() const;
	int height() const;


	// Ugly but will refactor later anyway 
	const Type _type;
	System * _system = nullptr;
	int _width = 0;
	int _height = 0;
	Shape _shape;
	std::uint64_t _componentID;

private:
};