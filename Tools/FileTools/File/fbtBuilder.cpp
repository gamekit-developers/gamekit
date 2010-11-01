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
#include "fbtConfig.h"
#include "fbtBuilder.h"
#include "fbtStreams.h"
#include "Generated/fbtScanner.inl"
#define fbtValidToken(x) (x > 0)


typedef fbtArray<FBTtype>   IntPtrArray;
typedef fbtArray<FBTtype>   TypeArray;

struct MaxAllocSize
{
	FBTuint32 m_name;
	FBTuint32 m_type;
	FBTuint32 m_tlen;
	FBTuint32 m_strc;
	FBTuint32 m_offs;
};

class fbtBuildInfo
{
public:

	fbtBuildInfo();
	~fbtBuildInfo() {}


	void reserve(void);
	int getLengths(fbtBuildStructs& struct_builders);
	int getTLengths(fbtBuildStructs& struct_builders);


	void makeBuiltinTypes(void);

	bool        hasType(const fbtId& type);
	FBTsizeType addType(const fbtId& type, const FBTuint32& len);
	FBTsizeType addName(const fbtId& name);

	MaxAllocSize        m_alloc;
	fbtStringPtrArray   m_name;
	fbtStringPtrArray   m_type;
	IntPtrArray         m_tlen;
	IntPtrArray         m_64ln;
	TypeArray           m_strc;
	fbtStringPtrArray   m_undef;
};



fbtBuilder::fbtBuilder() 
	:	m_build(new fbtBuildInfo()),
		fbt_start(0),
		m_curBuf(0),
		m_writeMode(0)
{
}


fbtBuilder::~fbtBuilder()
{
	delete m_build;
}


void fbtBuilder::makeName(fbtVariable& v, bool forceArray)
{

	fbtId newName;
	int i = 0, j = 0;

	if (v.m_isFptr)
		newName.push_back('(');

	if (v.m_ptrCount > 0)
	{
		for (i = 0; i < v.m_ptrCount; ++i)
			newName.push_back('*');
	}

	for (i = 0; i < v.m_name.size(); ++i)
		newName.push_back(v.m_name[i]);

	if (v.m_isFptr)
	{
		newName.push_back(')');
		newName.push_back('(');
		newName.push_back(')');
	}

	if (v.m_arraySize > 1 || forceArray)
	{
		for (i = 0; i < v.m_numSlots; ++i)
		{
			newName.push_back('[');

			fbtId dest;
			sprintf(dest.ptr(), "%i", v.m_arrays[i]);
			//itoa(v.m_arrays[i], dest.ptr(), 10);
			char* cp = dest.ptr();

			for (j = 0; cp[j]; ++j)
				newName.push_back(cp[j]);

			newName.push_back(']');
		}
	}

	v.m_name = newName;
}


int fbtBuilder::parseBuffer(const fbtId& name, const char* ms)
{
	fbtParser parser = fbtInitParse(name.c_str(), ms);
	fbt_includes.push_back(name.c_str());

	if (!parser)
	{
		fbtPrintf("Error : Parser initialization failed!\n");
		return -1;
	}

	int ret = doParse();
	fbtFreeParse(parser);
	return ret;
}



int fbtBuilder::parseFile(const fbtPath& id)
{
	fbtParser parser = fbtInitParse(id.c_str());
	fbt_includes.push_back(id);

	if (!parser)
	{
		fbtPrintf("Error : Parser initialization failed!\n");
		return -1;
	}

	int ret = doParse();
	fbtFreeParse(parser);
	return ret;
}

