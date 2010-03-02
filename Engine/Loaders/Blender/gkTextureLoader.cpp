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
#include "blender.h"

using namespace Ogre;




gkTextureLoader::gkTextureLoader(gkBlendFile *fp, Blender::Image *ima) :
        m_file(fp), m_image(ima)
{
    GK_ASSERT(m_file);
    GK_ASSERT(m_image);
}


gkTextureLoader::~gkTextureLoader()
{
}


void gkTextureLoader::loadResource(Resource* resource)
{
    GK_ASSERT(m_file);
    GK_ASSERT(m_image);
    GK_ASSERT(resource);

    Texture *texture = static_cast<Texture*>(resource);

    // internal packed file data
    if (m_image->packedfile == 0 || m_image->packedfile->data == 0)
    {
        gkPrintf("Warning: Skipping image %s no packed file information is present!", texture->getName().c_str());
        return;
    }

    Blender::PackedFile *pack = m_image->packedfile;
    unsigned char *rawBuffer = ((unsigned char*)pack->data);
    size_t rawSize = pack->size;

    DataStreamPtr stream = DataStreamPtr(new MemoryDataStream(rawBuffer, rawSize));
    Ogre::Image ima;
    ima.load(stream);

    texture->setUsage(Ogre::TU_DEFAULT);
    texture->setTextureType(Ogre::TEX_TYPE_2D);
    texture->setNumMipmaps(5);//ima.getNumMipmaps());
    texture->setWidth(ima.getWidth());
    texture->setHeight(ima.getHeight());
    texture->setDepth(ima.getDepth());
    texture->setFormat(ima.getFormat());

    ConstImagePtrList ptrs;
    ptrs.push_back(&ima);
    texture->_loadImages(ptrs);
}
