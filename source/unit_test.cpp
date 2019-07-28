#include "unit_test.h"

#include <iostream>

std::map<std::string, TestFunctionPtr> s_tests;

void runTests()
{
    std::cout << "Running tests.\n" << std::endl;
    int i = 1;
    int failureCount = 0;
    for (std::map<std::string, TestFunctionPtr>::iterator it = s_tests.begin(); it != s_tests.end(); ++it)
    {
        std::string name = (it->first);
        std::cout << "Running test \"" << name.c_str() << "\" (" << i++ << " of " << s_tests.size() << ")" << std::endl;
        bool success = it->second();
        if (!success)
        {
            ++failureCount;
        }
        std::cout << ((success)?"SUCCESS":"FAILURE") << std::endl;
    }

    std::cout << "Failed tests: " << failureCount << std::endl;
}
