/* Copyright (C) 2006 Charlie C
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/
#include <assert.h>

#include "bDNA.h"
#include "bChunk.h"
#include <string.h>
#include <stdlib.h>

using namespace bParse;

// 32 && 64 bit versions
extern unsigned char DNAstr[];
extern int DNAlen;

//unsigned char DNAstr[]={0};
//int DNAlen=0;


// removed... can no longer test
 //extern unsigned char DNAstr64[];
 //extern int DNAlen64;


// ----------------------------------------------------- //
bDNA::bDNA()
	:	mPtrLen(0)
{
	// --
}

// ----------------------------------------------------- //
bDNA::~bDNA()
{
	// --
}

// ----------------------------------------------------- //
bool bDNA::lessThan(bDNA *file)
{
	return ( mNames.size() < file->mNames.size());
}

// ----------------------------------------------------- //
char *bDNA::getName(int ind)
{
	assert(ind <= (int)mNames.size());
	return mNames[ind];
}


// ----------------------------------------------------- //
char *bDNA::getType(int ind)
{
	assert(ind<=  (int)mTypes.size());
	return mTypes[ind];
}


// ----------------------------------------------------- //
short *bDNA::getStruct(int ind)
{
	assert(ind <=  (int)mStructs.size());
	return mStructs[ind];
}


// ----------------------------------------------------- //
short bDNA::getLength(int ind)
{
	assert(ind <=  (int)mTlens.size());
	return mTlens[ind];
}


// ----------------------------------------------------- //
int bDNA::getReverseType(short type)
{
	std::map<int, int>::iterator it = mStructReverse.find(type);
	if (it != mStructReverse.end())
		return it->second;
	return -1;
}

// ----------------------------------------------------- //
int bDNA::getReverseType(char *type)
{
	std::map<bString, int>::iterator it = mTypeLookup.find(type);
	if (it != mTypeLookup.end())
		return it->second;
	return -1;
}

// ----------------------------------------------------- //
int bDNA::getNumStructs()
{
	return (int)mStructs.size();
}

// ----------------------------------------------------- //
bool bDNA::flagNotEqual(int dna_nr)
{
	assert(dna_nr <=	(int)mCMPFlags.size());
	return mCMPFlags[dna_nr] == FDF_STRUCT_NEQU;
}

// ----------------------------------------------------- //
bool bDNA::flagEqual(int dna_nr)
{
	assert(dna_nr <=	(int)mCMPFlags.size());
	int flag = mCMPFlags[dna_nr];
	return  flag == FDF_STRUCT_EQU;
}

// ----------------------------------------------------- //
bool bDNA::flagNone(int dna_nr)
{
	assert(dna_nr <=	(int)mCMPFlags.size());
	return mCMPFlags[dna_nr] == FDF_NONE;
}

// ----------------------------------------------------- //
int bDNA::getPointerSize()
{
	return mPtrLen;
}

// ----------------------------------------------------- //
void bDNA::initRecurseCmpFlags(int iter)
{
	// iter is FDF_STRUCT_NEQU

	short *oldStrc = mStructs[iter];
	short type = oldStrc[0];

	for (int i=0; i<(int)mStructs.size(); i++)
	{
		if (i != iter && mCMPFlags[i] == FDF_STRUCT_EQU )
		{
			short *curStruct = mStructs[i];
			int eleLen = curStruct[1];
			curStruct+=2;

			for (int j=0; j<eleLen; j++, curStruct+=2)
			{
				if (curStruct[0] == type)
				{
					char *name = mNames[curStruct[1]];
					if (name[0] != '*')
					{
						mCMPFlags[i] = FDF_STRUCT_NEQU;
						initRecurseCmpFlags(i);
					}
				}
			}
		}
	}
}

// ----------------------------------------------------- //
void bDNA::initCmpFlags(bDNA *memDNA)
{

    // compare the file to memory
	// this ptr should be the file data


	assert(!mNames.empty() && "SDNA empty!");
	mCMPFlags.resize(mStructs.size(), FDF_NONE);



	for (int i=0; i<(int)mStructs.size(); i++)
	{
		short *oldStruct = mStructs[i];

		int oldLookup = getReverseType(oldStruct[0]);
		if (oldLookup == -1)
		{
			mCMPFlags[i] = FDF_NONE;
			continue;
		}
//#define SLOW_FORWARD_COMPATIBLE 1
#ifdef SLOW_FORWARD_COMPATIBLE
		char* typeName = mTypes[oldLookup];
		int newLookup = memDNA->getReverseType(typeName);
		if (newLookup == -1)
		{
			mCMPFlags[i] = FDF_NONE;
			continue;
		}
		short *curStruct = memDNA->mStructs[newLookup];
#else
		// memory for file
		short *curStruct = memDNA->mStructs[oldLookup];
#endif	
	
		

		// rebuild...
		mCMPFlags[i] = FDF_STRUCT_NEQU;

#if 1

		if (curStruct[1] == oldStruct[1])
		{
			// type len same ...
			if (mTlens[oldStruct[0]] == memDNA->mTlens[curStruct[0]])
			{
				bool isSame = true;
				int elementLength = oldStruct[1];


				curStruct+=2;
				oldStruct+=2;


				for (int j=0; j<elementLength; j++, curStruct+=2, oldStruct+=2)
				{
					// type the same
					if (strcmp(mTypes[oldStruct[0]], memDNA->mTypes[curStruct[0]])!=0)
					{
						isSame=false;
						break;
					}

					// name the same
					if (strcmp(mNames[oldStruct[1]], memDNA->mNames[curStruct[1]])!=0)
					{
						isSame=false;
						break;
					}
				}
				// flag valid ==
				if (isSame)
					mCMPFlags[i] = FDF_STRUCT_EQU;
			}
		}
#endif
	}



	// recurse in
	for (int i=0; i<(int)mStructs.size(); i++)
	{
		if (mCMPFlags[i] == FDF_STRUCT_NEQU)
			initRecurseCmpFlags(i);
	}
}


// ----------------------------------------------------- //
void bDNA::initMemory()
{
	if (VOID_IS_8)
	{
        assert(0 && "64bit blend files not supported yet");
		//init((char*)DNAstr64, DNAlen64);
	}
	else
	{
		init((char*)DNAstr, DNAlen);
	}
}


// ----------------------------------------------------- //
void bDNA::init(char *data, int len, bool swap)
{
	int *intPtr=0;short *shtPtr=0;
	char *cp = 0;int dataLen =0;long nr=0;
	intPtr = (int*)data;

	/*
		SDNA (4 bytes) (magic number)
		NAME (4 bytes)
		<nr> (4 bytes) amount of names (int)
		<string>
		<string>
	*/

	if (strncmp(data, "SDNA", 4)==0)
	{
		// skip ++ NAME
		intPtr++; intPtr++;
	}



	// Parse names
	if (swap) dataLen = ChunkUtils::swapInt(*intPtr);
	else      dataLen = *intPtr;
	intPtr++;

	cp = (char*)intPtr;
	for (int i=0; i<dataLen; i++)
	{
		mNames.push_back(cp);
		while (*cp)cp++;
		cp++;
	}

	// ...
	nr= (long)cp;
	nr= (nr+3)&~3;
	cp= (char *)nr;



	/*
		TYPE (4 bytes)
		<nr> amount of types (int)
		<string>
		<string>
	*/

	intPtr = (int*)cp;
	assert(strncmp(cp, "TYPE", 4)==0); intPtr++;

	if (swap) dataLen = ChunkUtils::swapInt(*intPtr);
	else      dataLen = *intPtr;
	intPtr++;

	cp = (char*)intPtr;
	for (int i=0; i<dataLen; i++)
	{
		mTypes.push_back(cp);
		while (*cp)cp++;
		cp++;
	}

	nr= (long)cp;
	nr= (nr+3)&~3;
	cp= (char *)nr;


	/*
		TLEN (4 bytes)
		<len> (short) the lengths of types
		<len>
	*/

	// Parse type lens
	intPtr = (int*)cp;
	assert(strncmp(cp, "TLEN", 4)==0); intPtr++;

	dataLen = (int)mTypes.size();

	shtPtr = (short*)intPtr;
	for (int i=0; i<dataLen; i++, shtPtr++)
	{
		if (swap)
			shtPtr[0] = ChunkUtils::swapShort(shtPtr[0]);
		mTlens.push_back(shtPtr[0]);
	}

	if (dataLen & 1) shtPtr++;

	/*
		STRC (4 bytes)
		<nr> amount of structs (int)
		<typenr>
		<nr_of_elems>
		<typenr>
		<namenr>
		<typenr>
		<namenr>
	*/

	intPtr = (int*)shtPtr;
	cp = (char*)intPtr;
	assert(strncmp(cp, "STRC", 4)==0); intPtr++;

	if (swap) dataLen = ChunkUtils::swapInt(*intPtr);
	else      dataLen = *intPtr;
	intPtr++;


	shtPtr = (short*)intPtr;
	for (int i=0; i<dataLen; i++)
	{
		mStructs.push_back (shtPtr);
		if (swap)
		{
			shtPtr[0]= ChunkUtils::swapShort(shtPtr[0]);
			shtPtr[1]= ChunkUtils::swapShort(shtPtr[1]);

			int len = shtPtr[1];
			shtPtr+= 2;

			for (int a=0; a<len; a++, shtPtr+=2)
			{
				shtPtr[0]= ChunkUtils::swapShort(shtPtr[0]);
				shtPtr[1]= ChunkUtils::swapShort(shtPtr[1]);
			}
		}
		else
			shtPtr+= (2*shtPtr[1])+2;
	}


	// build reverse lookups
	for (int i=0; i<(int)mStructs.size(); i++)
	{
		short *strc = mStructs.at(i);
		if (!mPtrLen && strcmp(mTypes[strc[0]],"ListBase")==0)
		{
			mPtrLen = mTlens[strc[0]]/2;
		}

		mStructReverse.insert(std::make_pair(strc[0], i));
		mTypeLookup.insert(std::make_pair(mTypes[strc[0]],i));
	}
}


