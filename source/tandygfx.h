#ifndef TANDYGFX_H_INCLUDED
#define TANDYGFX_H_INCLUDED

class Image
{
public:
	Image(const char* filename);

	int width() const { return m_width; }
	int height() const { return m_height; }
	char* data() const { return m_data; }

private:
	int m_width, m_height;
	char* m_data;
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
	
};


#endif
