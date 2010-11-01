/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr

    Contributor(s): none yet.
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


#include "StdAfx.h"
#include "Lib/liUtils.h"
#include "liBlendFile.h"
#include "Loaders/Blender2/gkLoaderCommon.h"
#include "bBlenderFile.h"


#undef rad2 //WIN32 macro
#include "bMain.h"
#include "Blender.h"
#include "utStreams.h"

liBlendFile::liBlendFile(bParse::bBlenderFile* file)
	: m_file(file)
{
}

liBlendFile::~liBlendFile()
{
	SAFE_DELETE(m_file);
}

int liBlendFile::getObjectNames(liStrVec& names)
{
	if (!m_file) return 0;

	GK_ASSERT(m_file->getMain());

	bParse::bListBasePtr* iter = m_file->getMain()->getObject();

	for (int i = 0; i < iter->size(); i++)
	{
		Blender::Object* ob = (Blender::Object*)iter->at(i);
		names.push_back(GKB_IDNAME(ob));
	}

	return names.size();
}

liBlendLoader::liBlendLoader()
{
}

liBlendLoader::~liBlendLoader()
{
	unloadAll();
}

bool liBlendLoader::load(const gkString& fileName, bool forceReload)
{
	liBlendFile* file = getBlendFile(fileName);
	if (file)
	{
		if (forceReload)
			unload(fileName);
		else
			return true;

		file = NULL;
	}

	utMemoryStream fs;
	fs.open(fileName.c_str(), utStream::SM_READ);

	if (!fs.isOpen())
		return false;

	utMemoryStream buffer(utStream::SM_WRITE);
	fs.inflate(buffer);

	bParse::bBlenderFile* bfile = new bParse::bBlenderFile((char*)buffer.ptr(), buffer.size());
	bfile->parse(false);

	if (!bfile->ok() || !bfile->getMain())
	{
		SAFE_DELETE(bfile);
		return false;
	}

	m_files.insert(BlendFileList::value_type(fileName, new liBlendFile(bfile)));

	return true;
}

void liBlendLoader::unload(const gkString& fileName)
{
	delete_it(m_files, fileName);
}

void liBlendLoader::unloadAll()
{
	delete_all(m_files);
}

liBlendFile* liBlendLoader::getBlendFile(const gkString& fileName)
{
	if (m_files.find(fileName) == m_files.end())
		return NULL;

	return m_files[fileName];
}

UT_IMPLEMENT_SINGLETON(liBlendLoader);
