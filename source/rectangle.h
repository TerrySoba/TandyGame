#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include <stdint.h>

#define _MY_MIN_INT16 -32768

struct Point
{
	Point(int _x, int _y) :
		x(_x), y(_y)
	{}

	int16_t x,y;
};

struct Rectangle
{
	Rectangle() :
		x(0), y(0),
		width(0), height(0)
	{}

	Rectangle(int _x, int _y, int _width, int _height) :
		x(_x), y(_y),
		width(_width), height(_height)
	{}

	bool operator==(const Rectangle& other)
	{
		return
			x == other.x &&
			y == other.y &&
			width == other.width &&
			height == other.height;
			
	}

	Rectangle& operator*=(int16_t factor)
	{
		x *= factor;
		y *= factor;
		width *= factor;
		height *= factor;
		return *this;
	}

	void scale(int16_t horizontalFactor, int16_t verticalFactor)
	{
		x *= horizontalFactor;
		y *= verticalFactor;
		width *= horizontalFactor;
		height *= verticalFactor;
	}

	int16_t x,y;
	int16_t width, height;
};

#endif
