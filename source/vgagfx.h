#ifndef VGAGFX_H_INCLUDED
#define VGAGFX_H_INCLUDED

#include <vector>

#include "image_base.h"

struct Rectangle
{
	int x,y;
	int width, height;
};


class VgaGfx
{
public:
	VgaGfx();
	~VgaGfx();

	void drawScreen();

	void setBackground(const ImageBase& image);

	void drawImage(const ImageBase& image, int x, int y);
	void drawImageTransparent(const ImageBase& image, int targetX, int targetY, uint8_t transparentColor);

	void vsync();

	void clear();
private:

	// static char far* getScreenLine(int line);
	// char far* getBackBufferLine(int line);
	// char far* getBackgroundImageLine(int line);

	char* m_backgroundImage;
	char* m_screenBuffer;
	std::vector<Rectangle> m_undrawnRects;
	std::vector<Rectangle> m_dirtyRects;
};


#endif
