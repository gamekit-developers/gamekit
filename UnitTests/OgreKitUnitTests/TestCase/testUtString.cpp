#include "StdAfx.h"
#include "utString.h"

#define TEST_CASE_NAME testUtString

TEST(TEST_CASE_NAME, testStringFormat)
{
	utString s = utStringFormat("%d", 10);
	EXPECT_STREQ(s.c_str(), "10");

	const int BUF_SIZE = 1024*10;
	char longString[BUF_SIZE];
	const int STRING_SIZE = BUF_SIZE - 1;

	for (int i = 0; i < STRING_SIZE; i++)
		longString[i] = '0';


	longString[1022] = '9';
	longString[1023] = '1';
	longString[1024] = '1'; //utStringFormat's internal buf end
	longString[1025] = '2';
	longString[1026] = '2';
	
	longString[STRING_SIZE] = 0;

	gkString slong = utStringFormat("%s", longString);

#ifdef _MSC_VER
	longString[1024] = 0;
#else	
	longString[1023] = 0;  //NOTE: gcc vnsprintf's bufsize include null('\0').
#endif
	//printf("%s", slong.c_str());
	EXPECT_STREQ(slong.c_str(), longString);
}

TEST(TEST_CASE_NAME, testStringUtils)
{
	const utString loStr("abc 123");
	const utString upStr("ABC 123");

	EXPECT_TRUE(utStringUtils::lower(upStr) == loStr);
	EXPECT_TRUE(utStringUtils::upper(loStr) == upStr);

	utString nospStr("abc");
	utString lspStr(" \t\rabc");
	utString rspStr("abc \t\r");
	utString lrspStr(" \t\rabc \t\r");

	utStringUtils::trim(lspStr);
	utStringUtils::trim(rspStr);
	utStringUtils::trim(lrspStr);

	EXPECT_TRUE(lspStr == nospStr);
	EXPECT_TRUE(rspStr == nospStr);
	EXPECT_TRUE(lrspStr == nospStr);

	utString src("abc123");
	utString to("abkk23");
	utStringUtils::replace(src, "c1", "kk");
	EXPECT_TRUE(src == to);

	utString spl;
	utStringArray a1, a2;
	for (int i = 'a'; i <= 'z'; i++)
	{
		utString s = utStringFormat("%c", i);
		a1.push_back(s);

		if (i % 4 == 0) s += "\t  ";
		else if (i % 3 == 0) s += "\t";
		else if (i % 2 == 0) s += " ";
		else s += "\r";

		spl += s;
	}

	utStringUtils::split(a2, spl);

	ASSERT_TRUE(a1.size() == a2.size());
	for (size_t i = 0; i < a1.size(); i++)
		EXPECT_TRUE(a1[i] == a2[i]);
}