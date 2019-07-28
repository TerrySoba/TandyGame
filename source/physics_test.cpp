#include "unit_test.h"
#include "physics.h"


TEST(RectCollision)
{
    Rectangle a = {  0,   0, 10, 10};
    Rectangle b = {100, 100, 10, 10};
    Rectangle c = {  9,   9, 10, 10};
    Rectangle d = {  5,   5,  1,  1};
    Rectangle e = { -5,  -5, 10, 10};
    Rectangle f = {-11, -11, 10, 10};

    ASSERT_TRUE(Physics::intersectRect(a,a));
    ASSERT_FALSE(Physics::intersectRect(a,b));
    ASSERT_TRUE(Physics::intersectRect(a,c));
    ASSERT_TRUE(Physics::intersectRect(a,d));
    ASSERT_TRUE(Physics::intersectRect(a,e));
    ASSERT_FALSE(Physics::intersectRect(a,f));
}
