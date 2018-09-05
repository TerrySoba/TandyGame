#ifndef _IMAGE_H_INC_
#define _IMAGE_H_INC_

#include <stdint.h>

class Image
{
public:
    Image(const char* filename);
    Image(int width, int height, uint8_t value);

    ~Image();

    int width() const { return m_width; }
	int height() const { return m_height; }
	char* data() const { return m_data; }

private:
    int m_width;
    int m_height;
    char* m_data;
};

#endif
