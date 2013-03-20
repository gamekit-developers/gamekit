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

#ifndef FILEINTERFACEOGRE3D_H
#define FILEINTERFACEOGRE3D_H

#include <Rocket/Core/String.h>
#include <Rocket/Core/FileInterface.h>


class FileInterfaceOgre3D : public Rocket::Core::FileInterface
{
public:
	FileInterfaceOgre3D();
	virtual ~FileInterfaceOgre3D();

	/// Opens a file.		
	virtual Rocket::Core::FileHandle Open(const Rocket::Core::String& path);

	/// Closes a previously opened file.		
	virtual void Close(Rocket::Core::FileHandle file);

	/// Reads data from a previously opened file.		
	virtual size_t Read(void* buffer, size_t size, Rocket::Core::FileHandle file);

	/// Seeks to a point in a previously opened file.		
	virtual bool Seek(Rocket::Core::FileHandle file, long offset, int origin);

	/// Returns the current position of the file pointer.		
	virtual size_t Tell(Rocket::Core::FileHandle file);
};

#endif //FILEINTERFACEOGRE3D_H
