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
#include "gkSettings.h"

#ifdef OGREKIT_COMPILE_OGRE_SCRIPTS

#include "gkFont.h"
#include "OgreFont.h"
#include "OgreFontManager.h"


gkFont::gkFont(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:    gkResource(creator, name, handle), m_data(0)
{
}




gkFont::~gkFont()
{
	UTsize i;
	for (i = 0; i < m_loaders.size(); ++i)
		delete m_loaders[i];
	m_loaders.clear();
	delete m_data;
}


void gkFont::setData(void* data, UTsize size)
{
	if (size == 0 || size == UT_NPOS)
		return;

	if (m_data)
		delete m_data;

	m_data = new utMemoryStream();
	m_data->open(data, size, utStream::SM_READ);
}



gkFont::Loader* gkFont::addLoader(UTsize size, UTsize resolution)
{
	Loader* loader = new Loader(this, size, resolution);
	m_loaders.push_back(loader);
	return loader;
}


void gkFont::Loader::loadResource(Ogre::Resource* resource)
{
	Ogre::Font* font = (Ogre::Font*)resource;
	if (!m_parent || !m_parent->m_data || m_parent->m_data->size() == 0 || font != m_font)
		return;



	m_font->setType(Ogre::FT_TRUETYPE);
	m_font->setTrueTypeSize(Ogre::Real(m_size));
	m_font->setTrueTypeResolution(m_resolution);


	Ogre::DataStreamPtr memStream(OGRE_NEW Ogre::MemoryDataStream((void*)m_parent->m_data->ptr(), m_parent->m_data->size(), false, true));
	m_font->loadManualResource(memStream);
}


gkFont::Loader::~Loader()
{
	if (m_font)
	{
		m_font->unload();
		Ogre::FontManager::getSingleton().remove(m_font->getHandle());
	}
}


#endif
