#include "unit_test.h"
#include "shared_ptr.h"


#include <stdio.h>

struct TestThingy
{
    ~TestThingy()
    {
    }

    void doIt() {}
};


TEST(SharedPtrTest1)
{
    shared_ptr<int> i(new int);
    *(i.get()) = 1;
    ASSERT_TRUE(*i == 1);
    ASSERT_TRUE(i.use_count() == 1);
    shared_ptr<int> n(i);
    ASSERT_TRUE(i.get() == n.get());
    ASSERT_TRUE(i.use_count() == 2);
}

TEST(SharedPtrTest2)
{
    shared_ptr<TestThingy> i(new TestThingy);
    shared_ptr<TestThingy> j = i;

    j->doIt();
    i->doIt();
}
