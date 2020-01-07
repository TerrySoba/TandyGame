#ifndef CSV_READER_INCLUDED
#define CSV_READER_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <vector>

#include <exception>

template <class ValueT>
class CsvReader
{
public:
    CsvReader(const char* path)
    {
        FILE* fp = fopen(path, "rb");
        if (!fp)
        {
            throw std::runtime_error("Could not open CSV file.");
        }

        std::vector<char> buf;

        int width = -1;
        int tokenCount = 0;
        while(!feof(fp))
        {
            char ch = getc(fp);
            
            if (ch == ',' || ch == '\n')
            {
                if (buf.size() > 0)
                {
                    buf.push_back(0);

                    int value = atoi(&buf[0]);

                    // printf("value %d: %d\n", buf.size(), value);
                    m_data.push_back(value);
                    ++tokenCount;
                    buf.clear();

                    if (ch == '\n')
                    {
                        if (width == -1) width = tokenCount;
                        else if (width != tokenCount) throw std::runtime_error("Invalid CSV format.");
                        tokenCount = 0;
                    }
                }

            } else if (ch > ' ') {
                buf.push_back(ch);
            }
        }

        m_width = width;
        m_height = m_data.size() / m_width;

    }

    ValueT get(int x, int y) { return m_data[x + y * m_width]; }
    std::vector<ValueT>& data() { return m_data; }
    
    int width() { return m_width; }
    int height() { return m_height; }

private:
    int m_height, m_width;
    std::vector<ValueT> m_data;
};

#endif