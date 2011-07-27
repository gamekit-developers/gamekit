
#include "gtest/gtest.h"

#include "fbtFile.h"
#include "customFile.h"
#include "Custom.h"

#define TEST_CASE_NAME testFbtCustomFile

bool cstmStructEq(Custom::cstmStruct& a, Custom::cstmStruct& b)
{
	if (a.floatValue != b.floatValue ||
		a.intValue != b.intValue ||
		a.shortValue != b.shortValue ||
		a.charValue != b.charValue ||
		a.pad0 != b.pad0 ||
		a.pad1 != b.pad1)
		return false;
	return true;
}

bool cstmMainEq(Custom::cstmMain& a, Custom::cstmMain& b)
{
	if (a.floatValue != b.floatValue ||
		a.intValue != b.intValue ||
		a.shortValue != b.shortValue ||
		a.charValue != b.charValue ||
		a.pad0 != b.pad0 ||
		a.pad1 != b.pad1)
		return false;
	
	if(!cstmStructEq(a.structValue, b.structValue))
		return false;
	
	for (int i =0; i<4; i++)
	{
		if (a.floatArray[i] != b.floatArray[i] ||
			a.intArray[i] != b.intArray[i] ||
			a.shortArray[i] != b.shortArray[i])
			return false;
	}
	
	for (int i=0; i<2; i++)
	{
		if(!cstmStructEq(a.structArray[i], b.structArray[i]))
			return false;
	}
	
	for (int i=0; i<32; i++)
	{
		if(a.charArray[i] != b.charArray[i])
			return false;
	}
	
	for (int i =0; i<4; i++)
	{
		for (int j =0; i<4; i++)
		{
			if(a.float2DArray[i][j] != b.float2DArray[i][j])
				return false;
		}
	}
	
	int i=0;
	Custom::cstmStruct* sa = a.structList.m_first;
	Custom::cstmStruct* sb = b.structList.m_first;
	while(sa && sb)
	{
		if(!cstmStructEq(*sa, *sb))
			return false;
		
		if(i==3)
		{
			if (a.structPtr != sa ||
				b.structPtr != sb)
				return false;
		}
		
		i++;
		sa = sa->m_next;
		sb = sb->m_next;
	}
	
	if(!(sa==0 && sb==0))
		return false;
	
	return true;
}

bool cstmGlobEq(Custom::cstmGlobal& a, Custom::cstmGlobal& b)
{
	if (a.versionmajor != b.versionmajor ||
		a.versionminor != b.versionminor)
		return false;
	return cstmMainEq(a.main, b.main);
}

void cstmFillGlobal(Custom::cstmGlobal& global)
{
	global.versionmajor = 0;
	global.versionminor = 1;
	
	
	global.main.floatValue = 3.14158f;
	global.main.intValue = 666;
	global.main.shortValue = 66;
	global.main.charValue = 'X';
	
	global.main.structValue.m_next = 0;
	global.main.structValue.m_prev = 0;
	global.main.structValue.floatValue = 3.14158f;
	global.main.structValue.intValue = 999;
	global.main.structValue.shortValue = 99;
	global.main.structValue.charValue = 'T';
	
	for (int i=0; i<4; i++)
	{
		global.main.floatArray[i] = 3.14158f * i;
		global.main.intArray[i] = i;
		global.main.shortArray[i] = i;
	}
	
	for (int i=0; i<2; i++)
	{
	
		global.main.structArray[i].m_next = 0;
		global.main.structArray[i].m_prev = 0;
		global.main.structArray[i].floatValue = 3.14158f;
		global.main.structArray[i].intValue = 999;
		global.main.structArray[i].shortValue = 99;
		global.main.structArray[i].charValue = 'T';
	}
	
	char c ='A';
	for (int i=0; i<32; i++)
	{
		global.main.charArray[i] = c++;
	}
	
	for (int i =0; i<4; i++)
	{
		for (int j =0; i<4; i++)
		{
			global.main.float2DArray[i][j] = 3.14158f * i;
		}
	}
	
	c='A';
	for (int i =0; i<7; i++)
	{
		Custom::cstmStruct* s = new Custom::cstmStruct();
		
		s->floatValue = 3.14158f*i;
		s->intValue = 666*i;
		s->shortValue = 6*i;
		s->charValue = c++;
		
		global.main.structList.push_back(s);
		
		if(i==3)
			global.main.structPtr = s;
	}
}


TEST(TEST_CASE_NAME, readwriteCustomFile)
{
	cstmFile file1, file2;
	
	cstmFillGlobal(*file1.m_global);
	
	file1.reflect("testFbtCustomFile.cstm");
	file2.parse("testFbtCustomFile.cstm");
	
	EXPECT_TRUE(cstmGlobEq(*file1.m_global, *file2.m_global));
}


