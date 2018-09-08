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

	void setBackgroundImage(const Image& image);
	void drawBackground();

	void drawImage(const Image& image, int x, int y);

private:
	char* m_backgroundImage;
	std::vector<Rectangle> m_dirtyRects;
};


#endif
