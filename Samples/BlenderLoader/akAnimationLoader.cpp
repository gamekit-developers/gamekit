/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/
	
    Copyright (c) 2006-2010 Xavier T.
	
    Contributor(s): astojilj.
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

#include "akAnimationLoader.h"

#include "akAnimationChannel.h"
#include "akAnimationClip.h"
#include "akAnimationPlayerSet.h"
#include "akAnimationEngine.h"
#include "akAnimatedObject.h"

#include "akBlenderDefines.h"

#include "akBLoader.h"

#include "../akEntity.h"

void getSplineStartEnd(Blender::BezTriple* bez, int totvert, akScalar& start, akScalar& end)
{
	start = FLT_MAX;
	end = -FLT_MAX;
	
	Blender::BezTriple* bezt = bez;
	for (int c = 0; c < totvert; c++, bezt++)
	{
		if (start > bezt->vec[1][0]) start = bezt->vec[1][0];
		if (end < bezt->vec[1][0]) end = bezt->vec[1][0];
	}
}

void getIPOStartEnd(Blender::Ipo* bipo, akScalar& start, akScalar& end)
{
	start = FLT_MAX;
	end = -FLT_MAX;
	
	akScalar tstart, tend;
	
	Blender::IpoCurve* icu = (Blender::IpoCurve*)bipo->curve.first;
	while (icu)
	{
		if (icu->bezt)
		{
			getSplineStartEnd(icu->bezt, icu->totvert, tstart, tend);
			if (start > tstart) start = tstart;
			if (end < tend) end = tend;
		}
		icu = icu->next;
	}
}

void get24ActionStartEnd(Blender::bAction* action, akScalar& start, akScalar& end)
{
	start = FLT_MAX;
	end = -FLT_MAX;
	
	akScalar tstart, tend;
	
	Blender::bActionChannel* bac = (Blender::bActionChannel*)action->chanbase.first;
	while (bac)
	{
		if (bac->ipo)
		{
			getIPOStartEnd(bac->ipo, tstart, tend);
			if (start > tstart) start = tstart;
			if (end < tend) end = tend;
		}
		bac = bac->next;
	}
}

void get25ActionStartEnd(Blender::bAction* action, akScalar& start, akScalar& end)
{
	start = FLT_MAX;
	end = -FLT_MAX;
	
	akScalar tstart, tend;
	
	Blender::FCurve* bfc = (Blender::FCurve*)action->curves.first;
	while (bfc)
	{
		getSplineStartEnd(bfc->bezt, bfc->totvert, tstart, tend);
		if (start > tstart) start = tstart;
		if (end < tend) end = tend;
		bfc = bfc->next;
	}
	
	if(start == FLT_MAX && end == -FLT_MAX)
	{
		start = 0;
		end =0;
	}
}

void ConvertSpline(Blender::BezTriple* bez, akAnimationChannel* chan, int access, int mode, int totvert, 
                   akScalar xoffset, akScalar xfactor, akScalar yoffset, akScalar yfactor)
{
	
	akAnimationCurve::BezierInterpolation interpMode;
	
	switch (mode)
	{
	case 0://BEZT_IPO_CONST:
		interpMode = akAnimationCurve::BEZ_CONSTANT;
		break;
	case 1://BEZT_IPO_LIN:
		interpMode = akAnimationCurve::BEZ_LINEAR;
		break;
	case 2://BEZT_IPO_BEZ:
		interpMode = akAnimationCurve::BEZ_CUBIC;
		break;
	default:
		return;
	}
	
	akAnimationCurve* spline = new akAnimationCurve(totvert, access, interpMode);
	
	Blender::BezTriple* bezt = bez;
	for (int c = 0; c < totvert; c++, bezt++)
	{
		akScalar time, value, handle1t, handle1v, handle2t, handle2v;
		
		handle1t = (bezt->vec[0][0] + xoffset) * xfactor;
		handle1v = (bezt->vec[0][1] + yoffset) * yfactor;
		time = (bezt->vec[1][0] + xoffset) * xfactor;
		value = (bezt->vec[1][1] + yoffset) * yfactor;
		handle2t = (bezt->vec[2][0] + xoffset) * xfactor;
		handle2v = (bezt->vec[2][1] + yoffset) * yfactor;
		
		spline->setSample(c, time, value, handle1t, handle1v, handle2t, handle2v);
	}
	
	if (spline->getNumSamples())
		chan->addCurve(spline);
	else
		delete spline;
	
}


