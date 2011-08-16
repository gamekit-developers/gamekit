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

TEST(TEST_CASE_NAME, testCapacity)
{
	utHashTable<utIntHashKey, int> table1;

	const int count = 1024;
	table1.reserve(count);
	EXPECT_EQ(table1.capacity(), count);

	for (int i = 0; i < count; i++)
		table1.insert(i, i);

	EXPECT_EQ(table1.size(), count);
	EXPECT_EQ(table1.capacity(), count);

	table1.reserve(count/2);
	EXPECT_EQ(table1.capacity(), count);

	table1.reserve(count*2);
	EXPECT_EQ(table1.capacity(), count*2);
}

TEST(TEST_CASE_NAME, testAssignEmpty)
{
	utHashTable<utIntHashKey, int> table1, table2;

	const int count = 10;
	for (int i = 0; i < count; i++)
		table1.insert(i, i);

	EXPECT_EQ(table1.size(), count);
	EXPECT_EQ(table2.size(), 0);
	table1 = table2;
	EXPECT_EQ(table1.size(), 0);
}

TEST(TEST_CASE_NAME, testNestedType)
{
	typedef utHashTable<utHashedString, utString> tEntry;
	typedef utHashTable<utHashedString, tEntry> tSection;
	typedef utHashTable<utHashedString, tSection> tGroup;

	tEntry entry;
	tSection section;
	tGroup group;

	entry.insert("key1", "value1");
	section.insert("key2", entry);
	group.insert("key3", section);

	EXPECT_TRUE(entry.find("key1") != UT_NPOS);
	EXPECT_TRUE(section.find("key2") != UT_NPOS);
	EXPECT_TRUE(group.find("key3") != UT_NPOS);

	//EXPECT_STREQ(group.get("key3")->get("key2")->get("key1")->c_str(), "value1");
}
