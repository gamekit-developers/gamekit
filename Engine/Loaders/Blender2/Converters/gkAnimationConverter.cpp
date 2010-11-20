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


#include "gkAnimation.h"
#include "gkAnimationManager.h"

#include "AnimKit.h"

#include "gkLight.h"

void ConvertSpline(Blender::BezTriple* bez, akAnimationChannel* chan, int access, int mode, int totvert, gkVector2& range)
{
	akBezierSpline* spline = new akBezierSpline(access);

	switch (mode)
	{
	case 0://BEZT_IPO_CONST:
		spline->setInterpolationMethod(akBezierSpline::BEZ_CONSTANT);
		break;
	case 1://BEZT_IPO_LIN:
		spline->setInterpolationMethod(akBezierSpline::BEZ_LINEAR);
		break;
	case 2://BEZT_IPO_BEZ:
		spline->setInterpolationMethod(akBezierSpline::BEZ_CUBIC);
		break;
	default:
		return;
	}


	Blender::BezTriple* bezt = bez;
	for (int c = 0; c < totvert; c++, bezt++)
	{
		akBezierVertex v;

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


void convertLightIpo(Blender::Ipo* bipo, akAnimationChannel* chan, gkVector2& range)
{
//todo
}


void convertMaterialIpo(Blender::Ipo* bipo, akAnimationChannel* chan, gkVector2& range)
{
//todo
}


void convertActionIpo(Blender::Ipo* bipo, akAnimationChannel* chan, gkVector2& range)
{
	Blender::IpoCurve* icu = (Blender::IpoCurve*)bipo->curve.first;
	while (icu)
	{
		if (icu->bezt)
		{
			int code = -1;
			switch (icu->adrcode)
			{
			case AC_QUAT_W: code = gkTransformChannel::SC_ROT_QUAT_W; break;
			case AC_QUAT_X: code = gkTransformChannel::SC_ROT_QUAT_X; break;
			case AC_QUAT_Y: code = gkTransformChannel::SC_ROT_QUAT_Y; break;
			case AC_QUAT_Z: code = gkTransformChannel::SC_ROT_QUAT_Z; break;
			case AC_EUL_X: code = gkTransformChannel::SC_ROT_EULER_X; break;
			case AC_EUL_Y: code = gkTransformChannel::SC_ROT_EULER_Y; break;
			case AC_EUL_Z: code = gkTransformChannel::SC_ROT_EULER_Z; break;
			case AC_LOC_X: code = gkTransformChannel::SC_LOC_X; break;
			case AC_LOC_Y: code = gkTransformChannel::SC_LOC_Y; break;
			case AC_LOC_Z: code = gkTransformChannel::SC_LOC_Z; break;
			case AC_SIZE_X: code = gkTransformChannel::SC_SCL_X; break;
			case AC_SIZE_Y: code = gkTransformChannel::SC_SCL_Y; break;
			case AC_SIZE_Z: code = gkTransformChannel::SC_SCL_Z; break;
			}
			
			// ignore any other codes
			if (code != -1 && icu->totvert > 0)
				ConvertSpline(icu->bezt, chan, code, icu->ipo, icu->totvert, range);
		}
		icu = icu->next;
	}
}


void convertObjectIpo(Blender::Ipo* bipo, akAnimationChannel* chan, gkVector2& range)
{
	Blender::IpoCurve* icu = (Blender::IpoCurve*)bipo->curve.first;
	while (icu)
	{
		if (icu->bezt)
		{
			int code = -1;
			switch (icu->adrcode)
			{
			case OB_ROT_X: code = gkTransformChannel::SC_ROT_EULER_X; break;
			case OB_ROT_Y: code = gkTransformChannel::SC_ROT_EULER_Y; break;
			case OB_ROT_Z: code = gkTransformChannel::SC_ROT_EULER_Z; break;
			case OB_LOC_X: code = gkTransformChannel::SC_LOC_X; break;
			case OB_LOC_Y: code = gkTransformChannel::SC_LOC_Y; break;
			case OB_LOC_Z: code = gkTransformChannel::SC_LOC_Z; break;
			case OB_SIZE_X: code = gkTransformChannel::SC_SCL_X; break;
			case OB_SIZE_Y: code = gkTransformChannel::SC_SCL_Y; break;
			case OB_SIZE_Z: code = gkTransformChannel::SC_SCL_Z; break;
			}
			
			// ignore any other codes
			if (code != -1 && icu->totvert > 0)
				ConvertSpline(icu->bezt, chan, code, icu->ipo, icu->totvert, range);
		}
		icu = icu->next;
	}
}


gkAnimation* convertObjectIpoToAnimation(Blender::Ipo* bipo)
{
	gkAnimationManager& amgr = gkAnimationManager::getSingleton();
	gkKeyedAnimation* act = amgr.createKeyedAnimation(GKB_IDNAME(bipo));
	
	if(!act)
		return 0;
	
	// min/max
	gkVector2 range(FLT_MAX, -FLT_MAX);
	
	
	gkObjectChannel* chan = new gkObjectChannel(GKB_IDNAME(bipo), act);
	act->addChannel(chan);
	
	convertObjectIpo(bipo, chan, range);
	
	// never quaternion for blender <2.4 
	chan->setEulerRotation(true);
	
	// apply time range
	act->setStart(range.x);
	act->setEnd(range.y);
	
	return act;
}


//gkAction* convertLampIpoToAnimation(Blender::Ipo* bipo)
//{
//	gkActionManager& amgr = gkActionManager::getSingleton();
//	gkKeyedAction* act = amgr.createKeyedAction(GKB_IDNAME(bipo));
	
//	if(!act)
//		return 0;
	
//	// min/max
//	gkVector2 range(FLT_MAX, -FLT_MAX);
	
	
//	gkLightChannel* chan = new gkLightChannel(GKB_IDNAME(bipo), act);
//	act->addChannel(chan);
	
//	convertLightIpo(bipo, chan, range);
	
//	// apply time range
//	act->setStart(range.x);
//	act->setEnd(range.y);
	
//	return act;
//}

void convertAction24(Blender::bAction* action)
{
	// 2.4x actions are always Pose actions 
	gkKeyedAnimation* act = gkAnimationManager::getSingleton().createKeyedAnimation(GKB_IDNAME(action));
	
	if(!act)
		return;
	
	// min/max
	gkVector2 range(FLT_MAX, -FLT_MAX);
	
	
	Blender::bActionChannel* bac = (Blender::bActionChannel*)action->chanbase.first;
	
	while (bac)
	{
		gkBoneChannel* chan = new gkBoneChannel(bac->name, act);
		act->addChannel(chan);
		
		if (bac->ipo)
			convertActionIpo(bac->ipo, chan, range);
				
		bac = bac->next;
	}
	
	// apply time range
	act->setStart(range.x);
	act->setEnd(range.y);
}


void convertAction25(Blender::bAction* action)
{
	gkKeyedAnimation* act = gkAnimationManager::getSingleton().createKeyedAnimation(GKB_IDNAME(action));
	
	if(!act)
		return;
	
	// min/max
	gkVector2 range(FLT_MAX, -FLT_MAX);
	
	
	Blender::FCurve* bfc = (Blender::FCurve*)action->curves.first;
	
	while (bfc)
	{
		utString rnap (bfc->rna_path);
		utString chan_name;
		utString transform_name;
		
		akAnimationChannel* chan;
		
		// Pose action
		if (rnap.substr(0, 10) == "pose.bones")
		{
			// TODO use regex?
			size_t i = rnap.rfind('\"');
			chan_name = rnap.substr(12, i - 12);
			transform_name = rnap.substr(i + 3, rnap.length() - i + 3);
			
			chan = act->getChannel(chan_name);
			if(!chan)
			{
				chan = new gkBoneChannel(chan_name, act);
				act->addChannel(chan);
			}
		}
		// Object action
		else
		{
			transform_name = rnap;
			chan_name = "GKMainObjectChannel";
			
			chan = act->getChannel(chan_name);
			if(!chan)
			{
				gkObjectChannel* ochan = new gkObjectChannel(chan_name, act);
				
				ochan->setEulerRotation(true);
				
				chan = static_cast<akAnimationChannel*>(ochan);
				act->addChannel(chan);
			}
		}
		
		
		if (bfc->bezt)
		{
			int code = -1;
			if (transform_name == "rotation_quaternion")
			{
				if (bfc->array_index == 0) code = gkTransformChannel::SC_ROT_QUAT_W;
				else if (bfc->array_index == 1) code = gkTransformChannel::SC_ROT_QUAT_X;
				else if (bfc->array_index == 2) code = gkTransformChannel::SC_ROT_QUAT_Y;
				else if (bfc->array_index == 3) code = gkTransformChannel::SC_ROT_QUAT_Z;
			}
			else if (transform_name == "rotation_euler")
			{
				if (bfc->array_index == 0) code = gkTransformChannel::SC_ROT_EULER_X;
				else if (bfc->array_index == 1) code = gkTransformChannel::SC_ROT_EULER_Y;
				else if (bfc->array_index == 2) code = gkTransformChannel::SC_ROT_EULER_Z;
			}
			else if (transform_name == "location")
			{
				if (bfc->array_index == 0) code = gkTransformChannel::SC_LOC_X;
				else if (bfc->array_index == 1) code = gkTransformChannel::SC_LOC_Y;
				else if (bfc->array_index == 2) code = gkTransformChannel::SC_LOC_Z;
			}
			else if (transform_name == "scale")
			{
				if (bfc->array_index == 0) code = gkTransformChannel::SC_SCL_X;
				else if (bfc->array_index == 1) code = gkTransformChannel::SC_SCL_Y;
				else if (bfc->array_index == 2) code = gkTransformChannel::SC_SCL_Z;
			}
			
			// ignore any other codes
			if (code != -1 && bfc->totvert > 0)
				ConvertSpline(bfc->bezt, chan, code, bfc->bezt->ipo, bfc->totvert, range);
		}
		
		bfc = bfc->next;
	}
	
	
	// apply time range
	act->setStart(range.x);
	act->setEnd(range.y);
}


void convert25AnimData(gkGameObject* obj, Blender::AnimData* adt)
{
	if(!adt)
		return;
	
	if(adt->nla_tracks.first)
	{
		// TODO 2.5x NLA
	}
	
	if(adt->action)
	{
		gkAnimation* act =  dynamic_cast<gkAnimation*>(gkAnimationManager::getSingleton().getByName(GKB_IDNAME(adt->action)));
		
		if(!act)
		{
			convertAction25(adt->action);
			act =  dynamic_cast<gkAnimation*>(gkAnimationManager::getSingleton().getByName(GKB_IDNAME(adt->action)));
		}
		
		if(act)
		{
			gkAnimationPlayer* play = obj->addAnimation(act, GKB_IDNAME(adt->action));
			play->setWeight(adt->act_influence);
		}
		
	}
}


void gkAnimationLoader::convertAction(Blender::bAction* action, bool pre25compat)
{
	if(pre25compat)
		convertAction24(action);
	else
		convertAction25(action);
}


void gkAnimationLoader::convertActions(bParse::bListBasePtr* actions, bool pre25compat)
{
	for (int i = 0; i < actions->size(); ++i)
	{
		Blender::bAction* bact = (Blender::bAction*)actions->at(i);
		convertAction(bact, pre25compat);
	}
}


void gkAnimationLoader::convertObject(gkGameObject* obj, Blender::Object* bobj, bool pre25compat)
{
	// 2.4x
	if(pre25compat)
	{
		if(bobj && bobj->ipo)
		{
			gkAnimation* act =  dynamic_cast<gkAnimation*>(gkAnimationManager::getSingleton().getByName(GKB_IDNAME(bobj->ipo)));
			
			if(!act)
				act = convertObjectIpoToAnimation(bobj->ipo);
			
			if(act)
				obj->addAnimation(act, GKB_IDNAME(bobj->ipo));
		}
		
		if(bobj && bobj->action)
		{
			gkAnimation* act =  dynamic_cast<gkAnimation*>(gkAnimationManager::getSingleton().getByName(GKB_IDNAME(bobj->action)));
			
			if(!act)
			{
				convertAction(bobj->action, pre25compat);
				act =  dynamic_cast<gkAnimation*>(gkAnimationManager::getSingleton().getByName(GKB_IDNAME(bobj->action)));
			}
			
			if(act)
				obj->addAnimation(act, GKB_IDNAME(bobj->action));
		}
		// TODO 2.4 NLA
	}
	// 2.5x
	else
	{
		convert25AnimData(obj, bobj->adt);
	}
	
	// object data
	switch (bobj->type)
	{
	case OB_LAMP:       convertLamp(obj, bobj, pre25compat);        break;
	case OB_CAMERA:     convertCamera(obj, bobj, pre25compat);      break;
	case OB_MESH:       convertMesh(obj, bobj, pre25compat);        break;
	case OB_ARMATURE:   convertArmature(obj, bobj, pre25compat);    break;
	}
}


void gkAnimationLoader::convertLamp(gkGameObject* obj, Blender::Object* bobj, bool pre25compat)
{
	Blender::Lamp* blamp = static_cast<Blender::Lamp*>(bobj->data);
	
	if(pre25compat)
	{
//		convertLampIpoToAnimation(blamp->ipo);
	}
	else
		convert25AnimData(obj, blamp->adt);
}


void gkAnimationLoader::convertCamera(gkGameObject* obj, Blender::Object* bobj, bool pre25compat)
{
	Blender::Camera* bcam = static_cast<Blender::Camera*>(bobj->data);
	
	if(pre25compat)
	{
	// TODO bcam->ipo
	}
	else
		convert25AnimData(obj, bcam->adt);
}


void gkAnimationLoader::convertMesh(gkGameObject* obj, Blender::Object* bobj, bool pre25compat)
{
	Blender::Mesh* bmesh = static_cast<Blender::Mesh*>(bobj->data);
	
	if(pre25compat)
	{
		// TODO bmesh->ipo
	}
	else
		convert25AnimData(obj, bmesh->adt);
}


void gkAnimationLoader::convertArmature(gkGameObject* obj, Blender::Object* bobj, bool pre25compat)
{
	Blender::bArmature* barm = static_cast<Blender::bArmature*>(bobj->data);
	
	// nothing to do for 2.4x
	if(!pre25compat)
		convert25AnimData(obj, barm->adt);
}