void convertLightIpo(Blender::Ipo* bipo, akAnimationChannel* chan, akScalar start, akScalar& end, akScalar animfps)
{
	//todo
}


void convertMaterialIpo(Blender::Ipo* bipo, akAnimationChannel* chan, akScalar start, akScalar& end, akScalar animfps)
{
	//todo
}


void convertActionIpo(Blender::Ipo* bipo, akAnimationChannel* chan, akScalar start, akScalar animfps)
{
	Blender::IpoCurve* icu = (Blender::IpoCurve*)bipo->curve.first;
	while (icu)
	{
		if (icu->bezt)
		{
			akScalar yfactor = 1;
			
			int code = -1;
			switch (icu->adrcode)
			{
			case AC_QUAT_W: code = akAnimationCurve::AC_CODE_ROT_QUAT_W; break;
			case AC_QUAT_X: code = akAnimationCurve::AC_CODE_ROT_QUAT_X; break;
			case AC_QUAT_Y: code = akAnimationCurve::AC_CODE_ROT_QUAT_Y; break;
			case AC_QUAT_Z: code = akAnimationCurve::AC_CODE_ROT_QUAT_Z; break;
			case AC_EUL_X: code = akAnimationCurve::AC_CODE_ROT_EULER_X; yfactor = 10*akRPD; break;
			case AC_EUL_Y: code = akAnimationCurve::AC_CODE_ROT_EULER_Y; yfactor = 10*akRPD; break;
			case AC_EUL_Z: code = akAnimationCurve::AC_CODE_ROT_EULER_Z; yfactor = 10*akRPD; break;
			case AC_LOC_X: code = akAnimationCurve::AC_CODE_LOC_X; break;
			case AC_LOC_Y: code = akAnimationCurve::AC_CODE_LOC_Y; break;
			case AC_LOC_Z: code = akAnimationCurve::AC_CODE_LOC_Z; break;
			case AC_SIZE_X: code = akAnimationCurve::AC_CODE_SCL_X; break;
			case AC_SIZE_Y: code = akAnimationCurve::AC_CODE_SCL_Y; break;
			case AC_SIZE_Z: code = akAnimationCurve::AC_CODE_SCL_Z; break;
			}
			
			// ignore any other codes
			if (code != -1 && icu->totvert > 0)
				ConvertSpline(icu->bezt, chan, code, icu->ipo, icu->totvert, -start, 1.0f/animfps, 0, 1);
		}
		icu = icu->next;
	}
}


void convertObjectIpo(Blender::Ipo* bipo, akAnimationChannel* chan, akScalar start, akScalar& end, akScalar animfps)
{
	Blender::IpoCurve* icu = (Blender::IpoCurve*)bipo->curve.first;
	while (icu)
	{
		if (icu->bezt)
		{
			akScalar yfactor = 1;
			
			int code = -1;
			switch (icu->adrcode)
			{
			case OB_ROT_X: code = akAnimationCurve::AC_CODE_ROT_EULER_X; yfactor = 10*akRPD; break;
			case OB_ROT_Y: code = akAnimationCurve::AC_CODE_ROT_EULER_Y; yfactor = 10*akRPD; break;
			case OB_ROT_Z: code = akAnimationCurve::AC_CODE_ROT_EULER_Z; yfactor = 10*akRPD; break;
			case OB_LOC_X: code = akAnimationCurve::AC_CODE_LOC_X; break;
			case OB_LOC_Y: code = akAnimationCurve::AC_CODE_LOC_Y; break;
			case OB_LOC_Z: code = akAnimationCurve::AC_CODE_LOC_Z; break;
			case OB_SIZE_X: code = akAnimationCurve::AC_CODE_SCL_X; break;
			case OB_SIZE_Y: code = akAnimationCurve::AC_CODE_SCL_Y; break;
			case OB_SIZE_Z: code = akAnimationCurve::AC_CODE_SCL_Z; break;
			}
			
			// ignore any other codes
			if (code != -1 && icu->totvert > 0)
				ConvertSpline(icu->bezt, chan, code, icu->ipo, icu->totvert, -start, 1.0f/animfps, 0, yfactor);
		}
		icu = icu->next;
	}
}