int fbtBuilder::doParse(void)
{

	fbtTokenID TOK = NULL_TOKEN;


	while (fbtValidToken(TOK))
	{
		TOK = fbtLex();

		if (TOK == NAMESPACE)
		{
			fbtToken tp;
			TOK = fbtLex(tp);
			if (TOK == IDENTIFIER)
				fbt_namespaces.push_back(tp.m_buf);
		}


		if (TOK == STRUCT)
		{
			fbtToken tp;

			do
			{
				TOK = fbtLex(tp);

				if (TOK == IDENTIFIER)
				{
					fbtBuildStruct bs;
					bs.m_name = tp.m_buf;
					bs.m_path = tp.m_src;
					bs.m_line = tp.m_line;


					TOK = fbtLex(tp);
					if (TOK == LBRACKET)
					{
						do
						{
							TOK = fbtLex(tp);

							if (TOK == RBRACKET)
								break;

							if (TOK == CLASS || TOK == STRUCT)
								TOK = fbtLex(tp);

							if (FBT_TOK_IS_TYPE(TOK))
							{
								fbtId typeId = tp.m_buf;

								// Scan names till ';'
								fbtVariable cur;
								cur.m_type          = typeId;
								cur.m_path          = tp.m_src;
								cur.m_line          = tp.m_line;
								cur.m_undefined     = 0;

								bool forceArray = false;
								bool isId = TOK == IDENTIFIER;

								do
								{

									TOK = fbtLex(tp);

									switch (TOK)
									{
									case RBRACE:
									case LBRACE:
										forceArray = true;
										break;
									case CONSTANT:
										if (cur.m_numSlots + 1 > FBT_ARRAY_SLOTS)
										{
											fbtPrintf("Maximum number of array slots exceeded!\n");
											fbtPrintf("define FBT_ARRAY_SLOTS to expand.\nCurrent = [] * %i\n", FBT_ARRAY_SLOTS);
											return -1;
										}

										cur.m_arrays[cur.m_numSlots] = tp.m_constantSize;
										cur.m_numSlots ++;
										cur.m_arraySize *= tp.m_constantSize;
										break;
									case POINTER:
										cur.m_ptrCount++;
										break;
									case IDENTIFIER:
										cur.m_name = tp.m_buf;
										break;
									case FUNCTION_POINTER_BEG:
										cur.m_isFptr = 1;
										cur.m_ptrCount++;
										cur.m_name = tp.m_buf;
										break;
									case FUNCTION_POINTER_END:
										break;
									case TERM:
									case COMMA:
										{
											makeName(cur, forceArray);

											if (isId && cur.m_ptrCount == 0)
											{
												if (bs.m_nrDependentTypes > 0)
													bs.m_nrDependentTypes = bs.m_nrDependentTypes * 2;
												else
													bs.m_nrDependentTypes ++;
												cur.m_isDependentType = true;
											}

											bs.m_data.push_back(cur);
											cur.m_ptrCount = 0;
											cur.m_arraySize = 1;

											if (TOK == COMMA)
												cur.m_numSlots = 0;
										}
										break;
									default:
										{
											fbtPrintf("%s(%i): error : Unknown character parsed! %s\n", tp.m_src, tp.m_line, tp.m_buf);
											return -1;
										}
										break;

									}
								}
								while ((TOK != TERM) && fbtValidToken(TOK));
							}
							else
							{
								fbtPrintf("%s(%i): error : unknown token parsed %s\n", tp.m_src, tp.m_line, tp.m_buf);
								return -1;
							}
						}
						while ((TOK != RBRACKET) && fbtValidToken(TOK));

						fbt_struct_builders.push_back(bs);
					}
				}
			}
			while ((TOK != RBRACKET && TOK != TERM) && fbtValidToken(TOK));
		}
	}
	return 0;
}

int fbtBuilder::buildTypes(void)
{
	return m_build->getLengths(fbt_struct_builders);
}



void fbtBuilder::writeFile(const fbtId& id, fbtStream* fp)
{
	if (!fp)
		return;

	fp->writef("unsigned char %sFBT[]={\n", id.c_str());

	m_writeMode = 0;
	writeStream(fp);


	fp->writef("\n};\n");
	fp->writef("int %sLen=sizeof(%sFBT);\n", id.c_str(), id.c_str());

}


void fbtBuilder::writeFile(const fbtId& id, const fbtPath& path)
{
	fbtFileStream fp;
	fp.open(path.c_str(), fbtStream::SM_WRITE);
	if (!fp.isOpen())
	{
		fbtPrintf("Failed to open data file: %s\n", path.c_str());
		return;
	}

	fp.writef("unsigned char %sFBT[]={\n", id.c_str());

	m_writeMode = 0;
	writeStream(&fp);

	fp.writef("\n};\n");
	fp.writef("int %sLen=sizeof(%sFBT);\n", id.c_str(), id.c_str());

#if FBT_TYPE_LEN_VALIDATE == 1
	writeValidationProgram(path.c_str());
#endif
}


