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
#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreHighLevelGpuProgram.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreGpuProgramManager.h"
#include "OgreGpuProgram.h"
#include "OgreMaterialSerializer.h"
#include "OgreMaterialManager.h"
#include "OgreTechnique.h"
#include "OgrePass.h"
#include "OgreMaterialManager.h"

#include "gkMathUtils.h"
#include "gkMeshUtils.h"
#include "gkTextureLoader.h"
#include "gkEngine.h"
#include "gkLogger.h"
#include "gkUtils.h"
#include "gkMaterialLoader.h"
#include "gkBlenderDefines.h"
#include "gkBlendFile.h"
#include "gkPath.h"
#include "gkUserDefs.h"

#include "blender.h"
#include "bBlenderFile.h"
#include "bMain.h"

// Material Framework (WIP)
#define AMBIENT_VP  "BMF/Default/AmbientPassV"
#define AMBIENT_FP  "BMF/Default/AmbientPassFOneTex"
#define LIGHT_VP    "BMF/Default/LightPassV_std"
#define LIGHT_VPN   "BMF/Default/LightPassV_norm"
#define TEXTURE_VP  "BMF/Default/TexturePassStdV"
#define TEXTURE_FP  "BMF/Default/TexturePassStdF"

using namespace Ogre;




gkMaterialUtils::gkMaterialUtils(gkBlendFile *fp) :
        m_file(fp), m_tech(0), m_pass(0), m_blendMat(0),
        m_blendMesh(0), m_ogreMat(0),
        m_multiPass(false), m_modern(false)
{
    GK_ASSERT(m_file);
}


Pass *gkMaterialUtils::getOrCreatePass(size_t idx)
{
    if (!m_ogreMat || !m_blendMat || !m_tech)
        return 0;

    size_t passc = (size_t)m_tech->getNumPasses();


    if (idx < passc)
        return m_tech->getPass((unsigned short)idx);

    if (idx > passc)
        return m_tech->getPass((unsigned short)(passc - 1));

    return m_tech->createPass();
}

bool gkMaterialUtils::lampTest(void)
{
    GK_ASSERT(m_file);
    bParse::bMain *mp = m_file->getInternalFile()->getMain();
    return mp->getLamp()->size() != 0;
}

void gkMaterialUtils::handleStd(void)
{
    if (!m_ogreMat || !m_blendMat || !m_tech)
        return;

    Pass *pass = getOrCreatePass(0);


    ColourValue diffuse = ColourValue(m_blendMat->r, m_blendMat->g, m_blendMat->b, 1.0);
    ColourValue spec    = ColourValue(m_blendMat->specr, m_blendMat->specg, m_blendMat->specb, 1.0);


    if (!(m_blendMat->mode & MA_SHLESS) && lampTest())
    {
        pass->setLightingEnabled(true);
        pass->setShininess(m_blendMat->har / 4.0);

        gkColor dm = diffuse *(m_blendMat->emit + m_blendMat->ref);
        dm.a = 1.0;
        gkColor am = diffuse *m_blendMat->amb;
        am.a = 1.0;
        gkColor sm = spec *m_blendMat->spec;
        sm.a = 1.0;

        dm.a = m_blendMat->alpha;

        pass->setAmbient(am);
        pass->setDiffuse(dm);
        pass->setSpecular(sm);

        dm = diffuse *m_blendMat->emit;
        dm.a = 1.0;

        if (m_blendMat->emit > 0)
            pass->setSelfIllumination(dm);
        else
            pass->setSelfIllumination(ColourValue::Black);
    }
    else pass->setLightingEnabled(false);


    if (m_blendMat->mode & MA_WIRE)
        pass->setPolygonMode(Ogre::PM_WIREFRAME);


    bool alpha = false;

    size_t txcnt = getNumTextures();
    for (size_t i = 0; i < txcnt; i++)
    {
        Blender::MTex *mt = getTexture(i);
        if (mt && mt->tex && mt->tex->ima)
        {
            TextureUnitState *st = addTextureUnit(pass, mt);
            if (st)
                st->setColourOperation(LBO_MODULATE);

            if (mt->mapto & MAP_ALPHA)
                alpha = true;

        }
    }

    if (alpha)
    {
        pass->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
        pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    }
}


size_t gkMaterialUtils::getNumTextures(void)
{
    if (!m_ogreMat || !m_blendMat || !m_tech)
        return 0;

#define TestInvalidPtr(x) ( ( sizeof( UTintPtr ) == 8 ) ? (((UTintPtr)(x)) & 0x7) : (((UTintPtr)(x)) & 0x3))

    if (m_textures.empty())
    {
        if (m_blendMat->mtex != 0)
        {
            for (int i = 0; i < MAX_MTEX; i++)
            {
                if (!TestInvalidPtr(m_blendMat->mtex[i]))
                {
                    Blender::MTex *mtex = m_blendMat->mtex[i];
                    if (mtex && mtex->tex != 0)
                    {
                        if (mtex->tex->type == TEX_IMAGE)
                            m_textures.push_back(mtex);
                    }
                }
            }
        }
    }

    return m_textures.size();
}


Blender::MTex *gkMaterialUtils::getTexture(size_t i)
{
    return (i < m_textures.size()) ? m_textures.at(i) : 0;
}

