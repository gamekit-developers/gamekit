#include "StdAfx.h"

#define TEST_CASE_NAME testUtHashTable

TEST(TEST_CASE_NAME, testSize)
{
	utHashTable<utIntHashKey, int> table1;

	const int count = 1000;
	for (int i = 0; i < count; i++)
		table1.insert(i, i);

	EXPECT_EQ(table1.size(), count);
}