void fbtBuilder::writeStream(fbtStream* fp)
{
	m_curBuf = -1;


	int i;
	writeBinPtr(fp, (void*)&fbtIdNames::FBT_SDNA[0], 4);


	writeBinPtr(fp, (void*)&fbtIdNames::FBT_NAME[0], 4);
	i = m_build->m_name.size();
#if FBT_FAKE_ENDIAN == 1
	i = fbtSwap32(i);
#endif
	writeBinPtr(fp, &i, 4);
	writeCharPtr(fp, m_build->m_name);


	writeBinPtr(fp, (void*)&fbtIdNames::FBT_TYPE[0], 4);
	i = m_build->m_type.size();
#if FBT_FAKE_ENDIAN == 1
	i = fbtSwap32(i);
#endif
	writeBinPtr(fp, &i, 4);
	writeCharPtr(fp, m_build->m_type);

	writeBinPtr(fp, (void*)&fbtIdNames::FBT_TLEN[0], 4);
#if FBT_FAKE_ENDIAN == 1
	for (i = 0; i < (int)m_build->m_tlen.size(); i++)
		m_build->m_tlen.at(i) = fbtSwap16(m_build->m_tlen.at(i));
#endif

	writeBinPtr(fp, m_build->m_tlen.ptr(), m_build->m_alloc.m_tlen);
	if ( m_build->m_tlen.size() & 1 )
	{
		char pad[2] = {'@', '@'};
		writeBinPtr(fp, (void*)&pad[0], 2);
	}

	writeBinPtr(fp, (void*)&fbtIdNames::FBT_STRC[0], 4);
	i = fbt_struct_builders.size();
#if FBT_FAKE_ENDIAN == 1
	i = fbtSwap32(i);
#endif
	writeBinPtr(fp, &i, 4);



#if FBT_FAKE_ENDIAN == 1
	for (i = 0; i < (int) m_build->m_strc.size(); i++)
		m_build->m_strc.at(i) = fbtSwap16( m_build->m_strc.at(i));
#endif

	writeBinPtr(fp,  m_build->m_strc.ptr(), m_build->m_alloc.m_strc);

}


void fbtBuilder::writeCharPtr(fbtStream* fp, const fbtStringPtrArray& ptrs)
{
	char pad[4] = {'b', 'y', 't', 'e'};
	FBTsize i = 0, s = ptrs.size();
	int t = 0;


	while (i < s)
	{
		fbtId id = ptrs[i++];
		id.push_back('\0');
		writeBinPtr(fp, (void*)id.c_str(), id.size());

		t += id.size();
	}

	int len = t;
	len = (len + 3) & ~3;

	if (len - t)
	{
		fbtId id;
		int p;
		for (p = 0; p < (len - t); p++)
			id.push_back(pad[p%4]);

		writeBinPtr(fp, (void*)id.c_str(), id.size());
	}


}


void fbtBuilder::writeBinPtr(fbtStream* fp, void* ptr, int len)
{
	if (m_writeMode == 0)
	{
		unsigned char* cb = (unsigned char*)ptr;
		for (int i = 0; i < len; ++i, ++m_curBuf)
		{
			if ((m_curBuf % 24) == (23))
				fp->writef("\n");

			unsigned char cp = cb[i];
			fp->writef("0x%02X,", cp);
		}
	}
	else
	{
		fp->write(ptr, len);
	}
}

fbtBinTables* fbtBuilder::write(void)
{
	fbtMemoryStream ms;
	ms.open(fbtStream::SM_WRITE);
	m_writeMode = -1;

	writeStream(&ms);

	void* buffer = fbtMalloc(ms.size() + 1);
	fbtMemcpy(buffer, ms.ptr(), ms.size());

	return new fbtBinTables(buffer, ms.size());
}


