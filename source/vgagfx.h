#ifndef VGAGFX_H_INCLUDED
#define VGAGFX_H_INCLUDED

#include <vector>

#include "image_base.h"
#include "drawable.h"
#include "rectangle.h"


class VgaGfx
{
public:
	VgaGfx();
	~VgaGfx();

	void drawScreen();

	void setBackground(const ImageBase& image);

	// void drawImage(const Animation& image, int16_t x, int16_t y);
	void draw(const Drawable& image, int16_t x, int16_t y);
	void drawBackground(const Drawable& image, int16_t x, int16_t y);
	// void drawImage(const ImageBase& image, int16_t x, int16_t y);
	// void drawImageTransparent(const ImageBase& image, int16_t targetX, int16_t targetY, uint8_t transparentColor);
	// void drawImageTransparent(const Animation& image, int16_t x, int16_t y);

	void drawText(const char* text, int16_t x, int16_t y);

	void vsync();

	void clear();
	
private:
	char* m_backgroundImage;
	char* m_screenBuffer;
	std::vector<Rectangle> m_undrawnRects;
	std::vector<Rectangle> m_dirtyRects;

	uint8_t m_oldVideoMode;
};


#endif
