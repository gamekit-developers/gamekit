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
#ifndef __BFILE_H__
#define __BFILE_H__

#include "bCommon.h"

namespace bParse {

	// ----------------------------------------------------- //
	enum bFileFlags
	{
		FD_INVALID   =0,
		FD_OK        =1,
		FD_VOID_IS_8 =2,
		FD_SWAP      =4,
		FD_FILE_64   =8,
		FD_VARIES    =16
	};


	// ----------------------------------------------------- //
	class bFile
	{
	protected:
		
		char				m_headerString[7];

		bool				mOwnsBuffer;
		char*				mFileBuffer;
		int					mFileLen;
		int					mVersion;


		int					mDataStart;
		bDNA*				mFileDNA;
		bDNA*				mMemoryDNA;


		// 
	
		bPtrMap				mDataPointers;


		
		int					mFlags;

		virtual	void parseHeader();
		
		virtual	void parseData() = 0;

		virtual void resolvePointers() = 0;

		void parseStruct(char *strcPtr, char *dtPtr, int old_dna, int new_dna);
		void getMatchingFileDNA(short* old, bString lookupName, bString lookupType, char *strcData, char *data);
		char* getFileElement(short *firstStruct, char *lookupName, char *lookupType, char *data, short **foundPos);


		void swap(char *head, class bChunkInd& ch);

		void swapData(char *data, short type, int arraySize);
		void swapStruct(int dna_nr, char *data);




		char* readStruct(char *head, class bChunkInd& chunk);
		char *getAsString(int code);

	public:
		bFile(const char *filename, const char headerString[7]);
		
		//todo: make memoryBuffer const char
		//bFile( const char *memoryBuffer, int len);
		bFile( char *memoryBuffer, int len, const char headerString[7]);
		~bFile();

		bDNA*				getFileDNA()
		{
			return mFileDNA;
		}

		virtual	void	addDataBlock(char* dataBlock) = 0;

		int	getFlags() const
		{
			return mFlags;
		}

		bool ok();

		void parse(bool verboseDumpAllTypes);
		
	};
}


#endif//__BFILE_H__
