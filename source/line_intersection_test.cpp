#include "unit_test.h"


#include "rectangle.h"


Point intersection(
    const Point& line1Start, const Point& line1End,
    const Point& line2Start, const Point& line2End)
{
    Point line1Dir = line1End - line1Start;
    Point line2Dir = line2End - line2Start;

    const Point& a = line1Start;
    const Point& b = line1Dir;
    const Point& c = line2Start;
    const Point& d = line2Dir;


    int16_t t = (a.x * d.y - c.x * d.y - a.y * d.x + c.y * d.x) / (b.y * d.x - b.x * d.y);

    return a + t * b;
}


TEST(LineIntersection)
{
    ASSERT_TRUE(intersection(Point(0,0), Point(1,0),
                             Point(0,0), Point(0,1)) == Point(0,0))

    Point x = intersection(Point(100,0), Point(0,100),
                             Point(0,0), Point(100,100));

    printf("x: %d y: %d\n", x.x, x.y);


    ASSERT_TRUE(intersection(Point(100,0), Point(1,0),
                             Point(0,0), Point(0,1)) == Point(0,0))
}