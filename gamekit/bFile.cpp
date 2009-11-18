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
#include "bFile.h"
#include "bCommon.h"
#include "bMain.h"
#include "bDefines.h"
#include "bChunk.h"
#include "bDNA.h"
#include <math.h>
using namespace bParse;


// ----------------------------------------------------- //
bFile::bFile(const char *filename)
	:	mOwnsBuffer(false),
		mFileBuffer(0),
		mFileLen(0),
		mVersion(0),
		mDataStart(0),
		mFileDNA(0),
		mMemoryDNA(0),
		mMain(0),
		mFlags(FD_INVALID)
{
	FILE *fp = fopen(filename, "rb");
	if (fp)
	{
		fseek(fp, 0L, SEEK_END);
		mFileLen = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		mFileBuffer = (char*)malloc(mFileLen+1);
		fread(mFileBuffer, mFileLen, 1, fp);

		fclose(fp);

		//
		parseHeader();
		mMain= new bMain(this, filename, mVersion);
	}
}

// ----------------------------------------------------- //
bFile::bFile( char *memoryBuffer, int len)
:	mOwnsBuffer(true),
	mFileBuffer(0),
		mFileLen(0),
		mVersion(0),
		mDataStart(0),
		mFileDNA(0),
		mMemoryDNA(0),
		mMain(0),
		mFlags(FD_INVALID)
{
	mFileBuffer = memoryBuffer;
	mFileLen = len;
	
	parseHeader();
	mMain= new bMain(this, "memoryBuf", mVersion);
}


// ----------------------------------------------------- //
bFile::~bFile()
{
	if (mOwnsBuffer && mFileBuffer)
	{
		free(mFileBuffer);
		mFileBuffer = 0;
	}

	delete mMain;
	delete mMemoryDNA;
	delete mFileDNA;
}


// ----------------------------------------------------- //
bMain *bFile::getMain()
{
	return mMain;
}


// ----------------------------------------------------- //
void bFile::parseHeader()
{
	char *blenderBuf = mFileBuffer;
	char header[SIZEOFBLENDERHEADER+1] ;
	memcpy(header, blenderBuf, SIZEOFBLENDERHEADER);
	header[SIZEOFBLENDERHEADER]='\0';

	if (strncmp(header, "BLENDER", 7)!=0)
	{
		print ("Invalid blend file...");
		return;
	}

	char *ver = header+9;
	mVersion = atoi(ver);
	if (mVersion <= 241)
		print ("Warning, " << mVersion<< " not fully tested : <= 242");


	int endian= 1;
	endian= ((char*)&endian)[0];

	// swap ptr sizes...
	if (header[7]=='-')
	{
		mFlags |= FD_FILE_64;
		if (!VOID_IS_8)
			mFlags |= FD_VARIES;
	}
	else if (VOID_IS_8) mFlags |= FD_VARIES;

	// swap endian...
	if (header[8]=='V' && endian ==1)
		mFlags |= FD_SWAP;
	else
		if (endian==0)
			mFlags |= FD_SWAP;


	print (header);
	print ("sizeof(void*) == " << sizeof(void*));
	print ("Swapping endian? "<< ((mFlags & FD_SWAP)!=0));
	print ("File format is "<< ((mFlags &FD_FILE_64)!=0?"64":"32") << "bit");
	print ("Varing pointer sizes? "<< ((mFlags & FD_VARIES)!=0));


	mFlags |= FD_OK;
}

// ----------------------------------------------------- //
bool bFile::ok()
{
	return (mFlags &FD_OK)!=0;
}

// ----------------------------------------------------- //
void bFile::parse()
{
	if ( (mFlags &FD_OK) ==0)
		return;

	char *blenderData = mFileBuffer;
	int sdnaPos=0;

	char *tempBuffer = blenderData;
	for (int i=0; i<mFileLen; i++)
	{
		// looking for the data's starting position
		// and the start of SDNA decls

		if (!mDataStart && strncmp(tempBuffer, "REND", 4)==0)
			mDataStart = i;
		if (!sdnaPos && strncmp(tempBuffer, "SDNA", 4)==0)
			sdnaPos = i;

		if (mDataStart && sdnaPos) break;
		tempBuffer++;
	}

	mFileDNA = new bDNA();
	mFileDNA->init(blenderData+sdnaPos, mFileLen-sdnaPos, (mFlags & FD_SWAP)!=0);

	mFileDNA->dumpTypeDefinitions();

	mMemoryDNA = new bDNA();
	mMemoryDNA->initMemory();


	// as long as it kept up to date it will be ok!!
	if (mMemoryDNA->lessThan(mFileDNA))
	{
		//print ("Fatal error, file DNA is newer than built in...");
		print ("Warning, file DNA is newer than built in...");
	//	return;
	}

	mFileDNA->initCmpFlags(mMemoryDNA);
	parseData();
}


