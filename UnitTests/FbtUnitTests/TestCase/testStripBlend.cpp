#include "StdAfx.h"
#include "fbtBlend.h"
#include "Blender.h"
using namespace Blender;

#define TEST_CASE_NAME testStripBlend

#define TEST_IN_FILE "TestData/be32bit.blend"
#define TEST_OUT_FILE "TestTemp/be32bit.strip"

// Unneeded structures 
static FBTuint32 skipList[] =
{
	fbtCharHashKey("Panel").hash(),
	fbtCharHashKey("ARegion").hash(),
	fbtCharHashKey("ScrArea").hash(),
	fbtCharHashKey("ScrVert").hash(),
	fbtCharHashKey("ScrEdge").hash(),
	fbtCharHashKey("bScreen").hash(),

	// ... others
	0,
};


TEST(TEST_CASE_NAME, stripBlend)
{
	fbtBlend fp;
	fp.setIgnoreList(skipList);

	ASSERT_TRUE(fp.parse(TEST_IN_FILE, fbtFile::PM_COMPRESSED) == fbtFile::FS_OK);
	ASSERT_TRUE(fp.reflect(TEST_OUT_FILE) == fbtFile::FS_OK);

	fbtBlend fp2;
	ASSERT_TRUE(fp2.parse(TEST_OUT_FILE, fbtFile::PM_COMPRESSED) == fbtFile::FS_OK);
}
