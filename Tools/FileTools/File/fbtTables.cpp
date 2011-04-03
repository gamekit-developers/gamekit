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
#define FBT_IN_SOURCE
#include "fbtTables.h"
#include "fbtPlatformHeaders.h"


FBTsizeType fbtStruct::getUnlinkedMemberCount()
{
	FBTsizeType count = 0;
	for (FBTsizeType i = 0; i < m_members.size(); i++)
		if (!m_members[i].m_link) count++;

	return count;
}


fbtBinTables::fbtBinTables()
	:   m_name(0),
	    m_type(0),
	    m_tlen(0),
	    m_strc(0),
	    m_nameNr(0),
	    m_typeNr(0),
	    m_strcNr(0),
	    m_ptr(FBT_VOID),
	    m_otherBlock(0),
	    m_otherLen(0)
{
}

fbtBinTables::fbtBinTables(void* ptr, const FBTsize& len)
	:   m_name(0),
	    m_type(0),
	    m_tlen(0),
	    m_strc(0),
	    m_nameNr(0),
	    m_typeNr(0),
	    m_strcNr(0),
	    m_ptr(FBT_VOID),
	    m_otherBlock(ptr),
	    m_otherLen(len)
{
}

fbtBinTables::~fbtBinTables()
{
	fbtFree(m_name);
	fbtFree(m_type);
	fbtFree(m_tlen);
	fbtFree(m_strc);
	if (m_otherBlock)
		fbtFree(m_otherBlock);


	OffsM::Iterator it = m_offs.iterator();
	while (it.hasMoreElements())
		delete it.getNext();
}


bool fbtBinTables::read(bool swap)
{
	if (m_otherBlock && m_otherLen != 0)
		return read(m_otherBlock, m_otherLen, swap);
	return false;
}


