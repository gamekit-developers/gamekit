/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
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
#ifndef _gkPath_h_
#define _gkPath_h_

#include "gkCommon.h"
#include "OgreString.h"
#include "OgreStringVector.h"




// ----------------------------------------------------------------------------
class gkPath
{
public:
	static const Ogre::String SEPERATOR;

public:

	gkPath();
	gkPath(const Ogre::String& file);
	gkPath(const char* file);
	~gkPath();


	const Ogre::String& getPath(void) const;
	Ogre::String getAbsPath(void) const;

	// file access
	void		getAsString(Ogre::String &dest) const;
	Ogre::String	  getAsString(void) const;
	Ogre::String	  directory(void) const;
	Ogre::String	  base(void) const;
	Ogre::String	  extension(void) const;

	// seperator '/'
	void	normalize(void) const;


	/// normalize based on platform seperator
	void	normalizePlatform(void) const;

	int	 getFileSize(void) const;
	void	append(const Ogre::String &v);

	bool isAbs(void) const;
	bool isRel(void) const;

	bool exists(void) const;
	bool isFile(void) const;
	bool isDir(void) const;

	void split(Ogre::StringVector &arr) const;

protected:

	mutable Ogre::String m_path;
};




#endif//_gkPath_h_
