#ifndef UNIT_TEST_H_INCLUDED
#define UNIT_TEST_H_INCLUDED

#include <map>
#include <string>
#include <iostream>
#include <stdio.h>

enum TestResult
{
	TEST_FAILURE = 0,
	TEST_SUCCESS = 1
};

typedef void (*TestFunctionPtr)();

extern TestResult s_currentTestResult;

std::map<std::string, TestFunctionPtr>& getTests();
void runTests(std::string junitFilename = "");

#define xstr(s) str(s)
#define str(s) #s

#define RUN_STATIC(expr, line) \
	class StaticRunner ## line \
	{ \
	public: \
		StaticRunner ## line () { expr; } \
	}; \
	static StaticRunner ## line staticRunner ## line;

#define TEST(name) \
	void test_ ## name (); \
	RUN_STATIC(getTests()[xstr(name)] = &test_ ## name, name); \
	void test_ ## name ()

#define ASSERT_TRUE(expr) \
	if (!(expr)) \
	{ \
		std::cout << "Assert failed: ASSERT_TRUE("  xstr(expr)  ") " << __FILE__ << ":" << __LINE__ << std::endl; \
		s_currentTestResult = TEST_FAILURE; \
		return; \
	}

#define ASSERT_FALSE(expr) \
	if ((expr)) \
	{ \
		std::cout << "Assert failed: ASSERT_FALSE("  xstr(expr)  ") " << __FILE__ << ":" << __LINE__ << std::endl; \
		s_currentTestResult = TEST_FAILURE; \
		return; \
	}

#endif