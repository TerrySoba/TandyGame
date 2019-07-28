#ifndef UNIT_TEST_H_INCLUDED
#define UNIT_TEST_H_INCLUDED

#include <map>
#include <string>
#include <iostream>

typedef int (*TestFunctionPtr)();

extern std::map<std::string, TestFunctionPtr> s_tests;

void runTests();

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
	int test_ ## name (); \
	RUN_STATIC(s_tests[xstr(name)] = &test_ ## name, name); \
	int test_ ## name ()

#define ASSERT_TRUE(expr) \
	if (!(expr)) \
	{ \
		std::cout << "Assert failed: ASSERT_TRUE("  xstr(expr)  ") " << __FILE__ << ":" << __LINE__ << std::endl; \
		return 0; \
	}

#endif