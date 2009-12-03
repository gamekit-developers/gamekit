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
#include "OgreLight.h"
#include "gkObjectLoader.h"
#include "gkGameObject.h"
#include "gkLightObject.h"
#include "gkCameraObject.h"
#include "gkEntityObject.h"
#include "gkMathUtils.h"
#include "gkBlenderDefines.h"
#include "blender.h"

using namespace Ogre;



// ----------------------------------------------------------------------------
gkGameObjectLoader::gkGameObjectLoader(gkBlendFile *fp, Blender::Scene* sc, Blender::Object *ob) :
		mFile(fp), mObject(ob), mScene(sc)
{
	GK_ASSERT(mFile);
	GK_ASSERT(mObject);
	GK_ASSERT(mScene);
}

// ----------------------------------------------------------------------------
gkGameObjectLoader::~gkGameObjectLoader()
{
}

// ----------------------------------------------------------------------------
void gkGameObjectLoader::loadObject(gkGameObject *ob)
{
	GK_ASSERT(mFile);
	GK_ASSERT(mObject);
	GK_ASSERT(ob);


	Quaternion quat;
	Vector3 loc, scale;
	gkMathUtils::extractTransform(gkMathUtils::getFromFloat(mObject->obmat), loc, quat, scale);

	gkGameObjectProperties &props= ob->getProperties();
	props.position= loc;
	props.orientation= quat;
	props.scale= scale;

	switch (ob->getType())
	{
	case GK_CAMERA:
		setCamera(ob);
		break;
	case GK_ENTITY:
		setEntity(ob);
		break;
	case GK_LIGHT:
		setLight(ob);
		break;
	case GK_ARMATURE:
		break;
	}


	// TODO
	// setPhysicsData();
	// setConstraintData();
	// setPropertyData();
	// setLogicData();
}


// ----------------------------------------------------------------------------
void gkGameObjectLoader::setLight(gkGameObject *ob)
{
	gkLightObject *obj= static_cast<gkLightObject*>(ob);

	gkLightProperties &props= obj->getLightProperties();
	Blender::Lamp* la= static_cast<Blender::Lamp*>(mObject->data);

	props.diffuse= ColourValue(la->r, la->g, la->b);
	if (la->mode & LA_NO_DIFF)
		props.diffuse= ColourValue::Black;

	props.specular= ColourValue(la->r, la->g, la->b);
	if (la->mode & LA_NO_SPEC)
		props.specular= ColourValue::Black;

	props.power= la->energy;
	if (la->mode & LA_NEG)
		props.power= -props.power;

	props.linear= la->att1 / la->dist;
	props.constant= 1.0;
	props.quadratic= la->att2 / (la->dist * la->dist);

	props.type= Light::LT_POINT;
	if (la->type != LA_LOCAL)
		props.type= la->type == LA_SPOT ? Light::LT_SPOTLIGHT : Light::LT_DIRECTIONAL;


	props.casts= true;
	props.spot_inner= Radian(la->spotblend).valueDegrees();
	props.spot_outer= la->spotsize > 128 ? 128 : la->spotsize;
	props.falloff=   128.0 * la->spotblend;


	if (props.spot_inner > props.spot_outer)
		props.spot_inner= props.spot_outer;

#if 0
	props.extra= ENG_GET_BOOL("multipassmat") && HASCAPABILITY(RSC_VERTEX_PROGRAM) && HASCAPABILITY(RSC_FRAGMENT_PROGRAM);

	if (props.extra)
	{
		props.param[0]= la->dist;

		if (la->type == LA_SPOT)
		{
			float spotsi= la->spotsize;
			spotsi= Math::Cos(Math::PI * spotsi / 360.0);
			props.param[1]= spotsi;
			props.param[2]= (1.0 - spotsi) * la->spotblend;
		}
	}
#endif
}

// ----------------------------------------------------------------------------
void gkGameObjectLoader::setCamera(gkGameObject *ob)
{
	GK_ASSERT(ob->getType() == GK_CAMERA);

	if (mObject->data)
	{
		gkCameraObject *obj= static_cast<gkCameraObject*>(ob);

		Blender::Camera* camera= static_cast<Blender::Camera*>(mObject->data);

		gkCameraProperties &props= obj->getCameraProperties();
		props.clipend=  camera->clipend;
		props.clipstart= camera->clipsta;
		props.fov   =   Degree(camera->angle);

		// enable default camera
		if (mScene->camera == mObject)
			props.start = true;
	}
}

// ----------------------------------------------------------------------------
void gkGameObjectLoader::setEntity(gkGameObject *ob)
{
	gkEntityObject *obj= static_cast<gkEntityObject*>(ob);

	gkEntityProperties& props= obj->getEntityProperties();
	props.source= GKB_IDNAME(mObject);

#if 0
	// re-convert
	::Material *matr= Blender_object_get_material(mObject, 0);
	if (matr)
		props.casts= (matr->mode & MA_SHADBUF) != 0;
#endif

	if (mObject->parent)
	{
		Blender::Object *par= mObject->parent;
		Blender::bAction *act= par->action;
		if (!act && par->proxy_from)
			act= par->proxy_from->action;

		if (act)
			ob->_setStartPose(GKB_IDNAME(act));
	}
}
