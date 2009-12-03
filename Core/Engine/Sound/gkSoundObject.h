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
#ifndef _gkSoundObject_h_
#define _gkSoundObject_h_

#include "OgrePrerequisites.h"
#include "gkGameObject.h"


// ----------------------------------------------------------------------------
class gkSoundObject;

// ----------------------------------------------------------------------------
class gkSoundReferenceObject
{
public:
	typedef std::map<Ogre::String, gkSoundObject*> SoundReferenceMap;

public:
	gkSoundReferenceObject() {}
	~gkSoundReferenceObject() {}

	bool hasSound(const Ogre::String &name);
	void addSound(const Ogre::String &name, gkSoundObject *ref);

	SoundReferenceMap getSounds();
	gkSoundObject* getSound(const Ogre::String &name);

private:
	SoundReferenceMap mSounds;
};

// ----------------------------------------------------------------------------
class gkSoundObject
{
public:
	typedef struct Properties
	{
		Ogre::Real	gain;
		Ogre::String  source;
	}Properties;

protected:
	gkGameObject *mParent;
	Properties  mBaseProps;

public:

	gkSoundObject(gkGameObject *parent);
	virtual ~gkSoundObject();
	virtual void load()= 0;
	virtual void unload()= 0;
	gkGameObject* getParent();
};



#endif//_gkSoundObject_h_
