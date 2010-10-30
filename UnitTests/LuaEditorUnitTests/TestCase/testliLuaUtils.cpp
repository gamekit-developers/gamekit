//How write a test case
// 1. Copy this file to test{TEST_CASE_NAME}.cpp
// 2. Remove this comments, and replace {TEST_CASE_NAME}, {TEST_NAME} to proper names in the copied file.
//		(ex, MyTestCase, MyTest1 without quotes)
// 3. Write a test code. 
// 4. See here for more info
//		- http://code.google.com/p/googletest/wiki/Primer
//
// - You don't need to modify the CMakeLists.txt, 
//		All h/cpp files will added by automatically in TestCase directory.
// - If you have a test data file, put a data file to TestData directory,
//		that will be copied to build directory.

#include "StdAfx.h"

#define TEST_CASE_NAME testLiLuaUtils

TEST(TEST_CASE_NAME, test)
{
}
