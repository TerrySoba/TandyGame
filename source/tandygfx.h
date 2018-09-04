#ifndef TANDYGFX_H_INCLUDED
#define TANDYGFX_H_INCLUDED

#include <vector>

#include "image.h"

struct Rectangle
{
	int x,y;
	int width, height;
};

class TandyGfx
{
public:
	TandyGfx();
	~TandyGfx();

	void drawScreen();

	void drawImage(const Image& image, int x, int y);

private:
	char* m_screenBuffer;
	std::vector<Rectangle> m_dirtyRects;
};


#endif