akAnimationClip* akAnimationLoader::convertObjectIpoToAnimation(Blender::Ipo* bipo, akScalar animfps)
{
	akAnimationClip* act = new akAnimationClip();
	m_engine->addAnimationClip(AKB_IDNAME(bipo), act);
	
	akScalar start, end;
	
	if(!act)
		return 0;
	
	akAnimationChannel* chan = new akAnimationChannel(akAnimationChannel::AC_TRANSFORM, AKB_IDNAME(bipo));
	act->addChannel(chan);
	
	getIPOStartEnd(bipo, start, end);
	
	convertObjectIpo(bipo, chan, start, end, animfps);
	
	// never quaternion for blender <2.4 
	chan->setEulerRotation(true);
	
	// apply time range
	act->setLength( (end-start)/animfps);
	
	return act;
}


//gkAction* convertLampIpoToAnimation(Blender::Ipo* bipo)
//{
//      gkActionManager& amgr = gkActionManager::getSingleton();
//      gkKeyedAction* act = amgr.createKeyedAction(GKB_IDNAME(bipo));

//      if(!act)
//              return 0;

//      // min/max
//      gkVector2 range(FLT_MAX, -FLT_MAX);


//      gkLightChannel* chan = new gkLightChannel(GKB_IDNAME(bipo), act);
//      act->addChannel(chan);

//      convertLightIpo(bipo, chan, range);

//      // apply time range
//      act->setStart(range.x);
//      act->setEnd(range.y);

//      return act;
//}

void akAnimationLoader::convertAction24(Blender::bAction* action, akScalar animfps)
{
	// 2.4x actions are always Pose actions 
	akAnimationClip* act = new akAnimationClip(); 
	m_engine->addAnimationClip(AKB_IDNAME(action), act);
	
	if(!act)
		return;
	
	// min/max
	akScalar start, end;
	get24ActionStartEnd(action, start, end);
	
	Blender::bActionChannel* bac = (Blender::bActionChannel*)action->chanbase.first;
	
	while (bac)
	{
		akAnimationChannel* chan = new akAnimationChannel(akAnimationChannel::AC_BONE, bac->name);
		act->addChannel(chan);
		
		if (bac->ipo)
			convertActionIpo(bac->ipo, chan, start, animfps);
		
		bac = bac->next;
	}
	
	// apply time range
	act->setLength( (end-start)/animfps);
}


