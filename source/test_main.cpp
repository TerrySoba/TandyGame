#include "unit_test.h"


TEST(TestName123)
{
	// std::cout << "It works!" << std::endl;
	ASSERT_TRUE(0);
	return true;
}

TEST(TestGnaa)
{
	// std::cout << "It works!" << std::endl;
	return false;
}


int main()
{
   	runTests();
    return 0;
}
