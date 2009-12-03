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
#ifndef _gkDebugPage_h_
#define _gkDebugPage_h_

#include "gkCommon.h"
#include "OgreString.h"
#include "OgreStringVector.h"
#include "OgreIteratorWrappers.h"
#include "OgreStdHeaders.h"




// ---------------------------------------------------------------------------
class gkVariable;


// ---------------------------------------------------------------------------
class gkDebugPage
{
public:
	typedef std::vector<gkVariable*> PropertyList;
	typedef Ogre::VectorIterator<PropertyList> PropertyIterator;

public:

	gkDebugPage();
	~gkDebugPage();

	void initialize();
	void show(bool doit);

	bool isShown() {return mShow;}
	void addVariable(gkVariable *prop);
	void removeVariable(gkVariable *prop);
	bool hasVariable(gkVariable *prop);
	void clearProps();

	void draw();

private:
	bool mInitialized, mShow;
	Ogre::Overlay *mOver;
	Ogre::OverlayElement *mElement;
	Ogre::Real mCharHeight;
	PropertyList mProps;
};





#endif//_gkDebugPage_h_