void fbtBuilder::writeValidationProgram(const fbtPath& path)
{
#if FBT_TYPE_LEN_VALIDATE == 1

	fbtPath string;
	fbtPathArray split;
	path.split(split, '/', '\\');

	// find split points


	int i;
	int last = 0;
	for (i = path.size()-1; i > 0; --i)
	{
		if (path[i] == '.') 
		{
			last = i;
			break;
		}
	}

	
	
	for (i = 0; i < path.size(); ++i)
	{
		if (i >= last)
			break;

		string.push_back(path[i]);
	}
	string += "Validator.cpp";



	//fbtPrintf("Writing Validator for ==> %s\n", string.c_str());

	FILE* fp = fopen(string.c_str(), "wb");
	if (!fp)
	{
		fbtPrintf("Failed to open validator file %s\n", string.c_str());
		return;
	}


	for (i = 0; i < (int)fbt_includes.size(); ++i)
	{
		split.clear(true);
		fbt_includes[i].split(split, '/', '\\');

		fprintf(fp, "#include \"%s\"\n", fbt_includes[i].c_str());
	}

	fprintf(fp, "#include <stdlib.h>\n");
	fprintf(fp, "#include <stdio.h>\n\n");
	if (!fbt_namespaces.empty())
	{
		for (i = 0; i < (int)fbt_namespaces.size(); ++i)
			fprintf(fp, "using namespace %s;\n\n\n", fbt_namespaces[i].c_str());
	}

	fprintf(fp, "int main()\n{\n\tint errors=0;\n");



	fbtBuildStructs::Iterator it = fbt_struct_builders.iterator();

	while (it.hasMoreElements())
	{
		fbtBuildStruct& bs = it.getNext();

		fbtId& cur = m_build->m_type.at(bs.m_structId);
		FBTtype len = m_build->m_tlen.at(bs.m_structId);

		if (fbt_skip.find(cur) != FBT_NPOS)
			continue;



#if FBT_FAKE_ENDIAN == 1
		len = fbtSwap16(len);
#endif
		fprintf(fp, "\t");
		fprintf(fp, "if (sizeof(%s) != %i)\n\t{\n\t\terrors ++;\n", cur.c_str(), len);
		fprintf(fp, "#ifdef _MSC_VER\n");
		fprintf(fp, "\t\tfprintf(stderr, \"%%s(%%i): error : Validation failed with ( %%i = sizeof(%s) ) != %%i\\n\", __FILE__, __LINE__, (int)sizeof(%s), %i);\n", cur.c_str(), cur.c_str(), len);
		fprintf(fp, "#else\n");
		fprintf(fp, "\t\tfprintf(stderr, \"%%s:%%i: error : Validation failed with ( %%i = sizeof(%s) ) != %%i\\n\", __FILE__, __LINE__, (int)sizeof(%s), %i);\n", cur.c_str(), cur.c_str(), len);
		fprintf(fp, "#endif\n");
		fprintf(fp, "\t}\n");
		fprintf(fp, "\n");
	}

	fprintf(fp, "\t");
	fprintf(fp, "if (errors > 0)fprintf(stderr, \"%%s(%%i): error : there are %%i misaligned types.\\n\", __FILE__, __LINE__, errors);\n");

	fprintf(fp, "\treturn errors == 0 ? 0 : 1;\n}\n");


#endif

	fclose(fp);

}





fbtBuildInfo::fbtBuildInfo()
{
	m_alloc.m_name = 0;
	m_alloc.m_type = 0;
	m_alloc.m_tlen = 0;
	m_alloc.m_strc = 0;
	m_alloc.m_offs = 0;
}


void fbtBuildInfo::reserve(void)
{
	m_name.reserve(fbtMaxTable);
	m_type.reserve(fbtMaxTable);
	m_tlen.reserve(fbtMaxTable);
	m_strc.reserve(fbtMaxTable * fbtMaxMember);
}



void fbtBuildInfo::makeBuiltinTypes(void)
{
	addType("char",         sizeof(char));
	addType("uchar",        sizeof(char));
	addType("short",        sizeof(short));
	addType("ushort",       sizeof(short));
	addType("int",          sizeof(int));
	addType("long",         sizeof(long));
	addType("ulong",        sizeof(long));
	addType("float",        sizeof(float));
	addType("double",       sizeof(double));
#ifdef FBT_USE_SCALAR
	addType(FBT_SCALAR,     sizeof(scalar_t));
#endif
	addType("void",         0);
}



