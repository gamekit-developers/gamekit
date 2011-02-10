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
#include "OgreTexture.h"
#include "OgreTextureManager.h"
#include "OgreFreeImageCodec.h"
#include "gkTextureLoader.h"
#include "gkLogger.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "Blender.h"



gkTextureLoader::gkTextureLoader(Blender::Image* ima)
	:   m_stream(0)
{
	GK_ASSERT(ima);
	Blender::PackedFile* pack = ima->packedfile;
	if (pack)
	{
		m_stream = new utMemoryStream;
		m_stream->open(pack->data, pack->size, utStream::SM_READ);
	}
}


gkTextureLoader::~gkTextureLoader()
{
	delete m_stream;
}


void gkTextureLoader::loadResource(Ogre::Resource* resource)
{
	Ogre::Texture* texture = static_cast<Ogre::Texture*>(resource);

	if (!m_stream)
	{
		gkPrintf("Warning: Skipping image %s no packed file information is present!", texture->getName().c_str());
		return;
	}

	Ogre::DataStreamPtr stream(OGRE_NEW Ogre::MemoryDataStream(m_stream->ptr(), m_stream->size()));

	Ogre::Image ima;
	ima.load(stream);

	texture->setUsage(Ogre::TU_DEFAULT);
	texture->setTextureType(Ogre::TEX_TYPE_2D);
	texture->setNumMipmaps(gkEngine::getSingleton().getUserDefs().defaultMipMap);
	texture->setWidth(ima.getWidth());
	texture->setHeight(ima.getHeight());
	texture->setDepth(ima.getDepth());
	texture->setFormat(ima.getFormat());

	Ogre::ConstImagePtrList ptrs;
	ptrs.push_back(&ima);
	texture->_loadImages(ptrs);
}
