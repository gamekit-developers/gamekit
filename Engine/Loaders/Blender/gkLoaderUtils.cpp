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
#include "gkLoaderUtils.h"
#include "gkMathUtils.h"
#include "gkGameObject.h"
#include "gkGameObjectGroup.h"
#include "gkBlenderDefines.h"
#include "blender.h"
#include "bBlenderFile.h"
#include "bMain.h"


using namespace Ogre;


#define BPARSE_VER 249

gkScalar gkLoaderUtils::blender_anim_rate = 25;



gkLoaderUtils::gkLoaderUtils(bParse::bBlenderFile *file) :
        m_file(file)
{
    GK_ASSERT(m_file); // Required
}


void gkLoaderUtils::getLayers(Blender::Mesh *mesh, Blender::MTFace **eightLayerArray, Blender::MCol **oneMCol, int &validLayers)
{
    GK_ASSERT(mesh);

    bParse::bMain *mp = m_file->getMain();
    GK_ASSERT(mp);

    validLayers = 0;

    Blender::CustomDataLayer *layers = (Blender::CustomDataLayer*)mesh->fdata.layers;
    if (layers)
    {
        // push valid layers
        for (int i = 0; i < mesh->fdata.totlayer && validLayers < 8; i++)
        {
            if (layers[i].type == CD_MTFACE && eightLayerArray)
            {
                Blender::MTFace *mtf = (Blender::MTFace *)layers[i].data;
                if (mtf)
                    eightLayerArray[validLayers++] = mtf;
            }
            else if (layers[i].type == CD_MCOL)
            {
                if (oneMCol && !(*oneMCol))
                    *oneMCol = static_cast<Blender::MCol*>(layers[i].data);
            }
        }
    }
    else
    {
        if (eightLayerArray && mesh->mtface)
            eightLayerArray[validLayers++] = mesh->mtface;
        if (oneMCol && mesh->mcol)
            *oneMCol = mesh->mcol;


    }
}


int gkLoaderUtils::getDeformGroupIndex(Blender::Object *ob, const char *group)
{
    Blender::bDeformGroup *dg, *fg;

    // named group
    for (dg = (Blender::bDeformGroup*)ob->defbase.first; dg; dg = dg->next)
        if (!strcmp(dg->name, group)) break;

    if (!dg)
        return -1;


    int fidx = 0;
    for (fg = (Blender::bDeformGroup*)ob->defbase.first; fg; fg = fg->next, fidx++)
        if (fg == dg) break;

    return fidx;
}


Blender::Material* gkLoaderUtils::getMaterial(Blender::Object *ob, int index)
{
    if (!ob || ob->totcol == 0) return 0;

    index = gkClamp<int>(index, 1, ob->totcol);
    Blender::Material* ma;

    bParse::bMain *mp = m_file->getMain();

#if BPARSE_VER == 249
    // older files
    if (ob->colbits & (1 << (index - 1)))
#elif BPARSE_VER >= 250
    // access changed to matbits
    if (ob->matbits[index-1])
#endif
    {
        ma = (Blender::Material*)ob->mat[index-1];
    }
    else
    {
        Blender::Mesh *me = (Blender::Mesh*)ob->data;
        ma = (Blender::Material*)me->mat[index-1];
    }
    return ma;
}

void gkLoaderUtils::extractInstanceTransform(gkGameObject *inst,Blender::Object *ob, gkVector3 &loc, gkQuaternion &quat, gkVector3 &scale)
{
    gkGameObject *own = inst->getGroupInstance()->getOwner();

    bParse::bListBasePtr *objs = m_file->getMain()->getObject();
    Blender::Object *orig=0;

    for (int i=0; i<objs->size(); ++i)
    {
        orig = (Blender::Object*)objs->at(i);
        if (GKB_IDNAME(orig)==own->getName())
            break;
        else orig = 0;
    }

    gkMatrix4 p,c;
    c = gkMathUtils::getFromFloat(ob->obmat);

    if (orig != 0)
    {
        p = gkMathUtils::getFromFloat(orig->obmat);

        if (!ob->parent)
        {
            c = p * c;
            gkMathUtils::extractTransform(c, loc, quat, scale);
        }
        else
        {
            gkMatrix4 parent = gkMathUtils::getFromFloat(ob->parent->obmat);
            c = parent.inverse() * c;
            gkMathUtils::extractTransform(c, loc, quat, scale);
        }

    }
    else
    {
        gkMathUtils::extractTransform(c, loc, quat, scale);
    }
}