// ----------------------------------------------------- //
void bFile::parseData()
{
	print ("Building datablocks");
	print ("Chunk size = " << CHUNK_HEADER_LEN);
	print ("File chunk size = " << ChunkUtils::getOffset(mFlags));

	const bool swap = (mFlags&FD_SWAP)!=0;
	int seek = 0;


	char *dataPtr = mFileBuffer+mDataStart;

	bChunkInd dataChunk;
	dataChunk.code = 0;


	dataPtr += ChunkUtils::getNextBlock(&dataChunk, dataPtr, mFlags);
	char *dataPtrHead = 0;

	while (dataChunk.code != DNA1)
	{
		

//		if (dataChunk.code == GLOB)
//		{
//			dataPtr += seek;
//			continue;
//		}

		// one behind
		if (dataChunk.code == DNA1) break;

		// same as (BHEAD+DATA dependancy)
		dataPtrHead = dataPtr+ChunkUtils::getOffset(mFlags);
		char *id = readStruct(dataPtrHead, dataChunk);

		// lookup maps
		if (id)
		{
			mLibPointers.insert(std::make_pair(dataChunk.oldPtr, (bStructHandle*)id));

			mMain->m_chunks.push_back(dataChunk);
			// block it
			bListBasePtr *listID = mMain->getListBasePtr(dataChunk.code);
			if (listID)
				listID->push_back((bStructHandle*)id);
		}

		// next please!
		dataPtr += seek;

		seek =  ChunkUtils::getNextBlock(&dataChunk, dataPtr, mFlags);
		if (seek < 0)
			break;
	}

}

// ----------------------------------------------------- //
void bFile::swap(char *head, bChunkInd& dataChunk)
{
	char *data = head;
	short *strc = mFileDNA->getStruct(dataChunk.dna_nr);
	int len = mFileDNA->getLength(strc[0]);

	for (int i=0; i<dataChunk.nr; i++)
	{
		swapStruct(dataChunk.dna_nr, data);
		data+=len;
	}
}

// ----------------------------------------------------- //
char* bFile::readStruct(char *head, bChunkInd&  dataChunk)
{
	if (mFlags & FD_SWAP)
		swap(head, dataChunk);

	

	if (!mFileDNA->flagEqual(dataChunk.dna_nr))
	{
		// Ouch! need to rebuild the struct
		short *oldStruct,*curStruct;
		char *oldType, *newType;
		int oldLen, curLen, reverseOld;


		oldStruct = mFileDNA->getStruct(dataChunk.dna_nr);
		oldType = mFileDNA->getType(oldStruct[0]);
		oldLen = mFileDNA->getLength(oldStruct[0]);


		reverseOld = mMemoryDNA->getReverseType(oldType);

		bool isLink = false;

		if (strcmp("Link",oldType)==0)
		{
			isLink = true;
		}


		if ((reverseOld!=-1) && (!isLink))
		{
			// make sure it's here
			//assert(reverseOld!= -1 && "getReverseType() returned -1, struct required!");

			//
			curStruct = mMemoryDNA->getStruct(reverseOld);
			newType = mMemoryDNA->getType(curStruct[0]);
			curLen = mMemoryDNA->getLength(curStruct[0]);



			// make sure it's the same
			assert((strcmp(oldType, newType)==0) && "internal error, struct mismatch!");


			// numBlocks * length
    		char *dataAlloc = new char[(dataChunk.nr*curLen)+1];
			memset(dataAlloc, 0, (dataChunk.nr*curLen)+1);

			// track allocated
			mMain->addDatablock(dataAlloc);

			char *cur = dataAlloc;
			char *old = head;
			for (int block=0; block<dataChunk.nr; block++)
			{
				parseStruct(cur, old, dataChunk.dna_nr, reverseOld);

				cur += curLen;
				old += oldLen;
			}
			return dataAlloc;
		}
	} else
	{
		//printf("equal, just memcpy");
	}


	char *dataAlloc = new char[(dataChunk.len)+1];
	memset(dataAlloc, 0, dataChunk.len+1);


	// track allocated
	mMain->addDatablock(dataAlloc);
	memcpy(dataAlloc, head, dataChunk.len);
	return dataAlloc;

}


