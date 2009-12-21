/*
-------------------------------------------------------------------------------
	Lua 5.1 script tests.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "blender.h"

#include "LuaEngine.h"
#include "LuaBindingUtils.h"
#include "bBlenderFile.h"

#define ModuleName "bParse"
#define ModuleVersion "249"






struct BlenderFile
{
	BlenderFile() : m_file(0)
	{
	}

	~BlenderFile()
	{

		if (m_file)
			delete m_file;
	}

	bParse::bBlenderFile *m_file;


	static int constructor(LuaState *L);
	static int destructor(LuaState *L);

	static int load(LuaState *L);
	static int getFileGlobal(LuaState *L);

	static LuaMethodDef Methods[];
	static LuaTypeDef	Type;
};


int BlenderFile::constructor(LuaState *L)
{
	BlenderFile *file;
	LuaNew(L, file, Type, BlenderFile, BlenderFile());
	return 1;
}

int BlenderFile::destructor(LuaState *L)
{
	BlenderFile *fd= LuaBind::self<BlenderFile>(L);
	assert(fd);
	if (fd != 0)
		LuaDelete(fd, BlenderFile);
	return 0;
}


int BlenderFile::load(LuaState *L)
{

	BlenderFile *fd= LuaBind::self<BlenderFile>(L);
	if (fd == 0)
		return LuaBind::seterror(L, "Invalid self");

	char *fname; bool verbose=false;
	if (!LuaBind::parse(L, "s|b", &fname, &verbose))
		return LuaBind::seterror(L, "expected string argument and optional bool");

	bParse::bBlenderFile *fp = fd->m_file;

	if (fp != 0)
		delete fd->m_file;

	FILE *sysfile = fopen(fname, "rb");
	if (!sysfile)
		return LuaBind::seterror(L, "loading failed in file '%s', no such file or directory", fname);

	fseek(sysfile, 0L, SEEK_END);
	int len = ftell(sysfile);
	fseek(sysfile, 0L, SEEK_SET);


	char *membuf = new char[len+1];
	fread(membuf, len,1, sysfile);
	fclose(sysfile);


	fp = fd->m_file = new bParse::bBlenderFile(membuf, len);

	if (!fp->ok())
	{
		delete []membuf;
		delete fd->m_file;
		fd->m_file = 0;
		return LuaBind::seterror(L, "loading failed in file '%s', invalid blend", fname);
	}

	fp->parse(verbose);
	delete []membuf;

	return 0;
}


// Method initialization
LuaMethodDef BlenderFile::Methods[]=
{
	{"load", BlenderFile::load},
	{0,0}
};

// Class initialization
LuaTypeDef BlenderFile::Type = {
	"BlenderFile",
	"File loader for the .blend format. read/write support.",
	BlenderFile::constructor,
	BlenderFile::destructor, 
	0,
	0,
	BlenderFile::Methods,
};


void LuabParse_initialize(LuaState *L)
{
	int tval= LuaBind::beginBinding(L);
	LuaBind::table(L, ModuleName);
	LuaBind::set(L, "VERSION", ModuleVersion);
	
	// types
	LuaBind::set(L, &BlenderFile::Type);

	LuaBind::table(L);
	LuaBind::endBinding(L, tval);
}
