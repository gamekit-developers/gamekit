#include "StdAfx.h"

#include <string>
#include "fbtBlend.h"
#include "Blender.h"

using namespace Blender;

#define TEST_CASE_NAME testFbtBlendFile

#ifndef GKB_IDNAME
#define GKB_IDNAME(x) ((x) && (x)->id.name[0] != '0' ? (x)->id.name + 2 : "")
#endif

#ifndef OB_CAMERA
#define OB_CAMERA 11
#endif

bool parseBlendFile(const char *fname)
{
	fbtBlend fp;

	return fp.parse(fname, fbtFile::PM_READTOMEMORY) == fbtFile::FS_OK;	
}

TEST(TEST_CASE_NAME, parseBlend32bit)
{	
	EXPECT_TRUE(parseBlendFile("TestData/be32bit.blend"));
	EXPECT_TRUE(parseBlendFile("TestData/le32bitLink.blend"));
	EXPECT_TRUE(parseBlendFile("TestData/le32_bv225.blend"));
	EXPECT_TRUE(parseBlendFile("TestData/Refl_le32bitLink.blend"));
}

TEST(TEST_CASE_NAME, parseBlend64bit)
{	
	EXPECT_TRUE(parseBlendFile("TestData/le64bitLink.blend"));
}

TEST(TEST_CASE_NAME, findCameraObj)
{
	fbtBlend fp;

	const char *fname = "TestData/be32bit.blend";
	ASSERT_EQ(fp.parse(fname, fbtFile::PM_READTOMEMORY), fbtFile::FS_OK);

	bool findCameraObj = 0;
	std::string cameraName = "camera0";
	 
	fbtList& objects = fp.m_object;
	for (Object* ob = (Object*)objects.first; ob; ob = (Object*)ob->id.next)
	{
		if (cameraName == GKB_IDNAME(ob) && ob->type == OB_CAMERA)
		{
			findCameraObj = true;
			break;
		}
	}

	EXPECT_TRUE(findCameraObj);
}