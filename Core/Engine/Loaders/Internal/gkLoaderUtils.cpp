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
#include "gkLoaderUtils.h"
#include "gkMathUtils.h"
#include "gkBlenderDefines.h"
#include "blender.h"
#include "bBlenderFile.h"
#include "bMain.h"


using namespace Ogre;


#define BPARSE_VER 249

Ogre::Real gkLoaderUtils::blender_anim_rate = 25;


//-----------------------------------------------------------------------------
gkLoaderUtils::gkLoaderUtils(bParse::bBlenderFile *file) :
		mFile(file)
{
	GK_ASSERT(mFile); // Required
}

//-----------------------------------------------------------------------------
void gkLoaderUtils::getLayers(Blender::Mesh *mesh, Blender::MTFace **eightLayerArray, Blender::MCol **oneMCol, int &validLayers)
{
	GK_ASSERT(mesh);

	bParse::bMain *mp= mFile->getMain();
	GK_ASSERT(mp);

	validLayers= 0;

	Blender::CustomDataLayer *layers= (Blender::CustomDataLayer*)mesh->fdata.layers;
	if (layers)
	{
		// push valid layers
		for (int i= 0; i < mesh->fdata.totlayer && validLayers < 8; i++)
		{
			if (layers[i].type == CD_MTFACE && eightLayerArray)
			{
				Blender::MTFace *mtf= (Blender::MTFace *)layers[i].data;
				if (mtf)
					eightLayerArray[validLayers++]= mtf;
			}
			else if (layers[i].type == CD_MCOL)
			{
				if (oneMCol && !(*oneMCol))
					*oneMCol= static_cast<Blender::MCol*>(layers[i].data);
			}
		}
	}
	else
	{
		if (eightLayerArray && mesh->mtface)
			eightLayerArray[validLayers++]= mesh->mtface;
		if (oneMCol && mesh->mcol)
			*oneMCol= mesh->mcol;


	}
}

//-----------------------------------------------------------------------------
int gkLoaderUtils::getDeformGroupIndex(Blender::Object *ob, const char *group)
{
	Blender::bDeformGroup *dg, *fg;

	/// named group
	for (dg= (Blender::bDeformGroup*)ob->defbase.first; dg; dg= dg->next)
		if (!strcmp(dg->name, group)) break;

	if (!dg)
		return -1;


	int fidx= 0;
	for (fg= (Blender::bDeformGroup*)ob->defbase.first; fg; fg= fg->next, fidx++)
		if (fg == dg) break;

	return fidx;
}

//-----------------------------------------------------------------------------
Blender::Material* gkLoaderUtils::getMaterial(Blender::Object *ob, int index)
{
	if (!ob || ob->totcol==0) return 0;

	index= gkClamp(index, 1, ob->totcol);
	Blender::Material* ma;

	bParse::bMain *mp= mFile->getMain();

#if BPARSE_VER == 249
	/// older files
	if (ob->colbits & (1 << (index-1)))
#elif BPARSE_VER >= 250
	/// access changed to matbits
	if (ob->matbits[index-1])
#endif
		ma= (Blender::Material*)ob->mat[index-1];
	else
	{
		Blender::Mesh *me= (Blender::Mesh*)ob->data;
		ma= (Blender::Material*)me->mat[index-1];
	}
	return ma;
}



//-----------------------------------------------------------------------------
struct CurvePoint
{
	Ogre::Vector3 h1;
	Ogre::Vector3 cp;
	Ogre::Vector3 h2;
};

//-----------------------------------------------------------------------------
Ogre::Real gkLoaderUtils::evaluateIpoCurve(Blender::IpoCurve *icu, const Ogre::Real& time)
{
	if (!icu->bezt)
		return Ogre::Real(0.0);

	Ogre::Real cval= Ogre::Real(0.0);

	CurvePoint a,b;
	a= *reinterpret_cast<CurvePoint*>(&icu->bezt->vec[0][0]);
	b= *reinterpret_cast<CurvePoint*>(&(icu->bezt+(icu->totvert-1))->vec[0][0]);

	// TODO: Linear, Constant
	if (a.cp.x >= time)
	{
		cval= a.cp.y;
	}
	else if (b.cp.x <= time)
	{
		cval= b.cp.y;
	}
	else
	{
		Blender::BezTriple *ab= icu->bezt;
		Blender::BezTriple *bb= ab + 1;

		for (int i=0; i<icu->totvert; i++)
		{
			if (!ab || !bb) break;
			a= *reinterpret_cast<CurvePoint*>(&ab->vec[0][0]);
			b= *reinterpret_cast<CurvePoint*>(&bb->vec[0][0]);

			if (b.cp.x >= time && a.cp.x <= time)
			{
				if (icu->ipo == IPO_CONST)
					cval= a.cp.y;
				else
				{
					Vector2 P0, P1, P2, P3;
					P0= Vector2(a.cp.x, a.cp.y);
					P1= Vector2(a.h2.x, a.h2.y);
					P2= Vector2(b.h1.x, b.h1.y);
					P3= Vector2(b.cp.x, b.cp.y);

					Ogre::Real y;
					if (findApproximateCurve(P0, P1, P2, P3, time, y))
					{
						cval= y;
						break;
					}
				}
			}

			ab= bb;
			++bb;
		}
	}
	return cval;
}

//-----------------------------------------------------------------------------
bool gkLoaderUtils::findApproximateCurve(Vector2 &P0, Vector2 &P1, Vector2 &P2, Vector2& P3, Ogre::Real time, Ogre::Real &cval)
{
	// Cubic Bezier curves
	// B(t)= (1-t)^3 P0 + 3(1-t)^2 t P1 + 3(1-t)t^2 P2 + t^3 P3, t[0,1]

	// P0 is the start curve point
	// P3 is the end curve point
	// Step along the path to find the closest matching time.
	// false will mean try another set

	cval= Ogre::Real(0.0);
	Ogre::Real s= Ogre::Real(1.0), sc, sst;
	Ogre::Real t= Ogre::Real(0.0), tc, tts;

	//gkLoaderUtils::CorrectBezier(P0, P1, P2, P3);

	const Ogre::Real step= Ogre::Real(1.0) / blender_anim_rate;
	while (t <= 1.0)
	{
		sc=  s * s * s;
		tc=  t * t * t;
		sst= Ogre::Real(3.0) * s * s * t;
		tts= Ogre::Real(3.0) * t * t * s;

		if ((sc * P0.x + sst * P1.x + tts * P2.x + tc * P3.x) >= time)
		{
			cval= (sc * P0.y + sst * P1.y + tts * P2.y + tc * P3.y);
			return true;
		}

		t += step;
		s= Ogre::Real(1.0) - t;
	}
	return false;
}
