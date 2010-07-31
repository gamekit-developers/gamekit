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
#ifndef _gkMaterialLoader_h_
#define _gkMaterialLoader_h_

#include "gkLoaderCommon.h"
#include "OgreGpuProgram.h"
#include "OgreMaterial.h"





enum gkTextureLayers
{
	// six layers by default in ogre, see OGRE_MAX_TEXTURE_COORD_SETS
	// blender has eight
	TL_UV0 = 0,
	TL_UV1,
	TL_UV2,
	TL_UV3,
	TL_UV4,
	TL_UV5,
	TL_UV6,
	TL_UV7,
};




class gkMaterialUtils
{
public:
	typedef utArray<Blender::MTex *> MTexList;

public:

	gkMaterialUtils(gkBlendFile *fp);
	void getOgreMaterialFromMaterial(Ogre::MaterialPtr ptr, Blender::Material *matr, Blender::Mesh *me, int flags);
	void setOgreMaterialDefault(Ogre::MaterialPtr ptr, bool lighting, int flags, int alpha);


	void addTextureUnit(Ogre::MaterialPtr ptr, Blender::Image *ima, int layer_nr);


private:

	void handleStd(void);
	bool applyTexFace(int flags, int alpha);

	bool lampTest(void);

	Ogre::Pass *getOrCreatePass(size_t idx);

	size_t getNumTextures(void);
	Blender::MTex *getTexture(size_t i);
	Blender::MTex *getTexture(int mapto, int fallback);
	MTexList getTextures(int mapto);

	int getUVLayer(Blender::MTex *te);
	Ogre::TextureUnitState *addTextureUnit(Ogre::Pass *ptr, Blender::MTex *tex);
	Ogre::TextureUnitState *addTextureUnit(Ogre::Pass *ptr, Blender::Image *ima, int layer_nr);

	gkBlendFile            *m_file;         // current file
	Ogre::Technique        *m_tech;         // current technique
	Ogre::Pass             *m_pass;         // current pass
	Blender::Material      *m_blendMat;     // blender ptr
	Blender::Mesh          *m_blendMesh;    // owner mesh
	Ogre::Material         *m_ogreMat;      // destination material
	MTexList                m_textures;     // converted textures
	bool                    m_multiPass;    // use multi pass shaders (WIP)
	bool                    m_modern;       // multi pass supported ?
	int                     m_flags;

};




#endif//_gkMaterialLoader_h_
