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
#include "gkAnimationConverter.h"
#include "gkSkeletonResource.h"
#include "gkSkeletonManager.h"
#include "gkBlenderDefines.h"
#include "gkLoaderCommon.h"

#include "gkAction.h"
#include "gkActionChannel.h"
#include "gkBezierSpline.h"
#include "gkObjectAction.h"


void ConvertSpline(Blender::BezTriple* bez, gkAnimationChannel* chan, int access, int mode, int totvert, gkVector2& range)
{
	gkBezierSpline* spline = new gkBezierSpline(access);

	switch (mode)
	{
	case 0://BEZT_IPO_CONST:
		spline->setInterpolationMethod(gkBezierSpline::BEZ_CONSTANT);
		break;
	case 1://BEZT_IPO_LIN:
		spline->setInterpolationMethod(gkBezierSpline::BEZ_LINEAR);
		break;
	case 2://BEZT_IPO_BEZ:
		spline->setInterpolationMethod(gkBezierSpline::BEZ_CUBIC);
		break;
	default:
		return;
	}


	Blender::BezTriple* bezt = bez;
	for (int c = 0; c < totvert; c++, bezt++)
	{
		gkBezierVertex v;

		v.h1[0] = bezt->vec[0][0];
		v.h1[1] = bezt->vec[0][1];
		v.cp[0] = bezt->vec[1][0];
		v.cp[1] = bezt->vec[1][1];
		v.h2[0] = bezt->vec[2][0];
		v.h2[1] = bezt->vec[2][1];


		// calculate global time
		if (range.x > v.cp[0]) range.x = v.cp[0];
		if (range.y < v.cp[0]) range.y = v.cp[0];
		spline->addVertex(v);
	}

	if (spline->getNumVerts())
		chan->addSpline(spline);
	else
		delete spline;

}


void gkAnimationLoader::convertGameObject(bParse::bListBasePtr* actions, class gkGameObject* obj, bool pre25compat, Blender::Object* bobj)
{
	if(pre25compat)
	{
		// for older files
		if(bobj && bobj->ipo)
		{
			Blender::Ipo* bipo = bobj->ipo;
			
			gkAction* act = obj->createAction(GKB_IDNAME(bipo));
			
			// min/max
			gkVector2 range(FLT_MAX, -FLT_MAX);
			
			Blender::IpoCurve* icu = (Blender::IpoCurve*)bipo->curve.first;
			
			while (icu)
			{
				// one object chanel per action
				gkGameObjectChannel* gochan = act->getObjectChannel();
				if (!gochan)
				{
					gochan = new gkGameObjectChannel(act, obj);
					act->setObjectChannel(gochan);
				}
				
				if (icu->bezt)
				{
					int code = -1;
					switch (icu->adrcode)
					{
					case OB_ROT_X:  { code = SC_ROT_EULER_X;  break; }
					case OB_ROT_Y:  { code = SC_ROT_EULER_Y;  break; }
					case OB_ROT_Z:  { code = SC_ROT_EULER_Z;  break; }
					case OB_LOC_X:  { code = SC_LOC_X;  break; }
					case OB_LOC_Y:  { code = SC_LOC_Y;  break; }
					case OB_LOC_Z:  { code = SC_LOC_Z;  break; }
					case OB_SIZE_X: { code = SC_SCL_X;  break; }
					case OB_SIZE_Y: { code = SC_SCL_Y;  break; }
					case OB_SIZE_Z: { code = SC_SCL_Z;  break; }
					}
					
					// ignore any other codes
					if (code != -1 && icu->totvert > 0)
						ConvertSpline(icu->bezt, gochan, code, icu->ipo, icu->totvert, range);
				}
				icu = icu->next;
			}
			
			// never quaternion for blender <2.4 
			act->getObjectChannel()->setEulerRotation(true);
			
			// apply time range
			act->setStart(range.x);
			act->setEnd(range.y);
		}
	}
	else
	{
		// 250 + files
		
		for (int i = 0; i < actions->size(); ++i)
		{
			Blender::bAction* bact = (Blender::bAction*)actions->at(i);
	
			// find ownership
			Blender::bActionChannel* bac = (Blender::bActionChannel*)bact->chanbase.first;
	
			if (obj->hasAction(GKB_IDNAME(bact)))
				continue;
	
			gkAction* act = obj->createAction(GKB_IDNAME(bact));
			
			// min/max
			gkVector2 range(FLT_MAX, -FLT_MAX);
			
			Blender::FCurve* bfc = (Blender::FCurve*)bact->curves.first;
			
			while (bfc)
			{
				utString transform_name(bfc->rna_path);
				
				// one object chanel per action
				gkGameObjectChannel* gochan = act->getObjectChannel();
				if (!gochan)
				{
					gochan = new gkGameObjectChannel(act, obj);
					act->setObjectChannel(gochan);
				}
				
				if (bfc->bezt)
				{
					int code = -1;
					if (transform_name == "rotation_quaternion")
					{
						if (bfc->array_index == 0) code = SC_ROT_W;
						else if (bfc->array_index == 1) code = SC_ROT_X;
						else if (bfc->array_index == 2) code = SC_ROT_Y;
						else if (bfc->array_index == 3) code = SC_ROT_Z;
					}
					else if (transform_name == "rotation_euler")
					{
						if (bfc->array_index == 0) code = SC_ROT_EULER_X;
						else if (bfc->array_index == 1) code = SC_ROT_EULER_Y;
						else if (bfc->array_index == 2) code = SC_ROT_EULER_Z;
					}
					else if (transform_name == "location")
					{
						if (bfc->array_index == 0) code = SC_LOC_X;
						else if (bfc->array_index == 1) code = SC_LOC_Y;
						else if (bfc->array_index == 2) code = SC_LOC_Z;
					}
					else if (transform_name == "scale")
					{
						if (bfc->array_index == 0) code = SC_SCL_X;
						else if (bfc->array_index == 1) code = SC_SCL_Y;
						else if (bfc->array_index == 2) code = SC_SCL_Z;
					}
					
					// ignore any other codes
					if (code != -1 && bfc->totvert > 0)
						ConvertSpline(bfc->bezt, gochan, code, bfc->bezt->ipo, bfc->totvert, range);
					
				}
				bfc = bfc->next;
			}
			
			if(bobj && bobj->rotmode == 1 && act->getObjectChannel()) // ROT_MODE_EUL == 1 in blender
				act->getObjectChannel()->setEulerRotation(true);
			
			// apply time range
			act->setStart(range.x);
			act->setEnd(range.y);
	
		}
	}
}