// ----------------------------------------------------- //
void bFile::parseStruct(char *strcPtr, char *dtPtr, int old_dna, int new_dna)
{
	if (old_dna == -1) return;
	if (new_dna == -1) return;

	if (mFileDNA->flagEqual(old_dna))
	{
		short *strc = mFileDNA->getStruct(old_dna);
		int len = mFileDNA->getLength(strc[0]);

		memcpy(strcPtr, dtPtr, len);
		return;
	}

	// Ok, now build the struct
	char *memType, *memName, *cpc, *cpo;
	short *fileStruct, *filePtrOld, *memoryStruct, *firstStruct;
	int elementLength, size, revType, old_nr, new_nr, fpLen;
	short firstStructType;


	// File to memory lookup
	memoryStruct = mMemoryDNA->getStruct(new_dna);
	fileStruct = mFileDNA->getStruct(old_dna);
	firstStruct = fileStruct;


	filePtrOld = fileStruct;
	firstStructType = mMemoryDNA->getStruct(0)[0];

	// Get number of elements
	elementLength = memoryStruct[1];
	memoryStruct+=2;

	cpc = strcPtr; cpo = 0;
	for (int ele=0; ele<elementLength; ele++, memoryStruct+=2)
	{
		memType = mMemoryDNA->getType(memoryStruct[0]);
		memName = mMemoryDNA->getName(memoryStruct[1]);

		size = mMemoryDNA->getElementSize(memoryStruct[0], memoryStruct[1]);
		revType = mMemoryDNA->getReverseType(memoryStruct[0]);

		if (revType != -1 && memoryStruct[0]>=firstStructType && memName[0] != '*')
		{
			cpo = getFileElement(firstStruct, memName, memType, dtPtr, &filePtrOld);
			if (cpo)
			{
				old_nr = mFileDNA->getReverseType(memType);
				new_nr = revType;
				fpLen = mFileDNA->getElementSize(filePtrOld[0], filePtrOld[1]);


				parseStruct(cpc, cpo, old_nr, new_nr);
				cpc+=size;
				cpo+=fpLen;
			}
			else
				cpc+=size;
		}
		else
		{
			getMatchingFileDNA(fileStruct, memName, memType, cpc, dtPtr);
			cpc+=size;
		}
	}
}

// ----------------------------------------------------- //
static void getElement(int arrayLen, const char *cur, const char *old, char *oldPtr, char *curData)
{
#define getEle(value, current, type, cast, size, ptr)\
	if (strcmp(current, type)==0)\
	{\
		value = (*(cast*)ptr);\
		ptr += size;\
	}

#define setEle(value, current, type, cast, size, ptr)\
	if (strcmp(current, type)==0)\
	{\
		(*(cast*)ptr) = value;\
		ptr += size;\
	}
	double value = 0.0;

	for (int i=0; i<arrayLen; i++)
	{
		getEle(value, old, "char",   char,   sizeof(char),   oldPtr);
		setEle(value, cur, "char",   char,   sizeof(char),   curData);
		getEle(value, old, "short",  short,  sizeof(short),  oldPtr);
		setEle(value, cur, "short",  short,  sizeof(short),  curData);
		getEle(value, old, "int",    int,    sizeof(int),    oldPtr);
		setEle(value, cur, "int",    int,    sizeof(int),    curData);
		getEle(value, old, "long",   int,    sizeof(int),    oldPtr);
		setEle(value, cur, "long",   int,    sizeof(int),    curData);
		getEle(value, old, "float",  float,  sizeof(float),  oldPtr);
		setEle(value, cur, "float",  float,  sizeof(float),  curData);
		getEle(value, old, "double", double, sizeof(double), oldPtr);
		setEle(value, cur, "double", double, sizeof(double), curData);
	}
}


