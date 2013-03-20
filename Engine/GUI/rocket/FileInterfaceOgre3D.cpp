/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 harkon.kr

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

#include "Ogre.h"
#include "FileInterfaceOgre3D.h"

using namespace Ogre;

FileInterfaceOgre3D::FileInterfaceOgre3D()
{
}

FileInterfaceOgre3D::~FileInterfaceOgre3D()
{
}

Rocket::Core::FileHandle FileInterfaceOgre3D::Open(const Rocket::Core::String& path)
{
	DataStreamPtr stream = ResourceGroupManager::getSingleton().openResource(path.CString());
	if (stream.isNull())
		return 0;	

	return (Rocket::Core::FileHandle)(new DataStreamPtr(stream));
}

void FileInterfaceOgre3D::Close(Rocket::Core::FileHandle file)
{
	if (!file) return;

	DataStreamPtr* pstream = (DataStreamPtr*)(file);
	delete pstream;
}

size_t FileInterfaceOgre3D::Read(void* buffer, size_t size, Rocket::Core::FileHandle file)
{
	if (!file) return 0;

	DataStreamPtr stream = *(DataStreamPtr*)(file);	
	return stream->read(buffer, size);	
}

bool FileInterfaceOgre3D::Seek(Rocket::Core::FileHandle file, long offset, int origin)
{
	if (!file) return false;

	DataStreamPtr stream = *(DataStreamPtr*)(file);	
	long pos = 0;
	size_t size = stream->size();
	if (origin == SEEK_CUR)
		pos = stream->tell() + offset;
	else if (origin == SEEK_END)
		pos = size + offset;
	else
		pos = offset;

	if (pos < 0 || pos > (long)size)
		return false;

	stream->seek((size_t)pos);
	return true;
}

size_t FileInterfaceOgre3D::Tell(Rocket::Core::FileHandle file)
{
	if (!file) return 0;

	DataStreamPtr stream = *(DataStreamPtr*)(file);	

	return stream->tell();
}
