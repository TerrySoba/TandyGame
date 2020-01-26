#include "unit_test.h"
#include "vector.h"

TEST(VectorTest1)
{
    tnd::vector<int> vec1;
    vec1.push_back(3);
    vec1.push_back(4);
    vec1.push_back(7);
    ASSERT_TRUE(vec1.size() == 3);

    tnd::vector<int> vec2(vec1);
    ASSERT_TRUE(vec1[0] == vec2[0]);
    ASSERT_TRUE(vec1[1] == vec2[1]);
    ASSERT_TRUE(vec1[2] == vec2[2]);
    ASSERT_TRUE(vec2.size() == 3);

    tnd::vector<int> vec3;
    vec3 = vec1;
    ASSERT_TRUE(vec1[0] == vec3[0]);
    ASSERT_TRUE(vec1[1] == vec3[1]);
    ASSERT_TRUE(vec1[2] == vec3[2]);
    ASSERT_TRUE(vec3.size() == 3);
}
