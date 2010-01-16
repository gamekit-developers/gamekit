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
#ifndef _OgreMaterialLoader_h_
#define _OgreMaterialLoader_h_

#include "OgreBlendConverter.h"
#include "OgreGpuProgram.h"
#include "OgreMaterial.h"
#include "OgreArray.h"

namespace Ogre {


//-----------------------------------------------------------------------------
enum TextureLayers
{
	// six layers by default in ogre, see OGRE_MAX_TEXTURE_COORD_SETS
	// blender has eight
	TL_UV0=0,
	TL_UV1,
	TL_UV2,
	TL_UV3,
	TL_UV4,
	TL_UV5,
	TL_UV6,
	TL_UV7,
};



//-----------------------------------------------------------------------------
class MaterialUtils
{
public:
	typedef Array<Blender::MTex*> MTexList;

public:

	MaterialUtils(BlendConverter *fp);
	void getOgreMaterialFromMaterial(MaterialPtr ptr, Blender::Material *matr, Blender::Mesh *me, int flags);
	void setOgreMaterialDefault(MaterialPtr ptr, bool lighting, int flags, int alpha);
	void addTextureUnit(MaterialPtr ptr, Blender::Image *ima, int layer_nr);


private:

	void handleStd(void);
	bool applyTexFace(int flags, int alpha);
	bool lampTest(void);

	Pass *getOrCreatePass(size_t idx);

	size_t getNumTextures(void);
	Blender::MTex* getTexture(size_t i);
	Blender::MTex* getTexture(int mapto, int fallback);
	MTexList getTextures(int mapto);

	int getUVLayer(Blender::MTex *te);

	TextureUnitState* addTextureUnit(Pass* ptr, Blender::MTex *tex);
	TextureUnitState* addTextureUnit(Pass* ptr, Blender::Image *ima, int layer_nr);

	BlendConverter*			m_file;
	Technique*				m_tech;
	Pass*					m_pass;
	Blender::Material*		m_blenderMat;
	Blender::Mesh*			m_blenderMesh;
	Material*				m_ogreMat;
	MTexList				m_textures;
	bool					m_multiPass;
};


}

#endif//_OgreMaterialLoader_h_
