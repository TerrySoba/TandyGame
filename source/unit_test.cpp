#include "unit_test.h"

#include <iostream>

std::map<std::string, TestFunctionPtr>& getTests()
{
    static std::map<std::string, TestFunctionPtr> tests;
    return tests;
}

// std::map<std::string, TestFunctionPtr>* s_tests;
TestResult s_currentTestResult;

void runTests(std::string junitFilename)
{
    FILE* junit = NULL;
    if(!junitFilename.empty())
    {
       junit = fopen(junitFilename.c_str(), "wb");
    }

    if (junit) fprintf(junit, "<?xml version=\"1.0\" ?>\n<testsuites>\n<testsuite tests=\"%d\">\n", getTests().size());

    std::cout << "Running " << getTests().size() << " tests." << std::endl;
    int i = 1;
    int failureCount = 0;
    for (std::map<std::string, TestFunctionPtr>::iterator it = getTests().begin(); it != getTests().end(); ++it)
    {
        std::string name = (it->first);
        std::cout << "[ RUN       ] " << name.c_str() << " (" << i++ << " of " << getTests().size() << ")" << std::endl;
        s_currentTestResult = TEST_SUCCESS;
        it->second();
        if (s_currentTestResult != TEST_SUCCESS)
        {
            ++failureCount;
        }
        std::cout << "[      " << ((s_currentTestResult == TEST_SUCCESS)?"  OK":"FAIL") << " ] " << name.c_str() << std::endl;
        if (junit) fprintf(junit, "<testcase classname=\"test\" name=\"%s\">\n", name.c_str());
        if (junit && s_currentTestResult != TEST_SUCCESS)
        {
            fprintf(junit, "<failure type=\"AssertionFailure\">See build log for details.</failure>\n");
        }
        if (junit) fprintf(junit, "</testcase>\n");
    }

    std::cout << "Failed tests: " << failureCount << std::endl;

    if (junit) fprintf(junit, "</testsuite>\n</testsuites>\n");

    if (junit) fclose(junit);
}
