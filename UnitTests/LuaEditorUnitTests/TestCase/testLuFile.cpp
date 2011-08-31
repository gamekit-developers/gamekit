#include "StdAfx.h"

#include "luFile.h"
#include <wx/filename.h> 

#define TEST_CASE_NAME testLuFile

#define TEST_PROJ_FILE "TestData/Test0.okproj"
#define TEST_HHK_FILE "TestData/index.hhk"

TEST(TEST_CASE_NAME, testReadHhk)
{
	luHhkFile file;
	ASSERT_TRUE(file.load(TEST_HHK_FILE));

	EXPECT_TRUE(file.getHtmlFile("About") == "_about.html");
	EXPECT_TRUE(file.getHtmlFile("EditObjectActuator") == "class_edit_object_actuator.html");
}

/*
Version = 1
ProjectName = "MyFirstProject"
StartLuaFile = "Test0.lua"
Files = {
	"Test0.lua",
	"Test0.blend"
}
*/

#define TEST_PROJ_FILE_COUNT 2


TEST(TEST_CASE_NAME, testLoadProjFile)
{
	luProjectFile file;
	ASSERT_TRUE(file.load(TEST_PROJ_FILE));
	EXPECT_TRUE(file.getProjVersion() == 1);
	EXPECT_TRUE(file.getProjName() == "MyFirstProject");
	EXPECT_TRUE(file.getStartLuaFile() == "Test0.lua");

	ASSERT_TRUE(file.getFileSize() == TEST_PROJ_FILE_COUNT);
	EXPECT_TRUE(file.getFile(0) == "Test0.lua");
	EXPECT_TRUE(file.getFile(1) == "Test0.blend");

	EXPECT_TRUE(file.getProjFileName() == luFile::getAbsolutePath(TEST_PROJ_FILE));
	//printf("%s %s\n", file.getProjDir().c_str(), file.getProjFileName().c_str());
	EXPECT_TRUE(file.getProjDir() == luFile::getAbsolutePath("TestData"));
}


TEST(TEST_CASE_NAME, testAddRemoveFile)
{
	luProjectFile file;
	EXPECT_TRUE(file.create("test0.okproj"));

#define FILE1 "Test1.lua"
#define FILE2 "Test1.blend"

	EXPECT_TRUE(file.addFile(FILE1));
	EXPECT_TRUE(file.addFile(FILE2));

	EXPECT_TRUE(file.getFileSize() == 2);
	EXPECT_TRUE(file.hasFile(FILE1));
	EXPECT_TRUE(file.hasFile(FILE2));

	//for (size_t i = 0; i < file.getFileSize(); i++)
	//	printf("%s\n", file.getFile(i).c_str());

#ifdef WIN32
	EXPECT_TRUE(file.hasFile(utStringUtils::upper(FILE1)));	// test lower case name
	EXPECT_TRUE(file.hasFile(utStringUtils::lower(FILE1)));	// test upper case name
#else
	EXPECT_TRUE(file.hasFile(!utStringUtils::upper(FILE1)));	// test lower case name
	EXPECT_TRUE(file.hasFile(!utStringUtils::lower(FILE1)));	// test upper case name
#endif

	EXPECT_TRUE(file.removeFile(FILE1));
	EXPECT_TRUE(file.removeFile(FILE2));

	EXPECT_TRUE(file.getFileSize() == 0);
	EXPECT_TRUE(!file.hasFile(FILE1));
	EXPECT_TRUE(!file.hasFile(FILE2));

#undef FILE1
#undef FILE2

}

TEST(TEST_CASE_NAME, testRelativePath)
{
	luProjectFile file;
	gkString cwd = luFile::getCwd();
	EXPECT_TRUE(file.create(cwd + "/work/proj/my.okproj"));	
	EXPECT_TRUE(luFile::convPathStyle(file.getProjDir()) == luFile::convPathStyle(cwd + "/work/proj"));

	//printf("dir: %s rel: %d\n", luFile::getAbsolutePath("\\work\\proj").c_str(), luFile::isRelativePath("\\work\\lua\\test1.lua"));

	EXPECT_TRUE(file.addFile(cwd + "/work/lua/test1.lua"));
	EXPECT_TRUE(file.addFile(cwd + "/work/blend/test1.blend"));
	EXPECT_TRUE(file.addFile(cwd + "/work/proj/test2.blend"));
	
	//printf("%s\n", luFile::convPathStyle(file.getFile(0)).c_str());
	//printf("%s\n", file.getFile(1).c_str());

	EXPECT_TRUE(luFile::convPathStyle(file.getFile(0)) == "../lua/test1.lua");
	EXPECT_TRUE(luFile::convPathStyle(file.getFile(1)) == "../blend/test1.blend");
	EXPECT_TRUE(luFile::convPathStyle(file.getFile(2)) == "test2.blend");
}