// ----------------------------------------------------- //
void bFile::swapData(char *data, short type, int arraySize)
{
	if (mFlags &FD_SWAP)
	{
		if (type == 2 || type == 3)
		{
			short *sp = (short*)data;
			for (int i=0; i<arraySize; i++)
			{
				sp[0] = ChunkUtils::swapShort(sp[0]);
				sp++;
			}
		}
		if (type>3 && type <8)
		{
			char c;
			char *cp = data;
			for (int i=0; i<arraySize; i++)
			{
				c = cp[0];
				cp[0] = cp[3];
				cp[3] = c;
				c = cp[1];
				cp[1] = cp[2];
				cp[2] = c;
				cp+=4;
			}
		}
	}
}


// ----------------------------------------------------- //
void bFile::getMatchingFileDNA(short* dna_addr, bString lookupName,  bString lookupType, char *strcData, char *data)
{
	// find the matching memory dna data
	// to the file being loaded. Fill the
	// memory with the file data...

	int len = dna_addr[1], arrayLen;
	dna_addr+=2;

	for (int i=0; i<len; i++, dna_addr+=2)
	{
		bString type = mFileDNA->getType(dna_addr[0]);
		bString name = mFileDNA->getName(dna_addr[1]);

		int eleLen = mFileDNA->getElementSize(dna_addr[0], dna_addr[1]);
		arrayLen = mFileDNA->getArraySize((char*)name.c_str());

		if (name == lookupName)
		{
			if (name[0] == '*')
			{
				// cast pointers

				int ptrFile = mFileDNA->getPointerSize();
				int ptrMem = mMemoryDNA->getPointerSize();

				if (ptrFile == ptrMem)
					memcpy(strcData, data, ptrMem);

				else if (ptrMem==4 && ptrFile==8)
				{
					long64 longValue = 0;
					longValue = *((long64*)data);
					*((int*)strcData) = longValue>>3;
				}
				else if (ptrMem==8 && ptrFile==4)
					*((long64*)strcData)= *((int*)data);
				else
				{
					print (ptrFile << ' ' << ptrMem);
					assert(0 && "Invalid pointer len");
				}
			}

			else if (type==lookupType)
				memcpy(strcData, data, eleLen);
			else
				getElement(arrayLen, lookupType.c_str(), type.c_str(), data, strcData);

			// --
			return;
		}
		data+=eleLen;
	}
}


// ----------------------------------------------------- //
char* bFile::getFileElement(short *firstStruct, char *lookupName, char *lookupType, char *data, short **foundPos)
{
	short *old = firstStruct;//mFileDNA->getStruct(old_nr);
	int elementLength = old[1];
	bString sub0, sub1;
	old+=2;

	for (int i=0; i<elementLength; i++, old+=2)
	{
		char* type = mFileDNA->getType(old[0]);
		char* name = mFileDNA->getName(old[1]);
		int len = mFileDNA->getElementSize(old[0], old[1]);

		if (strcmp(lookupName, name)==0)
		{
			if (strcmp(type, lookupType)==0)
			{
				if (foundPos)
					*foundPos = old;
				return data;
			}
			return 0;
		}
		data+=len;
	}
	return 0;
}


// ----------------------------------------------------- //
void bFile::swapStruct(int dna_nr, char *data)
{
	if (dna_nr == -1) return;

	short *strc = mFileDNA->getStruct(dna_nr);
	short *firstStrc = strc;

	int elementLen= strc[1];
	strc+=2;

	short first = mFileDNA->getStruct(0)[0];

	char *buf = data;
	for (int i=0; i<elementLen; i++, strc+=2)
	{
		char *type = mFileDNA->getType(strc[0]);
		char *name = mFileDNA->getName(strc[1]);

		int size = mFileDNA->getElementSize(strc[0], strc[1]);
		if (strc[0] >= first && name[0]!='*')
		{
			int old_nr = mFileDNA->getReverseType(type);
			swapStruct(old_nr,buf);
		}
		else
		{
			int arrayLen = mFileDNA->getArraySize(name);
			swapData(buf, strc[0], arrayLen);
		}
		buf+=size;
	}
}

//eof