bool fbtBinTables::read(const void* ptr, const FBTsize& len, bool swap)
{
	FBTuint32* ip = 0, i, j, k, nl;
	FBTtype* tp = 0;

	char* cp = (char*)ptr;


	if (!fbtCharNEq(cp, fbtIdNames::FBT_SDNA, FBT_MAGIC))
	{
		fbtPrintf("Bin table is missing the start id!\n");
		return false;
	}

	cp += FBT_MAGIC;


	if (!fbtCharNEq(cp, fbtIdNames::FBT_NAME, FBT_MAGIC))
	{
		fbtPrintf("Bin table is missing the name id!\n");
		return false;
	}

	cp += FBT_MAGIC;


	FBTintPtr opad;

	ip = (FBTuint32*)cp;
	nl = *ip++;
	cp = (char*)ip;

	if (swap) nl = fbtSwap32(nl);


	if (nl > fbtMaxTable)
	{
		fbtPrintf("Max name table size exceeded!\n");
		return false;
	}
	else
	{
		m_name = (Names)fbtMalloc((nl * sizeof(fbtName)) + 1);
	}



	i = 0;
	while (i < nl && i < fbtMaxTable)
	{
		fbtName name = {cp, i, fbtCharHashKey(cp).hash(), 0, 0, 0, 1};

		fbtFixedString<64> bn;

		// re-lex
		while (*cp)
		{
			switch (*cp)
			{
			default:
				{
					bn.push_back(*cp);
					++cp; break;
				}
			case ')':
			case ']':
				++cp;
				break;
			case '(':   {++cp; name.m_isFptr = 1; break;    }
			case '*':   {++cp; name.m_ptrCount ++; break;   }
			case '[':
				{
					while ((*++cp) != ']')
						name.m_slots[name.m_numSlots] = (name.m_slots[name.m_numSlots] * 10) + ((*cp) - '0');
					name.m_arraySize *= name.m_slots[name.m_numSlots++];
				}
				break;
			}
		}
		++cp;

		//fbtPrintf("%d %d: %s %s %u %u\n", m_nameNr, m_base.size(), name.m_name, bn.c_str(), name.m_nameId, bn.hash());

		m_name[m_nameNr++] = name;
		m_base.push_back(bn.hash());
		++i;
	}

	// read alignment
	opad = (FBTintPtr)cp;
	opad = ((opad + 3) & ~3) - opad;
	while (opad--) cp++;

	if (!fbtCharNEq(cp, fbtIdNames::FBT_TYPE, FBT_MAGIC))
	{
		fbtPrintf("Bin table is missing the type id!\n");
		return false;
	}

	cp += FBT_MAGIC;

	ip = (FBTuint32*)cp;
	nl = *ip++;
	cp = (char*)ip;

	if (swap) nl = fbtSwap32(nl);

	if (nl > fbtMaxTable)
	{
		fbtPrintf("Max name table size exceeded!\n");
		return false;
	}
	else
	{
		m_type = (Types)fbtMalloc((nl * sizeof(fbtType) + 1));
		m_tlen = (TypeL)fbtMalloc((nl * sizeof(FBTtype) + 1));
	}

	i = 0;
	while (i < nl)
	{
		fbtType typeData = {cp, fbtCharHashKey(cp).hash(), -1};
		m_type[m_typeNr++] = typeData;
		while (*cp) ++cp;
		++cp;
		++i;
	}

	// read alignment
	opad = (FBTintPtr)cp;
	opad = ((opad + 3) & ~3) - opad;
	while (opad--) cp++;

	if (!fbtCharNEq(cp, fbtIdNames::FBT_TLEN, FBT_MAGIC))
	{
		fbtPrintf("Bin table is missing the tlen id!\n");
		return false;
	}

	cp += FBT_MAGIC;

	tp = (FBTtype*)cp;

	i = 0;
	while (i < m_typeNr)
	{

		m_tlen[i] = *tp++;
		if (swap)
			m_tlen[i] = fbtSwap16(m_tlen[i]);
		++i;
	}

	// read alignment
	if (m_typeNr & 1) ++tp;

	cp = (char*)tp;

	if (!fbtCharNEq(cp, fbtIdNames::FBT_STRC, FBT_MAGIC))
	{
		fbtPrintf("Bin table is missing the tlen id!\n");
		return false;
	}

	cp += FBT_MAGIC;


	ip = (FBTuint32*)cp;
	nl = *ip++;
	tp = (FBTtype*)ip;

	if (swap) nl = fbtSwap32(nl);

	if (nl > fbtMaxTable)
	{
		fbtPrintf("Max name table size exceeded!\n");
		return false;
	}
	else
		m_strc = (Strcs)fbtMalloc(nl * fbtMaxMember * sizeof(FBTtype) + 1);


	m_typeFinder.reserve(m_typeNr);


	i = 0;
	while (i < nl)
	{
		m_strc[m_strcNr++] = tp;


		if (swap)
		{
			tp[0] = fbtSwap16(tp[0]);
			tp[1] = fbtSwap16(tp[1]);

			m_type[tp[0]].m_strcId = m_strcNr - 1;

			m_typeFinder.insert(m_type[tp[0]].m_name, m_type[tp[0]]);


			k = tp[1];
			FBT_ASSERT( k < fbtMaxMember );

			j = 0;
			tp += 2;

			while (j < k)
			{
				tp[0] = fbtSwap16(tp[0]);
				tp[1] = fbtSwap16(tp[1]);

				++j;
				tp += 2;
			}
		}
		else
		{
			FBT_ASSERT( tp[1] < fbtMaxMember );
			m_type[tp[0]].m_strcId = m_strcNr - 1;
			m_typeFinder.insert(m_type[tp[0]].m_name, m_type[tp[0]]);

			tp += (2 * tp[1]) + 2;
		}

		++i;
	}

	if (m_strcNr == 0)
	{
		fbtFree(m_name);
		fbtFree(m_type);
		fbtFree(m_tlen);
		fbtFree(m_strc);

		m_name = 0;
		m_type = 0;
		m_tlen = 0;
		m_strc = 0;

		return false;
	}

	compile();
	return true;
}


void fbtBinTables::compile(FBTtype i, FBTtype nr, fbtStruct* off, FBTuint32& cof, FBTuint32 depth, fbtStruct::Keys& keys)
{
	FBTuint32 e, l, a, oof, ol;
	FBTuint16 f = m_strc[0][0];

	if (i > m_strcNr)
	{
		fbtPrintf("Missing recursive type\n");
		return;
	}


	for (a = 0; a < nr; ++a)
	{
		// Only calculate offsets on recursive structs
		// This saves undeded buffers
		FBTtype* strc = m_strc[i];

		oof = cof;
		ol = m_tlen[strc[0]];

		l = strc[1];
		strc += 2;

		for (e = 0; e < l; e++, strc += 2)
		{
			if (strc[0] >= f && m_name[strc[1]].m_ptrCount == 0)
			{
				fbtKey64 k = {m_type[strc[0]].m_typeId, m_name[strc[1]].m_nameId};
				keys.push_back(k);

				compile(m_type[strc[0]].m_strcId, m_name[strc[1]].m_arraySize, off, cof, depth+1, keys);

				keys.pop_back();
			}
			else
				putMember(strc, off, a, cof, depth, keys);
		}

		if ((cof - oof) != ol)
			fbtPrintf("Build ==> invalid offset (%i)(%i:%i)\n", a, (cof - oof), ol);
	
	}
}




