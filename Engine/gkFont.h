/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#ifndef _gkFont_h_
#define _gkFont_h_
#include "gkSettings.h"

#ifdef OGREKIT_COMPILE_OGRE_SCRIPTS

#include "gkCommon.h"
#include "gkResource.h"
#include "utStreams.h"
#include "OgreResource.h"

///Internal font, loaded from Blender VFont objects in .ttf format.
class gkFont : public gkResource
{
public:

	class Loader : public Ogre::ManualResourceLoader
	{
	private:
		gkFont* m_parent;
		Ogre::Font* m_font;
		UTsize m_size, m_resolution;

	public:

		Loader(gkFont* parent, UTsize size, UTsize resolution)
			: m_parent(parent), m_font(0), m_size(size), m_resolution(resolution)
		{
		}
		virtual ~Loader();

		void setFont(Ogre::Font* font)  {m_font = font;}

		void loadResource(Ogre::Resource* resource);
	};

	typedef utArray<Loader*> Loaders;


public:

	gkFont(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	~gkFont();

	void setData(void* data, UTsize size);

	const void*     getData(void)    {return m_data->ptr();}
	UTsize          getSize(void)    {return m_data->size();}
	utMemoryStream*  getBuffer(void) {return m_data;}



	Loader* addLoader(UTsize size, UTsize resolution);


private:
	utMemoryStream* m_data;

	friend class Loader;
	Loaders m_loaders;
};


#endif
#endif//_gkFont_h_