FBTsizeType fbtBuildInfo::addType(const fbtId& type, const FBTuint32& len)
{
	FBTsize loc;
	if ((loc = m_type.find(type)) == FBT_NPOS)
	{
		m_alloc.m_type += type.size() + 1;
		m_alloc.m_tlen += sizeof(FBTtype);

		loc = m_type.size();

		m_type.push_back(type);
		m_tlen.push_back(len);
		m_64ln.push_back(len);
	}
	return loc;
}

bool fbtBuildInfo::hasType(const fbtId& type)
{
	return m_type.find(type) != FBT_NPOS;
}


FBTsizeType fbtBuildInfo::addName(const fbtId& name)
{
	FBTsize loc;
	if ((loc = m_name.find(name)) == FBT_NPOS)
	{
		m_alloc.m_name += name.size() + 1;
		loc = m_name.size();

		m_name.push_back(name);
	}
	return loc;
}


int fbtBuildInfo::getLengths(fbtBuildStructs& struct_builders)
{


	makeBuiltinTypes();

	fbtBuildStructs::Iterator bit = struct_builders.iterator();
	while (bit.hasMoreElements())
	{
		fbtBuildStruct& bs = bit.getNext();
		bs.m_structId = addType(bs.m_name, 0);

		m_strc.push_back(bs.m_structId);
		m_strc.push_back((FBTint16)bs.m_data.size());

		m_alloc.m_strc += (sizeof(FBTtype) * 2);


		fbtVariables::Iterator it = bs.m_data.iterator();
		while (it.hasMoreElements())
		{
			fbtVariable& cvar = it.getNext();

			cvar.m_typeId = addType(cvar.m_type, 0);
			cvar.m_nameId = addName(cvar.m_name);

			m_strc.push_back(cvar.m_typeId);
			m_strc.push_back(cvar.m_nameId);

			m_alloc.m_strc += (sizeof(FBTtype) * 2);
		}
	}

	return getTLengths(struct_builders);
}