Blender::MTex *gkMaterialUtils::getTexture(int mapto, int fallback)
{
    for (size_t i = 0; i < m_textures.size(); i++)
    {
        Blender::MTex *mt = m_textures[i];
        if (mt && mt->tex && mt->tex->ima)
        {
            if (mt->mapto & mapto)
                return mt;
        }
    }
    if (fallback != -1)
        return getTexture(fallback);
    return 0;
}

gkMaterialUtils::MTexList gkMaterialUtils::getTextures(int mapto)
{
    MTexList ret;
    for (size_t i = 0; i < m_textures.size(); i++)
    {
        Blender::MTex *mt = m_textures[i];

        if (mt && mt->tex && mt->tex->ima)
        {
            if (mt->mapto & mapto)
                ret.push_back(mt);
        }
    }
    return ret;
}


int gkMaterialUtils::getUVLayer(Blender::MTex *te)
{
    if (!m_blendMesh || !te || !m_ogreMat || !m_blendMat || !m_tech)
        return TL_UV0;

    if (!(te->texco & TEXCO_UV))
        return TL_UV0;

    gkStringVector names;
    if (m_blendMesh->fdata.layers)
    {
        Blender::CustomDataLayer *cd = (Blender::CustomDataLayer*)m_blendMesh->fdata.layers;

        if (cd)
        {
            for (int i = 0; i < m_blendMesh->fdata.totlayer; i++)
            {
                if (cd[i].type == CD_MTFACE)
                    names.push_back(gkString(cd[i].name));
            }
        }
    }

    int layer = TL_UV0;
    for (size_t i = 0; i < names.size(); i++)
    {
        if (names[i] == te->uvname)
        {
            layer = TL_UV0 + (int)i;
            break;
        }
    }
    return layer;
}


TextureUnitState *gkMaterialUtils::addTextureUnit(Pass *ptr, Blender::MTex *tex)
{
    if (!tex) return 0;
    if (!tex->tex) return 0;

    return addTextureUnit(ptr, tex->tex->ima, getUVLayer(tex));

}


void gkMaterialUtils::addTextureUnit(Ogre::MaterialPtr ptr, Blender::Image *ima, int layer_nr)
{
    addTextureUnit(ptr->getTechnique(0)->getPass(0), ima, layer_nr);
}

TextureUnitState *gkMaterialUtils::addTextureUnit(Pass *pass, Blender::Image *ima, int layer_nr)
{
    // can only use packed file or file path
    // ImBuf is internal to blender
    gkPath p(ima->name);
    const gkString& base = p.base();

    if (!ima && !ima->packedfile && !gkUtils::isResource(base))
        return 0;


    m_file->_registerImage(ima);

    TextureUnitState *state = 0;
    if (gkUtils::isResource(base))
        state = pass->createTextureUnitState(base);
    else
        state = pass->createTextureUnitState(GKB_IDNAME(ima));

    if (state && layer_nr >= TL_UV0 && layer_nr <= TL_UV7)
        state->setTextureCoordSet(layer_nr);
    if (state)
    {
        state->setTextureAddressingMode(ima->tpageflag & IMA_CLAMP_U ? TextureUnitState::TAM_CLAMP : TextureUnitState::TAM_WRAP,
                                        ima->tpageflag & IMA_CLAMP_V ? TextureUnitState::TAM_CLAMP : TextureUnitState::TAM_WRAP,
                                        TextureUnitState::TAM_WRAP
                                       );
    }
    return state;
}


bool gkMaterialUtils::applyTexFace(int flags, int alpha)
{
    if (!m_ogreMat || !m_tech)
        return false;
    m_flags = flags;

    if (flags &TF_INVISIBLE)
    {
        m_ogreMat->setReceiveShadows(false);
        m_tech->setColourWriteEnabled(false);
        m_tech->setDepthWriteEnabled(false);
        m_tech->setLightingEnabled(false);

        // skip the rest
        return false;
    }

    if (flags & TF_LIGHT)
        m_tech->setLightingEnabled(lampTest());

    if (flags &TF_TWOSIDE)
    {
        m_tech->setCullingMode(CULL_NONE);
        m_tech->setManualCullingMode(MANUAL_CULL_NONE);
    }

    if ((alpha &TF_ALPHA) || (alpha &TF_CLIP))
    {
        m_tech->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
        m_tech->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
    }
    if (alpha &TF_ADD)
        m_tech->setSceneBlending(Ogre::SBF_ONE, Ogre::SBF_ONE);

    return true;
}


void gkMaterialUtils::getOgreMaterialFromMaterial(Ogre::MaterialPtr ptr, Blender::Material *matr, Blender::Mesh *me, int flags)
{
    if (ptr.isNull()) return;

    m_ogreMat = ptr.getPointer();
    m_blendMat = matr;
    m_blendMesh = me;
    m_tech = m_ogreMat->getTechnique(0);

    if (applyTexFace(flags, 0))
    {
        getNumTextures();
        handleStd();
    }
}


void gkMaterialUtils::setOgreMaterialDefault(Ogre::MaterialPtr ptr, bool lighting, int flags, int alpha)
{
    if (ptr.isNull()) return;

    m_ogreMat = ptr.getPointer();
    m_tech = m_ogreMat->getTechnique(0);

    ptr->setLightingEnabled(lighting && lampTest());
    ptr->setDiffuse(0.8, 0.8, 0.8, 1.0);
    ptr->setAmbient(0.4, 0.4, 0.4);
    ptr->setSelfIllumination(0,0,0);
    ptr->setSpecular(0,0,0,1);
    applyTexFace(flags, alpha);
}
