#include "unit_test.h"
#include "csv_reader.h"


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
    ASSERT_TRUE(reader.width() == 7);
    ASSERT_TRUE(reader.height() == 17);
}