int fbtBuildInfo::getTLengths(fbtBuildStructs& struct_builders)
{

	fbtBuildStruct* strcs = struct_builders.ptr();
	FBTsize tot = struct_builders.size(), i, e;

	int next = tot, prev = 0;

	FBTtype* tln64 = m_64ln.ptr();
	FBTtype* tlens = m_tlen.ptr();
	FBTsize nrel = 0, ct, len, fake64;

	int status = LNK_OK;

	fbtStringPtrArray m_missingReport, m_zpdef;


	FBTtype fbt_start = 0;
	if (strcs)
		fbt_start = strcs[0].m_structId;

	fbtVariable* vptr = 0;



	while (next != prev)
	{
		prev = next;
		next = 0;


		for (i = 0; i < tot; ++i)
		{
			fbtBuildStruct& cur = strcs[i];

			if (tlens[cur.m_structId] != 0)
			{
				FBTsizeType pos;
				if ((pos = m_missingReport.find(cur.m_name)) != FBT_NPOS)
					m_missingReport.erase(pos);
			}
			else
			{
				vptr = cur.m_data.ptr();
				nrel = cur.m_data.size();

				len     = 0;
				fake64  = 0;
				bool hasPtr = false;

				for (e = 0; e < nrel; ++e)
				{
					fbtVariable& v = vptr[e];
					ct = v.m_typeId;

					if (v.m_ptrCount > 0)
					{
						hasPtr = true;
						if (len % FBT_VOID)
						{
							fbtERROR( v.m_path.c_str(),
							          v.m_line, "align %i: %s %s add %i bytes\n", FBT_VOID,
							          v.m_type.c_str(), v.m_name.c_str(), FBT_VOID - (len % FBT_VOID)
							        );

							status |= LNK_ALIGNEMENTP;
						}

						if (fake64 % 8)
						{
							fbtERROR( v.m_path.c_str(),
							          v.m_line, "align %i: %s %s add %i bytes\n", 8,
							          v.m_type.c_str(), v.m_name.c_str(), 8 - (fake64 % 8)
							        );

							status |= LNK_ALIGNEMENTP;
						}

						len += FBT_VOID * v.m_arraySize;
						fake64 += 8 * v.m_arraySize;

					}
					else if (tlens[ct])
					{

						if (ct >= fbt_start)
						{
							if (FBT_VOID8 && (len % 8))
							{
								fbtERROR(v.m_path.c_str(),
								         v.m_line, "align: %i alignment error add %i bytes\n",
								         8,
								         8 - (len % 8)
								        );
								status |= LNK_ALIGNEMENTS;
							}
						}

						if (tlens[ct] > 3 && (len % 4))
						{
							fbtERROR(cur.m_path.c_str(),
							         v.m_line,
							         "align %i: in %s::%s add %i bytes\n",
							         4,
							         cur.m_name.c_str(), v.m_name.c_str(), 4 - (len % 4)
							        );
							status |= LNK_ALIGNEMENT4;
						}
						else if (tlens[ct] == 2 && (len % 2))
						{
							fbtERROR(cur.m_path.c_str(),
							         v.m_line, "align %i: in %s::%s add %i bytes\n",
							         2,
							         cur.m_name.c_str(), v.m_name.c_str(), 2 - (len % 2)
							        );
							status |= LNK_ALIGNEMENT2;
						}

						len += tlens[ct] * v.m_arraySize;
						fake64 += tln64[ct] * v.m_arraySize;
					}
					else
					{
						next ++;
						len = 0;

						if (m_missingReport.find(cur.m_name) == FBT_NPOS)
							m_missingReport.push_back(cur.m_name);

						tln64[cur.m_structId] = 0;
						tlens[cur.m_structId] = 0;

						break;
					}
				}


				tln64[cur.m_structId] = fake64;
				tlens[cur.m_structId] = len;

				if (len != 0)
				{
					if (hasPtr || fake64 != len)
					{
						if (fake64 % 8)
						{
							fbtERROR(cur.m_path.c_str(),
							         cur.m_line,
							         "64Bit alignment, in %s add %i bytes\n",
							         cur.m_name.c_str(), 8 - (fake64 % 8)
							        );
							status |= LNK_ALIGNEMENT8;
						}
					}

					if (len % 4)
					{
						fbtERROR(cur.m_path.c_str(),
						         cur.m_line,
						         "align 4: in %s add %i bytes\n",
						         cur.m_name.c_str(), 4 - (len % 4)
						        );
						status |= LNK_ALIGNEMENT4;

					}
				}
			}
		}
	}


	if (!m_missingReport.empty())
	{
		status |= LNK_UNDEFINED_TYPES;

		fbtStringPtrArray::Iterator it = m_missingReport.iterator();
		while (it.hasMoreElements())
		{
			fbtId& id = it.getNext();
			fbtPrintf("Link error: undefined reference to type '%s'\n", id.c_str());
		}
	}

	if (fbtDEBUG >= 3)
	{
		fbtBuildStructs::Iterator bit = struct_builders.iterator();

		while (bit.hasMoreElements())
		{
			fbtBuildStruct& bs = bit.getNext();

			fbtTRACE(bs.m_path.c_str(), bs.m_line, "typeid (%s):%i\n", bs.m_name.c_str(), bs.m_structId);
			if (fbtDEBUG > 0 && !bs.m_data.empty())
			{
				fbtVariables::Iterator it = bs.m_data.iterator();
				while (it.hasMoreElements())
				{
					fbtVariable& cvar = it.getNext();
					fbtTRACE(cvar.m_path.c_str(), cvar.m_line, "typeid:%-8inameid:%-8isizeof:%-8i%s %s\n",
					         cvar.m_typeId, cvar.m_nameId,
					         (cvar.m_ptrCount > 0 ? FBT_VOID : tlens[cvar.m_typeId]) * cvar.m_arraySize,
					         cvar.m_type.c_str(),
					         cvar.m_name.c_str()
					        );
				}
			}
		}
	}

	return status;
}
