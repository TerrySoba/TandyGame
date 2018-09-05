#include "image.h"

#include <stdio.h>
#include <string.h>

Image::Image(const char* filename)
{
    FILE* fp = fopen(filename, "rb");

	if (!fp)
	{
		printf("Could not open file \"%s\".\n", filename);
        return;
	}

    fread(&m_width, 2, 1, fp);
    fread(&m_height, 2, 1, fp);

    m_data = new char[m_width * m_height / 2];

    fread(m_data, 1, m_width * m_height / 2, fp);

	fclose(fp);
}

Image::Image(int width, int height, uint8_t value) :
    m_width(width),
    m_height(height)
{
    int s = m_width * m_height / 2;
    m_data = new char[s];
    memset(m_data, value, s);
}

Image::~Image()
{
    delete[] m_data;
}
