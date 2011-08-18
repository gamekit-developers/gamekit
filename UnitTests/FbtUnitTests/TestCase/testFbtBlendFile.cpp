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
	//return fp.parse(fname, fbtFile::PM_COMPRESSED) == fbtFile::FS_OK;	
}



bool parse_Ptr_PtrPtr_PtrArray(const char *fname, 
						      int& ptrPtrCount,
						      int& ptrArrayCount)
{
	fbtBlend fp;
	bool parseOk = fp.parse(fname, fbtFile::PM_READTOMEMORY) == fbtFile::FS_OK;
	if (!parseOk)
		return false;

	for (Blender::Text* tx = (Blender::Text*)fp.m_text.first; tx; tx = (Blender::Text*)tx->id.next)
	{
		for (Blender::TextLine* tl = (Blender::TextLine*)tx->lines.first; tl; tl = tl->next)
		{
			bool textLine = tl->line != 0;
			if (!textLine)
				return false;

			bool lengthMatch = strlen(tl->line) == tl->len;
			if (!lengthMatch)
				return false;
		}
	}

	for (Blender::bScreen* bs = (Blender::bScreen*)fp.m_screen.first; bs; bs = (Blender::bScreen*)bs->id.next)
	{
		bool edgebaseElements = bs->edgebase.first != 0;
		if (!edgebaseElements)
			return false;

		for (Blender::ScrEdge* edge = (Blender::ScrEdge*)bs->edgebase.first; edge; edge = edge->next)
		{
			Blender::ScrVert* v1 = edge->v1;
			Blender::ScrVert* v2 = edge->v2;

			bool hasVerts = v1 != 0;
			if (!hasVerts)
				return false;
		}
	}

	ptrPtrCount = 0;
	ptrArrayCount = 0;

	fbtList& objects = fp.m_object;
	for (Object* ob = (Object*)objects.first; ob; ob = (Object*)ob->id.next)
	{
		bool meshHasData = ob->data && ob->type == 1;
		if (meshHasData)
		{

			Mesh* me = (Mesh*)ob->data;

			bool meshHasMaterials = me->mat && *me->mat;
			if (!meshHasMaterials)
				return false;

			for (int i = 0; i < me->totcol; ++i)
			{
				Material* ma = me->mat[i];
				if (ma)
				{
					++ptrPtrCount;

					if (ma->mtex)
					{
						int j = 0;
						while (ma->mtex[j] != 0)
						{
							if (ma->mtex[j]->tex)
							{
								++ptrArrayCount;
							}
							++j;
						}
					}
				}
			}
		}
	}
	return true;
}

TEST(TEST_CASE_NAME, parsePointer32BitLinks)
{
	int ptrPtrCount;
	int ptrArrayCount;
	EXPECT_TRUE(parse_Ptr_PtrPtr_PtrArray("TestData/le32bitLink.blend", ptrPtrCount, ptrArrayCount));

	ASSERT_EQ(ptrPtrCount, 4);
	ASSERT_EQ(ptrArrayCount, 24);
}

TEST(TEST_CASE_NAME, parsePointer64BitLinks)
{
	int ptrPtrCount;
	int ptrArrayCount;
	EXPECT_TRUE(parse_Ptr_PtrPtr_PtrArray("TestData/le64bitLink.blend", ptrPtrCount, ptrArrayCount));

	ASSERT_EQ(ptrPtrCount, 4);
	ASSERT_EQ(ptrArrayCount, 24);
}

TEST(TEST_CASE_NAME, parseBlend32bit)
{	
	EXPECT_TRUE(parseBlendFile("TestData/be32bit.blend"));
	EXPECT_TRUE(parseBlendFile("TestData/le32bit.blend"));	
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
		//printf("%d %s\n", ob->type, GKB_IDNAME(ob));
		if (cameraName == GKB_IDNAME(ob) && ob->type == OB_CAMERA)
		{
			findCameraObj = true;
			break;
		}
	}

	EXPECT_TRUE(findCameraObj);
}