void akAnimationLoader::convertAction25(Blender::bAction* action, akScalar animfps)
{
	akAnimationClip* act = new akAnimationClip();
	m_engine->addAnimationClip(AKB_IDNAME(action), act);
	
	if(!act)
		return;
	
	// min/max
	akScalar start, end;
	get25ActionStartEnd(action, start, end);
	
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
			size_t i = rnap.rfind('\"');
			if(i!=UT_NPOS && i<rnap.length()-3)
			{
				chan_name = rnap.substr(12, i - 12);
				transform_name = rnap.substr(i + 3, rnap.length() - i + 3);
				
				chan = act->getChannel(chan_name);
				if(!chan)
				{
					chan = new akAnimationChannel(akAnimationChannel::AC_BONE, chan_name);
					act->addChannel(chan);
				}
			}
		}
		// Morph (shape key) action
		else if(rnap.substr(0, 10) == "key_blocks")
		{
			size_t i = rnap.rfind('\"');
			chan_name = rnap.substr(12, i - 12);
			transform_name = rnap.substr(i + 3, rnap.length() - i + 3);
			
			chan = act->getChannel(chan_name);
			if(!chan)
			{
				chan = new akAnimationChannel(akAnimationChannel::AC_MORPH, chan_name);
				act->addChannel(chan);
			}
		}
		// Object action
		else if( rnap == "rotation_quaternion" ||
				 rnap == "rotation_euler" ||
				 rnap == "location" ||
				 rnap == "scale")
		{
			transform_name = rnap;
			chan_name = "GKMainObjectChannel";
			
			chan = act->getChannel(chan_name);
			if(!chan)
			{
				chan = new akAnimationChannel(akAnimationChannel::AC_TRANSFORM, chan_name);
				act->addChannel(chan);
			}
		}
		
		
		if (bfc->bezt)
		{
			int code = -1;
			if (transform_name == "value")
			{
				code = akAnimationCurve::AC_CODE_VALUE;
			}
			else if (transform_name == "rotation_quaternion")
			{
				if (bfc->array_index == 0) code = akAnimationCurve::AC_CODE_ROT_QUAT_W;
				else if (bfc->array_index == 1) code = akAnimationCurve::AC_CODE_ROT_QUAT_X;
				else if (bfc->array_index == 2) code = akAnimationCurve::AC_CODE_ROT_QUAT_Y;
				else if (bfc->array_index == 3) code = akAnimationCurve::AC_CODE_ROT_QUAT_Z;
			}
			else if (transform_name == "rotation_euler")
			{
				chan->setEulerRotation(true);
				
				if (bfc->array_index == 0) code = akAnimationCurve::AC_CODE_ROT_EULER_X;
				else if (bfc->array_index == 1) code = akAnimationCurve::AC_CODE_ROT_EULER_Y;
				else if (bfc->array_index == 2) code = akAnimationCurve::AC_CODE_ROT_EULER_Z;
			}
			else if (transform_name == "location")
			{
				if (bfc->array_index == 0) code = akAnimationCurve::AC_CODE_LOC_X;
				else if (bfc->array_index == 1) code = akAnimationCurve::AC_CODE_LOC_Y;
				else if (bfc->array_index == 2) code = akAnimationCurve::AC_CODE_LOC_Z;
			}
			else if (transform_name == "scale")
			{
				if (bfc->array_index == 0) code = akAnimationCurve::AC_CODE_SCL_X;
				else if (bfc->array_index == 1) code = akAnimationCurve::AC_CODE_SCL_Y;
				else if (bfc->array_index == 2) code = akAnimationCurve::AC_CODE_SCL_Z;
			}
			
			// ignore any other codes
			if (code != -1 && bfc->totvert > 0)
				ConvertSpline(bfc->bezt, chan, code, bfc->bezt->ipo, bfc->totvert, -start, 1.0f/animfps, 0, 1);
		}
		
		bfc = bfc->next;
	}
	
	
	// apply time range
	act->setLength( (end-start)/animfps);
}


void akAnimationLoader::convert25AnimData(akEntity* obj, Blender::AnimData* adt, akScalar animfps)
{
	if(!adt)
		return;
	
	if(adt->nla_tracks.first)
	{
		// TODO 2.5x NLA
	}
	
	if(adt->action)
	{
		//gkAnimation* act =  dynamic_cast<gkAnimation*>(gkAnimationManager::getSingleton().getByName(GKB_IDNAME(adt->action)));
		akAnimationClip* clip = m_engine->getAnimationClip(AKB_IDNAME(adt->action));
		if(!clip)
		{
			convertAction25(adt->action, animfps);
			clip = m_engine->getAnimationClip(AKB_IDNAME(adt->action));
		}
		
		if(clip)
		{
			akAnimatedObject* animated = obj->getAnimatedObject();
			
			if(animated==0)
			{
				animated = new akAnimatedObject();
				m_engine->addAnimatedObject(obj->getName(), animated);
				obj->setAnimatedObject(animated);
			}
			
			akAnimationPlayer* play = animated->getAnimationPlayers()->addNewAnimationPlayer(clip);
			//play->setWeight(adt->act_influence);
		}
		
	}
}


void akAnimationLoader::convertAction(Blender::bAction* action, bool pre25compat, akScalar animfps)
{
	if(pre25compat)
		convertAction24(action, animfps);
	else
		convertAction25(action, animfps);
}


void akAnimationLoader::convertActions(fbtList actions, bool pre25compat, akScalar animfps)
{
	fbtList::Link* link = actions.first;
	while(link)
	{
		Blender::bAction* bact = (Blender::bAction*)link;
		convertAction(bact, pre25compat, animfps);
		link = link->next;
	}
}


