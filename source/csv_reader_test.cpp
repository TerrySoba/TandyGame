#include "unit_test.h"
#include "csv_reader.h"
#include "rectangle.h"

TEST(CsvReaderTest)
{
    CsvReader<int> reader("test.csv");
    ASSERT_TRUE(reader.width() == 3);
    ASSERT_TRUE(reader.height() == 4);

    ASSERT_TRUE(reader.get(0,0) == 1);
    ASSERT_TRUE(reader.get(2,0) == 3);
    ASSERT_TRUE(reader.get(2,1) == 6);
    ASSERT_TRUE(reader.get(2,3) == 12);
}

TEST(CsvReaderTest2)
{
    CsvReader<int> reader("test2.csv");
    ASSERT_TRUE(reader.width() == 58);
    ASSERT_TRUE(reader.height() == 47);

    ASSERT_TRUE(reader.get(10,38) == 622);
}

TEST(CsvReaderTest3)
{
    CsvReader<int> reader("..\\images\\level1.csv");
    ASSERT_TRUE(reader.width() == 20);
    ASSERT_TRUE(reader.height() == 12);

    ASSERT_TRUE(reader.get(0,11) == 122);
    ASSERT_TRUE(reader.get(19,11) == 60);
}

TEST(CsvReaderTest4)
{
    CsvReader<int> reader("..\\testdata\\lines.csv");
    ASSERT_TRUE(reader.width() == 4);
    ASSERT_TRUE(reader.height() == 3);
}

uint8_t getCsvEntry(CsvReader<uint8_t>& reader, int x, int y)
{
    return reader.get(x,y);
}

uint8_t getCsvEntrySwapped(CsvReader<uint8_t>& reader, int x, int y)
{
    return reader.get(y,x);
}

typedef uint8_t (*CsvGetterPtr)(CsvReader<uint8_t>& reader, int x, int y);


enum Direction
{
    HORIZONTAL = 1,
    VERTICAL = 2,
};

std::vector<Rectangle> getLines(CsvReader<uint8_t>& reader, Direction direction)
{
    std::vector<Rectangle> rectangles;

    uint8_t groundIndex = 1;

    int width;
    int height;
    CsvGetterPtr get;

    if (direction == HORIZONTAL)
    {
        width = reader.width();
        height = reader.height();
        get = getCsvEntry;
    } else {
        width = reader.height();
        height = reader.width();
        get = getCsvEntrySwapped;
    }

    for (int y = 0; y < height; ++y)
    {
        int lineStart = -1;
        for (int x = 0; x < width; ++x)
        {
            if (get(reader,x,y) == groundIndex)
            {
                if (lineStart == -1) lineStart = x;
            }
            else
            {
                if (lineStart != -1)
                {
                    rectangles.push_back(Rectangle(lineStart, y, x - lineStart, 1));
                    lineStart = -1;
                }
            }
        }
        if (lineStart != -1)
        {
            rectangles.push_back(Rectangle(lineStart, y, width - lineStart, 1));
            lineStart = -1;
        }
    }

    // if vertical swap x and y, width and height
    if (direction == VERTICAL)
    {
        for (int i = 0; i < rectangles.size(); ++i)
        {
            Rectangle& r = rectangles[i];
            rectangles[i] = Rectangle(r.y, r.x, r.height, r.width);
        }
    }

    return rectangles;
}

TEST(LineDetectorTest)
{
    CsvReader<uint8_t> reader("..\\testdata\\lines.csv");

    std::vector<Rectangle> horizontalLines = getLines(reader, HORIZONTAL);
    ASSERT_TRUE(horizontalLines.size() == 3);
    ASSERT_TRUE(horizontalLines.at(0) == Rectangle(0, 1, 2, 1));
    ASSERT_TRUE(horizontalLines.at(1) == Rectangle(3, 1, 1, 1));
    ASSERT_TRUE(horizontalLines.at(2) == Rectangle(3, 2, 1, 1));


    std::vector<Rectangle> verticalLines = getLines(reader, VERTICAL);
    ASSERT_TRUE(verticalLines.size() == 3);
    ASSERT_TRUE(verticalLines.at(0) == Rectangle(0, 1, 1, 1));
    ASSERT_TRUE(verticalLines.at(1) == Rectangle(1, 1, 1, 1));
    ASSERT_TRUE(verticalLines.at(2) == Rectangle(3, 1, 1, 2));

}