void gkAnimationLoader::convertSkeleton(bParse::bListBasePtr* actions, gkSkeletonResource* skel, bool pre25compat)
{
	for (int i = 0; i < actions->size(); ++i)
	{
		Blender::bAction* bact = (Blender::bAction*)actions->at(i);

		// find ownership
		Blender::bActionChannel* bac = (Blender::bActionChannel*)bact->chanbase.first;


		if (skel->hasAction(GKB_IDNAME(bact)))
			continue;

		gkAction* act = skel->createAction(GKB_IDNAME(bact));

		// min/max
		gkVector2 range(FLT_MAX, -FLT_MAX);

		if (bac && pre25compat)
		{
			// for older files
			while (bac)
			{
				if (skel->hasBone(bac->name))
				{
					gkBone* bone = skel->getBone(bac->name);

					gkActionChannel* achan = new gkActionChannel(act, bone);
					act->addChannel(achan);

					if (bac->ipo)
					{
						Blender::IpoCurve* icu = (Blender::IpoCurve*)bac->ipo->curve.first;
						while (icu)
						{
							if (icu->bezt)
							{
								int code = -1;
								switch (icu->adrcode)
								{
								case AC_QUAT_W: { code = SC_ROT_W;  break; }
								case AC_QUAT_X: { code = SC_ROT_X;  break; }
								case AC_QUAT_Y: { code = SC_ROT_Y;  break; }
								case AC_QUAT_Z: { code = SC_ROT_Z;  break; }
								case AC_LOC_X:  { code = SC_LOC_X;  break; }
								case AC_LOC_Y:  { code = SC_LOC_Y;  break; }
								case AC_LOC_Z:  { code = SC_LOC_Z;  break; }
								case AC_SIZE_X: { code = SC_SCL_X;  break; }
								case AC_SIZE_Y: { code = SC_SCL_Y;  break; }
								case AC_SIZE_Z: { code = SC_SCL_Z;  break; }
								}

								// ignore any other codes
								if (code != -1 && icu->totvert > 0)
									ConvertSpline(icu->bezt, achan, code, icu->ipo, icu->totvert, range);
							}
							icu = icu->next;
						}
					}
				}
				bac = bac->next;
			}
		}
		else
		{
			// 250 + files

			Blender::FCurve* bfc = (Blender::FCurve*)bact->curves.first;

			while (bfc)
			{
				utString rnap (bfc->rna_path);
				utString bone_name;
				utString transform_name;

				if (rnap.substr(0, 10) == "pose.bones")
				{
					// TODO use regex?
					size_t i = rnap.rfind('\"');
					bone_name = rnap.substr(12, i - 12);
					transform_name = rnap.substr(i + 3, rnap.length() - i + 3);
				}

				if (skel->hasBone(bone_name))
				{
					gkBone* bone = skel->getBone(bone_name);

					// add one chanel per bone only
					gkActionChannel* achan = act->getChannel(bone);
					if (!achan)
					{
						achan = new gkActionChannel(act, bone);
						act->addChannel(achan);
					}

					if (bfc->bezt)
					{
						int code = -1;
						if (transform_name == "rotation_quaternion")
						{
							if (bfc->array_index == 0) code = SC_ROT_W;
							else if (bfc->array_index == 1) code = SC_ROT_X;
							else if (bfc->array_index == 2) code = SC_ROT_Y;
							else if (bfc->array_index == 3) code = SC_ROT_Z;
						}
						else if (transform_name == "location")
						{
							if (bfc->array_index == 0) code = SC_LOC_X;
							else if (bfc->array_index == 1) code = SC_LOC_Y;
							else if (bfc->array_index == 2) code = SC_LOC_Z;
						}
						else if (transform_name == "scale")
						{
							if (bfc->array_index == 0) code = SC_SCL_X;
							else if (bfc->array_index == 1) code = SC_SCL_Y;
							else if (bfc->array_index == 2) code = SC_SCL_Z;
						}

						// ignore any other codes
						if (code != -1 && bfc->totvert > 0)
							ConvertSpline(bfc->bezt, achan, code, bfc->bezt->ipo, bfc->totvert, range);
					}
				}
				bfc = bfc->next;
			}
		}

		// apply time range
		act->setStart(range.x);
		act->setEnd(range.y);
	}
}
