#include "StdAfx.h"
#include "liUtils.h"

#define TEST_CASE_NAME testLiUtils

TEST(TEST_CASE_NAME, testFilePath)
{
	const char* path = "c:/test/file1.txt";
	EXPECT_TRUE(getDirName(path) == "c:/test");
	EXPECT_TRUE(getFileName(path) == "file1.txt");	
	EXPECT_TRUE(getFileNameBase(path) == "file1");
	EXPECT_TRUE(getFileNameExt(path) == "txt");

	path =  "c:\\test\\file1.txt";
	EXPECT_TRUE(getFileName(path) == "file1.txt");
	EXPECT_TRUE(getDirName(path) == "c:\\test");
	EXPECT_TRUE(getFileNameBase(path) == "file1");
	EXPECT_TRUE(getFileNameExt(path) == "txt");

	path =  "c:\\dir 123\\sub dir/file1.txt"; //normalize to '/'
	EXPECT_TRUE(getFileName(path) == "file1.txt");
	EXPECT_TRUE(getDirName(path) == "c:/dir 123/sub dir");
	EXPECT_TRUE(getFileNameBase(path) == "file1");
	EXPECT_TRUE(getFileNameExt(path) == "txt");

	path = "/test/file1.txt";
	EXPECT_TRUE(getDirName(path) == "/test");
	EXPECT_TRUE(getFileName(path) == "file1.txt");	
	EXPECT_TRUE(getFileNameBase(path) == "file1");
	EXPECT_TRUE(getFileNameExt(path) == "txt");

	path = "/test/file1";
	EXPECT_TRUE(getDirName(path) == "/test");
	EXPECT_TRUE(getFileName(path) == "file1");	
	EXPECT_TRUE(getFileNameBase(path) == "file1");
	EXPECT_TRUE(getFileNameExt(path) == "");

	path = ".";
	EXPECT_TRUE(getDirName(path) == "."  && getFileName(path) == "" && getFileNameBase(path) == "" && getFileNameExt(path) == "");

	path = "..";
	EXPECT_TRUE(getDirName(path) == ".." && getFileName(path) == ""  && getFileNameBase(path) == "" && getFileNameExt(path) == "");

	path = "";
	EXPECT_TRUE(getDirName(path) == "" && getFileName(path) == ""  && getFileNameBase(path) == "" && getFileNameExt(path) == "");
}
