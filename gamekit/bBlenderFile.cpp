
#include "bBlenderFile.h"
#include "bMain.h"
#include "bDefines.h"

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

void	bBlenderFile::addDataBlock(char* dataBlock)
{
	mMain->addDatablock(dataBlock);
}
