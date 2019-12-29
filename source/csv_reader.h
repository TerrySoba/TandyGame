#ifndef CSV_READER_INCLUDED
#define CSV_READER_INCLUDED

#include <vector>

class CsvReader
{
public:
    CsvReader(const char* path);

    int get(int x, int y) { return m_data[x + y * m_width]; }
    int width() { return m_width; }
    int height() { return m_height; }

private:
    int m_height, m_width;
    std::vector<int> m_data;
};

#endif