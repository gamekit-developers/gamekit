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
#include "gkOgreSkyBoxGradient.h"
#include "OgreTexture.h"
#include "OgreTextureManager.h"
#include "OgreMaterialManager.h"
#include "OgreMaterial.h"
#include "OgreTechnique.h"
#include "OgrePass.h"


gkSkyBoxGradient::gkSkyBoxGradient(const gkSceneMaterial& material, const gkString& group)
	: m_material(material)
{

	Ogre::MaterialPtr ptr = Ogre::MaterialManager::getSingleton().create(m_material.m_name, group); //GK_BUILTIN_GROUP);

	ptr->setLightingEnabled(false);
	ptr->setReceiveShadows(false);


	Ogre::TexturePtr txptr =  Ogre::TextureManager::getSingleton().create(m_material.m_name, group, true, this); //GK_BUILTIN_GROUP
	Ogre::TextureUnitState* tus = ptr->getTechnique(0)->getPass(0)->createTextureUnitState();

	tus->setTextureName(m_material.m_name, Ogre::TEX_TYPE_CUBE_MAP);
	tus->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	tus->setTextureFiltering(Ogre::TFO_BILINEAR);
}



gkSkyBoxGradient::~gkSkyBoxGradient()
{
	Ogre::MaterialManager::getSingleton().remove(m_material.m_name);
	Ogre::TextureManager::getSingleton().remove(m_material.m_name);
}


void gkSkyBoxGradient::fill(Ogre::Image& ima, int size, const Ogre::ColourValue& v, const Ogre::PixelFormat& fmt)
{
	int x, sqs;
	unsigned int* ptr, r;

	sqs = size * size;
	unsigned char* tbuf = new unsigned char[sqs*4];

	Ogre::PixelUtil::packColour(v, fmt, &r);
	ptr = (unsigned int*)tbuf;
	x = 0;
	do
		ptr[x++] = r;
	while (x < sqs);

	Ogre::DataStreamPtr imaStream(new Ogre::MemoryDataStream(tbuf, sqs * 4));
	ima.loadRawData(imaStream, size, size, 1, fmt);
	delete []tbuf;

}


void gkSkyBoxGradient::grad(Ogre::Image& ima, int x1, int y1, int x2, int y2, int size,
                            const Ogre::ColourValue& s, const Ogre::ColourValue& e,
                            const Ogre::PixelFormat& fmt)
{
	int x, y, sqs;
	float A, B, C, C1, C2;
	unsigned int* ptr;

	x1 = gkClamp<int>(x1, 0, size); x2 = gkClamp<int>(x2, 0, size);
	y1 = gkClamp<int>(y1, 0, size); y2 = gkClamp<int>(y2, 0, size);

	A = float(x2 - x1);
	B = float(y2 - y1);
	C1 = A * x1 + B * y1;
	C2 = A * x2 + B * y2;
	sqs = size * size;


	float scale = 1.f / float(sqs);
	unsigned char* tbuf = new unsigned char[sqs*4];
	ptr = (unsigned int*)tbuf;
	Ogre::ColourValue c;

	for (x = 0; x < size; ++x)
	{
		for (y = 0; y < size; y++)
		{
			C = A * (x + 0.5f) + B * (y + 0.5f);

			if (C <= C1) c = s;
			else if (C >= C2) c = e;
			else c = (s * (C2 - C) + e * (C - C1)) / (C2 - C1);

			unsigned int dest = 0;
			Ogre::PixelUtil::packColour(c, fmt, &dest);
			(*ptr++) = dest;
		}
	}

	Ogre::DataStreamPtr stream(new Ogre::MemoryDataStream(tbuf, sqs * 4));
	ima.loadRawData(stream, size, size, 1, fmt);
	delete []tbuf;
}


void gkSkyBoxGradient::loadResource(Ogre::Resource* resource)
{
	Ogre::Texture* tex = (Ogre::Texture*)resource;

	Ogre::PixelFormat fmt = Ogre::PF_R8G8B8A8;

	tex->setFormat(fmt);
	tex->setUsage(Ogre::TU_DEFAULT);
	tex->setTextureType(Ogre::TEX_TYPE_CUBE_MAP);
	tex->setNumMipmaps(0);

	unsigned int size = 64;
	unsigned int sf = 16; // offset


	Ogre::Image zeni, hori;

	Ogre::Image zhf, zhb, zhr, zhl;

	fill(zeni, size, m_material.m_zenith, fmt);
	fill(hori, size, m_material.m_horizon, fmt);

	unsigned int ma = size - sf;
	unsigned int mi = sf;

	grad(zhf, mi, mi, ma, mi, size, m_material.m_zenith,  m_material.m_horizon, fmt);
	grad(zhb, mi, mi, ma, mi, size, m_material.m_horizon, m_material.m_zenith,  fmt);
	grad(zhr, mi, mi, mi, ma, size, m_material.m_horizon, m_material.m_zenith,  fmt);
	grad(zhl, mi, mi, mi, ma, size, m_material.m_zenith,  m_material.m_horizon, fmt);

	Ogre::ConstImagePtrList ptrs;
	// not the correct order
	ptrs.push_back(&zhr);
	ptrs.push_back(&zhl);
	ptrs.push_back(&zhf);
	ptrs.push_back(&zhb);
	ptrs.push_back(&hori);
	ptrs.push_back(&zeni);

	tex->_loadImages(ptrs);
}
