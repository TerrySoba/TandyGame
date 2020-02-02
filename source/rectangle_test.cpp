#include "unit_test.h"
#include "rectangle.h"

TEST(RectangleIntersection)
{
    {
        Rectangle a(0, 0, 100, 100);
        Rectangle b(-100, -100, 150, 120);
        Rectangle inter1 = a.intersection(b);
        Rectangle inter2 = b.intersection(a);
        ASSERT_TRUE(inter1 == Rectangle(0,0,50,20));
        ASSERT_TRUE(inter2 == inter1);
        ASSERT_FALSE(inter1.empty());
    }

    {
        Rectangle a(0, 0, 100, 100);
        Rectangle b(50, 50, 10, 20);
        Rectangle inter1 = a.intersection(b);
        Rectangle inter2 = b.intersection(a);
        ASSERT_TRUE(inter1 == Rectangle(50,50,10,20));
        ASSERT_TRUE(inter2 == inter1);
        ASSERT_FALSE(inter1.empty());
    }

    {
        Rectangle a(1000, 1000, 100, 100);
        Rectangle b(50, 50, 10, 20);
        Rectangle inter1 = a.intersection(b);
        Rectangle inter2 = b.intersection(a);
        ASSERT_TRUE(inter1 == Rectangle(0,0,0,0));
        ASSERT_TRUE(inter2 == inter1);
        ASSERT_TRUE(inter1.empty());
    }
}
