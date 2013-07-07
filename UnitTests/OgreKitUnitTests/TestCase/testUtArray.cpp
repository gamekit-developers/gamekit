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

TEST(TEST_CASE_NAME, testErase)
{
	const int count = 10;
	utArray<int> arr1;

	for (int i = 0; i < count; i++)
		arr1.push_back(i);

	int k = 5;
	EXPECT_EQ(arr1.find(k), 5);

	arr1.erase(k);
	EXPECT_EQ(arr1.size(), count-1);

	for (UTsize i = 0; i < arr1.size(); i++)
		EXPECT_NE(arr1[i], k);

	EXPECT_EQ(arr1.find(k), UT_NPOS);
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

#if 0
TEST(TEST_CASE_NAME, testIter)
{
	const int count = 10;
	utArray<int> arr1;
	for (int i = 0; i < count; i++)
		arr1.push_back(i);

	int check = 0;
	utArray<int>::Iterator it = arr1.iterator();
	while (it.hasMoreElements())
	{
		utArray<int>::ValueType v = it.getNext();
		if (v % 2 == 0)
			arr1.erase(v);
		EXPECT_TRUE(v == check); //TODO:fix
		//printf("%d\n", v);
		check++;
	}
	EXPECT_EQ(check, count);
}
#endif

bool cmp(const int &a, const int &b)
{
	return a > b;
}

TEST(TEST_CASE_NAME, testSort)
{
	const int count = 10;
	utArray<int> arr1;
	for (int i = count-1; i >= 0; i--)
		arr1.push_back(i);

	arr1.sort(cmp);

	for (int i = 0; i < count; i++)
		EXPECT_EQ(arr1[i], i);
}