void fbtBinTables::compile(void)
{
	m_offs.reserve(fbtMaxTable);

	if (!m_strc || m_strcNr <= 0)
	{
		fbtPrintf("Build ==> No structurs.");
		return;
	}

	FBTuint32 i, cof = 0, depth;
	FBTuint16 f = m_strc[0][0], e, memberCount;

	fbtStruct::Keys emptyKeys;
	for (i = 0; i < m_strcNr; i++)
	{
		FBTtype* strc = m_strc[i];

		FBTtype strcType = strc[0];

		depth = 0;
		cof = 0;
		fbtStruct* off = new fbtStruct;
		off->m_key.k16[0] = strcType;
		off->m_key.k16[1] = 0;
		off->m_val.k32[0] = m_type[strcType].m_typeId;
		off->m_val.k32[1] = 0; // no name
		off->m_nr         = 0;
		off->m_dp         = 0;
		off->m_off        = cof;
		off->m_len        = m_tlen[strcType];
		off->m_strcId     = i;
		off->m_link       = 0;
		off->m_flag       = fbtStruct::CAN_LINK;

		m_offs.push_back(off);

		memberCount = strc[1];

		strc += 2;
		off->m_members.reserve(fbtMaxMember);

		for (e = 0; e < memberCount; ++e, strc += 2)
		{
			if (strc[0] >= f && m_name[strc[1]].m_ptrCount == 0) //strc[0]:member_type, strc[1]:member_name
			{
				fbtStruct::Keys keys;
				fbtKey64 k = {m_type[strc[0]].m_typeId, m_name[strc[1]].m_nameId};
				keys.push_back(k);
				compile(m_type[strc[0]].m_strcId, m_name[strc[1]].m_arraySize, off, cof, depth+1, keys);				
			}
			else
				putMember(strc, off, 0, cof, 0, emptyKeys);
		}

		if (cof != off->m_len)
		{
			off->m_flag |= fbtStruct::MISALIGNED;
			fbtPrintf("Build ==> invalid offset %s:%i:%i:%i\n", m_type[off->m_key.k16[0]].m_name, i, cof, off->m_len);
		}

	}
}

void fbtBinTables::putMember(FBTtype* cp, fbtStruct* off, FBTtype nr, FBTuint32& cof, FBTuint32 depth, fbtStruct::Keys& keys)
{
	fbtStruct nof;
	nof.m_key.k16[0] = cp[0];
	nof.m_key.k16[1] = cp[1];
	nof.m_val.k32[0] = m_type[cp[0]].m_typeId;
	nof.m_val.k32[1] = m_base[cp[1]];
	nof.m_off        = cof;
	nof.m_strcId     = off->m_strcId;
	nof.m_nr         = nr;
	nof.m_dp         = depth;
	nof.m_link       = 0;
	nof.m_flag       = fbtStruct::CAN_LINK;
	nof.m_len        = (m_name[cp[1]].m_ptrCount ? m_ptr : m_tlen[cp[0]]) * m_name[cp[1]].m_arraySize;
	nof.m_keyChain   = keys;
	off->m_members.push_back(nof);
	cof += nof.m_len;

#ifdef _DEBUG
	//fbtPrintf("%s %s\n", getStructType(off), getStructName(off));
	//fbtPrintf("\t%s %s nr:%d cof:%d depth:%d\n", getStructType(&nof), getStructName(&nof), nr, cof, depth);
#endif
}


FBTtype fbtBinTables::findTypeId(const fbtCharHashKey &cp)
{
	FBTsizeType pos = m_typeFinder.find(cp);
	if (pos != FBT_NPOS)
		return m_typeFinder.at(pos).m_strcId;
	return -1;
}

const char* fbtBinTables::getStructType(const fbtStruct* strc)
{
	
	//return strc ? m_type[strc->m_key.k16[0]].m_name : "";

	FBTuint32 k = strc ? strc->m_key.k16[0] : (FBTuint32)-1;	
	return  (k >= m_typeNr) ? "" : m_type[k].m_name;
}

const char* fbtBinTables::getStructName(const fbtStruct* strc)
{	
	FBTuint32 k = strc ? strc->m_key.k16[1] : (FBTuint32)-1;	
	return  (k >= m_nameNr) ? "" : m_name[k].m_name;
}

const char* fbtBinTables::getOwnerStructName(const fbtStruct* strc)
{
	//cp0 = mp->m_type[mp->m_strc[c->m_strcId][0]].m_name;
	FBTuint32 k = strc ? strc->m_strcId : (FBTuint32)-1;
	return (k >= m_strcNr || *m_strc[k] >= m_typeNr) ? "" : m_type[*m_strc[k]].m_name;
}