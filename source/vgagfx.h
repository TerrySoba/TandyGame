#ifndef VGAGFX_H_INCLUDED
#define VGAGFX_H_INCLUDED

#include <vector>

#include "image_base.h"
#include "animation.h"

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

	void drawImage(const Animation& image, int x, int y);
	void drawImage(const ImageBase& image, int x, int y);
	void drawImageTransparent(const ImageBase& image, int targetX, int targetY, uint8_t transparentColor);
	void drawImageTransparent(const Animation& image, int x, int y);

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

	uint8_t m_oldVideoMode;
};


#endif