// ----------------------------------------------------- //
int bDNA::getArraySize(char* string)
{
	int ret = 1;
	bString os = string;

	char* next = 0;
	for (int i=0; i<(int)os.size(); i++)
	{
		char c = os[i];

		if (c == '[')
			next = &os[i+1];
		else if (c==']')
			if (next)
				ret *= atoi(next);
	}

//	print (string << ' ' << ret);
	return ret;
}

// ----------------------------------------------------- //
int bDNA::getElementSize(short type, short name)
{
	assert(name < (int)mNames.size());
	assert(type < (int)mTypes.size());

	char *el = mNames[name];

	int namelen = (int)strlen(el);
	int ret =0;
	int mult=1;

	if (el[0] == '*' || el[1] == '*')
	{
		if (el[namelen-1] == ']')
			mult = getArraySize(el);

		ret = mPtrLen*mult;
	}
	else if (type <= (int)mTlens.size())
	{
		if (el[namelen-1] == ']')
			mult = getArraySize(el);

		ret= mTlens[type]*mult;
	}
	return ret;
}


void bDNA::dumpTypeDefinitions()
{
	int i;

	int numTypes = mTypes.size();
	
	for (i=0;i<numTypes;i++)
	{

	}

	for (int i=0; i<(int)mStructs.size(); i++)
	{
		short *oldStruct = mStructs[i];

		int oldLookup = getReverseType(oldStruct[0]);
		if (oldLookup == -1)
		{
			mCMPFlags[i] = FDF_NONE;
			continue;
		}

		short* newStruct = mStructs[oldLookup];
		char* typeName = mTypes[newStruct[0]];
		printf("%3d: %s ",i,typeName);
		
		//char *name = mNames[oldStruct[1]];
		int len = oldStruct[1];
		printf(" (%d fields) ",len);
		oldStruct+=2;

		printf("{");
		int j;
		for (j=0; j<len; ++j,oldStruct+=2) {
			printf("%s %s",	mTypes[oldStruct[0]],mNames[oldStruct[1]]);
			if (j == len-1) {
				printf(";}");
			} else {
				printf("; ");
			}
		}
		printf("\n\n");

	}
	


#if 0
	/* dump out display of types and their sizes */
	for (i=0; i<bf->types_count; ++i) {
		/* if (!bf->types[i].is_struct)*/
		{
			printf("%3d: sizeof(%s%s)=%d",
				i,
				bf->types[i].is_struct ? "struct " : "atomic ",
				bf->types[i].name, bf->types[i].size);
			if (bf->types[i].is_struct) {
				int j;
				printf(", %d fields: { ", bf->types[i].fieldtypes_count);
				for (j=0; j<bf->types[i].fieldtypes_count; ++j) {
					printf("%s %s",
						bf->types[bf->types[i].fieldtypes[j]].name,
						bf->names[bf->types[i].fieldnames[j]]);
					if (j == bf->types[i].fieldtypes_count-1) {
						printf(";}");
					} else {
						printf("; ");
					}
				}
			}
			printf("\n\n");

		}
	}
#endif

}




//eof