void akAnimationLoader::convertObject(akEntity* obj, Blender::Object* bobj, bool pre25compat, akScalar animfps)
{
	// 2.4x
	if(pre25compat)
	{
		if(bobj && bobj->ipo)
		{
			//gkAnimation* act =  dynamic_cast<gkAnimation*>(gkAnimationManager::getSingleton().getByName(GKB_IDNAME(bobj->ipo)));
			akAnimationClip* clip = m_engine->getAnimationClip(AKB_IDNAME(bobj->ipo));
			
			if(!clip)
				clip = convertObjectIpoToAnimation(bobj->ipo, animfps);
			
			if(clip)
			{
				akAnimatedObject* animated = obj->getAnimatedObject();
				
				if(animated==0)
				{
					animated = new akAnimatedObject();
					m_engine->addAnimatedObject(obj->getName(), animated);
					obj->setAnimatedObject(animated);
				}
				
				animated->getAnimationPlayers()->addNewAnimationPlayer(clip);
			}
		}
		
		if(bobj && bobj->action)
		{
			//gkAnimation* act =  dynamic_cast<gkAnimation*>(gkAnimationManager::getSingleton().getByName(GKB_IDNAME(bobj->action)));
			akAnimationClip* clip = m_engine->getAnimationClip(AKB_IDNAME(bobj->action));
			
			if(!clip)
			{
				convertAction(bobj->action, pre25compat, animfps);
				clip = m_engine->getAnimationClip(AKB_IDNAME(bobj->action));
			}
			
			if(clip)
			{
				akAnimatedObject* animated = obj->getAnimatedObject();
				
				if(animated==0)
				{
					animated = new akAnimatedObject();
					m_engine->addAnimatedObject(obj->getName(), animated);
					obj->setAnimatedObject(animated);
				}
				
				animated->getAnimationPlayers()->addNewAnimationPlayer(clip);
			}
		}
		// TODO 2.4 NLA
	}
	// 2.5x
	else
	{
		convert25AnimData(obj, bobj->adt, animfps);
		
		if(bobj->parent && bobj->parent->type == OB_ARMATURE) 
			convert25AnimData(obj, bobj->parent->adt, animfps);
	}
	
	// object data
//	switch (bobj->type)
//	{
//	case OB_LAMP:       convertLamp(obj, bobj, pre25compat, animfps);        break;
//	case OB_CAMERA:     convertCamera(obj, bobj, pre25compat, animfps);      break;
//	case OB_MESH:       convertMesh(obj, bobj, pre25compat, animfps);        break;
//	case OB_ARMATURE:   convertArmature(obj, bobj, pre25compat, animfps);    break;
//	}
}


//void akAnimationLoader::convertLamp(gkGameObject* obj, Blender::Object* bobj, bool pre25compat, akScalar animfps)
//{
//        Blender::Lamp* blamp = static_cast<Blender::Lamp*>(bobj->data);

//        if(pre25compat)
//        {
////              convertLampIpoToAnimation(blamp->ipo);
//        }
//        else
//                convert25AnimData(obj, blamp->adt, animfps);
//}


//void akAnimationLoader::convertCamera(gkGameObject* obj, Blender::Object* bobj, bool pre25compat, akScalar animfps)
//{
//        Blender::Camera* bcam = static_cast<Blender::Camera*>(bobj->data);

//        if(pre25compat)
//        {
//        // TODO bcam->ipo
//        }
//        else
//                convert25AnimData(obj, bcam->adt, animfps);
//}


//void akAnimationLoader::convertMesh(gkGameObject* obj, Blender::Object* bobj, bool pre25compat, akScalar animfps)
//{
//        Blender::Mesh* bmesh = static_cast<Blender::Mesh*>(bobj->data);

//        if(pre25compat)
//        {
//                // TODO bmesh->ipo
//        }
//        else
//                convert25AnimData(obj, bmesh->adt, animfps);
//}


//void akAnimationLoader::convertArmature(gkGameObject* obj, Blender::Object* bobj, bool pre25compat, akScalar animfps)
//{
//        Blender::bArmature* barm = static_cast<Blender::bArmature*>(bobj->data);

//        // nothing to do for 2.4x
//        if(!pre25compat)
//                convert25AnimData(obj, barm->adt, animfps);
//}
