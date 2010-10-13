#include "StdAfx.h"


#include "fbtBullet.h"
#include "Bullet.h"

#define TEST_CASE_NAME testFbtBulletFile

bool testBulletFile(const char* fname)
{
	//btBulletFile fp;
	btBulletFile fp;

	if (fp.parse(fname, fbtFile::PM_READTOMEMORY) != fbtFile::FS_OK)
	{
		return false;
	}

	return true;
}


TEST(TEST_CASE_NAME, readBullet32bit)
{	
	EXPECT_TRUE(testBulletFile("TestData/le32Bitf.bullet"));
}
