
#include "bBlenderFile.h"
#include "bMain.h"
#include "bDefines.h"
#include "bDNA.h"

using namespace bParse;

bBlenderFile::bBlenderFile(const char* fileName)
:bFile(fileName, "BLENDER")
{
	mMain= new bMain(this, fileName, mVersion);
}



bBlenderFile::bBlenderFile(char *memoryBuffer, int len)
:bFile(memoryBuffer,len, "BLENDER"),
mMain(0)
{
	mMain= new bMain(this, "memoryBuf", mVersion);
}


bBlenderFile::~bBlenderFile()
{
	delete mMain;
}


bMain* bBlenderFile::getMain()
{
	return mMain;
}

// ----------------------------------------------------- //
void bBlenderFile::parseData()
{
	print ("Building datablocks");
	print ("Chunk size = " << CHUNK_HEADER_LEN);
	print ("File chunk size = " << ChunkUtils::getOffset(mFlags));

	const bool swap = (mFlags&FD_ENDIAN_SWAP)!=0;
	int seek = 0;


	char *dataPtr = mFileBuffer+mDataStart;

	bChunkInd dataChunk;
	dataChunk.code = 0;


	dataPtr += ChunkUtils::getNextBlock(&dataChunk, dataPtr, mFlags);
	char *dataPtrHead = 0;

	while (dataChunk.code != DNA1)
	{
		



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

		if (dataChunk.code == GLOB)
		{
			m_glob = (bStructHandle*) id;
		}

		// next please!
		dataPtr += seek;

		seek =  ChunkUtils::getNextBlock(&dataChunk, dataPtr, mFlags);
		if (seek < 0)
			break;
	}

}

void	bBlenderFile::addDataBlock(char* dataBlock)
{
	mMain->addDatablock(dataBlock);
}

void bBlenderFile::resolvePointersMismatch()
{
//	printf("resolvePointersStructMismatch\n");

	int i;
	for (i=0;i<	m_listBaseFixupArray.size();i++)
	{
		char* cur = m_listBaseFixupArray.at(i);
		void** ptrptr = (void**) cur;
		void* listBasePtr = *ptrptr;
		listBasePtr = mMain->findLibPointer(listBasePtr);
		if (listBasePtr)
		{
			*(ptrptr) = listBasePtr;
			mMain->linkList(listBasePtr);
		} else
		{
//			mMain->linkList(listBasePtr);
//			printf("listbase not found %x\n",cur);
		}
	}
	
	for (i=0;i<	m_pointerFixupArray.size();i++)
	{
		char* cur = m_pointerFixupArray.at(i);
		void** ptrptr = (void**) cur;
		void* ptr = *ptrptr;
		ptr = mMain->findLibPointer(ptr);
		if (ptr)
		{
			//printf("Fixup pointer!\n");
			*(ptrptr) = ptr;
		} else
		{
//			printf("pointer not found: %x\n",cur);
		}
	}
}

///this loop only works fine if the Blender DNA structure of the file matches the headerfiles
void bBlenderFile::resolvePointersStruct(bChunkInd& dataChunk)
{
	bool patched = false;
	short int* oldStruct = mFileDNA->getStruct(dataChunk.dna_nr);
	short oldLen = mFileDNA->getLength(oldStruct[0]);

	char* cur	= (char*)mMain->findLibPointer(dataChunk.oldPtr);
	for (int block=0; block<dataChunk.nr; block++)
	{
		oldStruct = mFileDNA->getStruct(dataChunk.dna_nr);
		
		int elementLength = oldStruct[1];
		oldStruct+=2;


		char* memType;
		char* memName;

		char* elemPtr= cur;

		for (int ele=0; ele<elementLength; ele++, oldStruct+=2)
		{

			memType = mFileDNA->getType(oldStruct[0]);
			memName = mFileDNA->getName(oldStruct[1]);
			//printf("%s %s\n",memType,memName);
			if (memName[0] == '*')
			{
				void** ptrptr = (void**) elemPtr;
				void* ptr = *ptrptr;
				ptr = mMain->findLibPointer(ptr);
				if (ptr)
				{
					//printf("Fixup pointer!\n");
					*(ptrptr) = ptr;
					patched = true;
				}
			}
			if (strcmp(memType,"ListBase")==0)
			{
			//	printf("Fixup ListBase!\n");
				void** ptrptr = (void**) elemPtr;
				void* listBasePtr = *ptrptr;
				listBasePtr = mMain->findLibPointer(listBasePtr);
				if (listBasePtr)
				{
					*(ptrptr) = listBasePtr;
					mMain->linkList(listBasePtr);
					patched = true;
				}
			}

			int size = mFileDNA->getElementSize(oldStruct[0], oldStruct[1]);
			elemPtr+=size;

		}
		cur += oldLen;
	}

	if (!patched)
	{
		//printf("not patched %s, #%d\n",oldType,dataChunk.nr);
	}
}


///Resolve pointers replaces the original pointers in structures, and linked lists by the new in-memory structures
void bBlenderFile::resolvePointers()
{
	printf("resolvePointers start\n");
	char *dataPtr = mFileBuffer+mDataStart;

	if (mFlags & (FD_BITS_VARIES | FD_VERSION_VARIES))
	{
		resolvePointersMismatch();	
	}
	
	{
		for (int i=0;i<mMain->m_chunks.size();i++)
		{
			bChunkInd& dataChunk = mMain->m_chunks.at(i);

			if (mFileDNA->flagEqual(dataChunk.dna_nr))
			{
				//dataChunk.len
				short int* oldStruct = mFileDNA->getStruct(dataChunk.dna_nr);
				char* oldType = mFileDNA->getType(oldStruct[0]);
				
				//printf("------------------------------------------");
				//printf("Struct %s\n",oldType);

				

				//skip certain structures
		///Warning: certain structures might need to be skipped, such as CustomDataLayer, Link etc. Not skipping them causes crashes.
				if (strcmp(oldType,"CustomDataLayer")==0)
					continue;
				if (strcmp(oldType,"Link")==0)
					continue;
		///Other types are skipped just because they dont' containt pointers, just for optimization (MVert,MEdge,MFace,ScrVert)
				if (strcmp(oldType,"MVert")==0)
					continue;
				if (strcmp(oldType,"MEdge")==0)
					continue;
				if (strcmp(oldType,"MFace")==0)
					continue;
				if (strcmp(oldType,"ScrVert")==0)
					continue;

				resolvePointersStruct(dataChunk);
			} else
			{
				//printf("skipping mStruct\n");
			}
		}
	}
	
	printf("resolvePointers end\n");
}