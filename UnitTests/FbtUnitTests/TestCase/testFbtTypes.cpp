#include "StdAfx.h"
#include "fbtTypes.h"

#define TEST_CASE_NAME testFbtTypes

TEST(TEST_CASE_NAME, testFbtDebugger)
{
	const int BUF_SIZE = 1024*10;
	char longString[BUF_SIZE];
	const int STRING_SIZE = BUF_SIZE - 1;

	for (int i = 0; i < STRING_SIZE; i++)
		longString[i] = '0';

	longString[1022] = '1';
	longString[1023] = '1'; //debug buf end
	longString[1024] = '2';
	longString[1025] = '2';
	
	longString[STRING_SIZE] = 0;

	fbtDebugger::report("%s", longString); 
}
