/*
-------------------------------------------------------------------------------
    This file is part of FBT (File Binary Tables).
    http://gamekit.googlecode.com/

    Copyright (c) 2010 Charlie C & Erwin Coumans.

-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#ifndef _fbtBuilder_h_
#define _fbtBuilder_h_

#include "fbtTypes.h"
#include "fbtTables.h"

/** \addtogroup FBT
*  @{
*/

typedef fbtFixedString<272>         fbtPath;
typedef fbtFixedString<fbtMaxID>    fbtId;
typedef int                         fbtArraySlots[FBT_ARRAY_SLOTS];
typedef void*                       fbtParser;
typedef fbtArray<fbtId>             fbtStringPtrArray;
typedef fbtArray<fbtPath>           fbtPathArray;

class fbtVariable
{
public:

	fbtVariable()
		:   m_type(), 
			m_name(), 
			m_typeId(-1),
		    m_nameId(-1), 
			m_ptrCount(0), 
			m_numSlots(0),
		    m_isFptr(0), 
			m_lstat(0), 
			m_undefined(0),
		    m_isDependentType(false), 
			m_arraySize(1),
			m_path(),
			m_line(-1)
	{
	}

	fbtId   m_type;
	fbtId   m_name;
	int     m_typeId;
	int     m_nameId;
	int     m_ptrCount;
	int     m_numSlots;
	int     m_isFptr;
	int     m_lstat;
	int     m_undefined;
	bool    m_isDependentType;


	fbtArraySlots m_arrays;
	FBTsize m_arraySize;

	fbtPath         m_path;
	FBTsize         m_line;



};

typedef fbtArray<fbtVariable> fbtVariables;


class fbtBuildStruct
{
public:

	fbtBuildStruct()
		:   m_structId(-1),
		    m_line(-1),
		    m_nrDependentTypes(0)
	{
	}

	FBTsize         m_structId;
	fbtId           m_name;
	fbtVariables    m_data;
	FBTsize         m_nrDependentTypes;
	fbtPath         m_path;
	FBTsize         m_line;

};

typedef fbtArray<fbtBuildStruct> fbtBuildStructs;



enum fbtLinkerIssues
{

	LNK_OK              = 0,
	LNK_ASSERT          = (1 << 0),
	LNK_ALIGNEMENT2     = (1 << 1),
	LNK_ALIGNEMENT4     = (1 << 2),
	LNK_ALIGNEMENT8     = (1 << 3),
	LNK_ALIGNEMENTS     = (1 << 4),
	LNK_ALIGNEMENTP     = (1 << 5),
	LNK_UNKNOWN         = (1 << 6),
	LNK_UNDEFINED_TYPES = (1 << 7),
	LNK_DUPLICATE_TYPES = (1 << 8)
};


class fbtBuilder
{
public:

	fbtBuilder();
	~fbtBuilder();

	int parseFile(const fbtPath& id);
	int parseBuffer(const fbtId& name, const char* ms);

	int buildTypes(void);

	void writeFile(const fbtId& id, class fbtStream* fp);
	void writeFile(const fbtId& id, const fbtPath& path);
	void writeStream(class fbtStream* fp);

	fbtBinTables* write(void);

private:

	int doParse(void);
	class fbtBuildInfo* m_build;

	void writeBinPtr(fbtStream* fp, void* ptr, int len);
	void writeCharPtr(fbtStream* fp, const fbtStringPtrArray& ptrs);

	void writeValidationProgram(const fbtPath& path);


	void makeName(fbtVariable&, bool);

	FBTsize           fbt_start;
	fbtPathArray      fbt_includes;
	fbtStringPtrArray fbt_namespaces, fbt_skip;

	fbtBuildStructs fbt_struct_builders;
	int m_curBuf, m_writeMode;

};

/** @}*/

#endif//_fbtBuilder_h_
