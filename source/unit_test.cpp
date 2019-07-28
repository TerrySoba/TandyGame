#include "unit_test.h"

#include <iostream>

std::map<std::string, TestFunctionPtr>& getTests()
{
    static std::map<std::string, TestFunctionPtr> tests;
    return tests;
}

// std::map<std::string, TestFunctionPtr>* s_tests;
TestResult s_currentTestResult;

void runTests()
{
    std::cout << "Running tests.\n" << std::endl;
    int i = 1;
    int failureCount = 0;
    for (std::map<std::string, TestFunctionPtr>::iterator it = getTests().begin(); it != getTests().end(); ++it)
    {
        std::string name = (it->first);
        std::cout << "Running test \"" << name.c_str() << "\" (" << i++ << " of " << getTests().size() << ")" << std::endl;
        s_currentTestResult = TEST_SUCCESS;
        it->second();
        if (s_currentTestResult != TEST_SUCCESS)
        {
            ++failureCount;
        }
        std::cout << ((s_currentTestResult == TEST_SUCCESS)?"SUCCESS":"FAILURE") << std::endl;
    }

    std::cout << "Failed tests: " << failureCount << std::endl;
}
