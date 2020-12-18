#include "unit_test.h"
#include "tiny_string.h"
#include "vector.h"
#include <string.h>


tnd::vector<int> getSpacePositions(const char* text)
{
    tnd::vector<int> spacePositions;
    const char* ptr = text;
    int pos = 0;
    while (*ptr != '\0')
    {
        if (*ptr == ' ') spacePositions.push_back(pos);
        ++pos;
        ++ptr;
    }

    spacePositions.push_back(pos);
    return spacePositions;
}




tnd::vector<TinyString> formatString(const char* str, int lineWidth)
{
    tnd::vector<int> spaces = getSpacePositions(str);

    tnd::vector<TinyString> out;

    int diff = 0;
    for (int i = 0; i < spaces.size(); ++i)
    {
        if (spaces[i] - diff >= lineWidth)
        {
            TinyString line(lineWidth);
            memcpy(line.data(), &str[diff], spaces[i-1] - diff);
            out.push_back(line);
            // puts(line.c_str());
            diff = spaces[i-1]+1;
        }
    }

    TinyString line(lineWidth);
    memcpy(line.data(), &str[diff], spaces[spaces.size()-1] - diff);
    out.push_back(line);
    // puts(line.c_str());

    return out;
}


TEST(TextLayoutTest1)
{
    const char* testText = "test text is a test text";

    tnd::vector<int> spaces = getSpacePositions(testText);
    ASSERT_TRUE(spaces.size() == 6);
    ASSERT_TRUE(spaces[spaces.size()-1] == strlen(testText));

    ASSERT_TRUE(spaces[0] == 4);
    ASSERT_TRUE(spaces[1] == 9);
    ASSERT_TRUE(spaces[2] == 12);
    ASSERT_TRUE(spaces[3] == 14);
    ASSERT_TRUE(spaces[4] == 19);
    ASSERT_TRUE(spaces[5] == 24);
}

TEST(TextLayoutTest2)
{
    const char* testText = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur gravida bibendum enim sodales ornare.";
    const char* resultText =
    "Lorem   ipsum  dolor  sit  amet,\n"
    "consectetur   adipiscing   elit.\n"
    "Curabitur  gravida bibendum enim\n"
    "sodales                  ornare.";

    tnd::vector<TinyString> f = formatString(testText, 32);

    for (int i = 0; i < f.size(); ++i)
    {
        printf("\"%s\"\n", f[i].c_str());
    }

    // puts(f.c_str());

    // ASSERT_TRUE(f == TinyString(resultText));
}
