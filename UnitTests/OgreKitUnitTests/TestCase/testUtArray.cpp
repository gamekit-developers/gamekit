#include "StdAfx.h"

#define TEST_CASE_NAME testUtArray

TEST(TEST_CASE_NAME, testSize)
{
	const int count = 1000;
	utArray<int> arr1;
	for (int i = 0; i < count; i++)
		arr1.push_back(i);

	EXPECT_EQ(arr1.size(), count);

	arr1.erase(0);
	EXPECT_EQ(arr1.size(), count-1);
	arr1.pop_back();

	arr1.clear();
	EXPECT_EQ(arr1.size(), 0);	
}

TEST(TEST_CASE_NAME, testCopy)
{
	const int count = 1000;
	utArray<int> arr1;
	for (int i = 0; i < count; i++)
		arr1.push_back(i);

	utArray<int> arr2(arr1);
	EXPECT_EQ(arr2.size(), count);

	for (int i = 0; i < count; i++)
		EXPECT_EQ(arr1[i], arr2[i]);

	utArray<int> arr3;
	arr3 = arr2;

	for (int i = 0; i < count; i++)
		EXPECT_EQ(arr3[i], i);

	arr1.clear();

	arr3 = arr1;
	EXPECT_EQ(arr3.size(), 0);

	utArray<int> arr4;
	utArray<int> arr5(arr4);

	EXPECT_EQ(arr4.size(), arr5.size());
}