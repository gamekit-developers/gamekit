#include "StdAfx.h"

#undef rad2 //WinSDK macro
#include "bBlenderFile.h"
#include "bMain.h"
#include "Blender.h"

#include "Loaders/Blender2/gkBlenderDefines.h"
#include "utStreams.h"


#define TEST_CASE_NAME		BlendFile
#define TEST_BLEND_FILE		"TestData/Test0.blend"

TEST(TEST_CASE_NAME, testReadObjects)
{
	utMemoryStream fs;
	fs.open(TEST_BLEND_FILE, utStream::SM_READ);

	ASSERT_TRUE(fs.isOpen());

	utMemoryStream buffer(utStream::SM_WRITE);
	fs.inflate(buffer);

	bParse::bBlenderFile bfile((char*)buffer.ptr(), buffer.size());
	bfile.parse(false);

	ASSERT_TRUE(bfile.ok());
	ASSERT_TRUE(bfile.getMain() != NULL);
	bParse::bListBasePtr* iter = bfile.getMain()->getObject();
	ASSERT_TRUE(iter != NULL);
		
	bool findCamera = false;
	gkString cameraName = "Camera";
	for (int i = 0; i < iter->size(); i++)
	{
		Blender::Object* ob = (Blender::Object*)iter->at(i);		
		if (cameraName == GKB_IDNAME(ob))
		{
			findCamera = true;
			EXPECT_EQ(ob->type, OB_CAMERA);
		}
	}

	EXPECT_TRUE(findCamera);
}
