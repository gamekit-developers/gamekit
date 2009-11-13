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
#ifndef __BDNA_H__
#define __BDNA_H__


#include "bCommon.h"

namespace bParse {

	class bDNA
	{
	public:
		bDNA();
		~bDNA();

		void init(char *data, int len, bool swap=false);
		void initMemory();

		int getArraySize(char* str);
		int getElementSize(short type, short name);


		char *getName(int ind);
		char *getType(int ind);
		short *getStruct(int ind);
		short getLength(int ind);
		int getReverseType(short type);
		int getReverseType(char *type);


		int getNumStructs();

		//
		bool lessThan(bDNA* other);

		void initCmpFlags(bDNA *memDNA);
		bool flagNotEqual(int dna_nr);
		bool flagEqual(int dna_nr);
		bool flagNone(int dna_nr);


		int getPointerSize();

		void	dumpTypeDefinitions();

	
	private:
		enum FileDNAFlags
		{
			FDF_NONE=0,
			FDF_STRUCT_NEQU,
			FDF_STRUCT_EQU
		};

		void initRecurseCmpFlags(int i);

		std::vector<int>			mCMPFlags;
		std::vector<char*>			mNames;
		std::vector<char*>			mTypes;
		std::vector<short*>			mStructs;
		std::vector<short>			mTlens;
		std::map<int, int>			mStructReverse;
		std::map<bString, int>		mTypeLookup;

		int							mPtrLen;



	};
}


#endif//__BDNA_H__
