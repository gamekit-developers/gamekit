/*
-------------------------------------------------------------------------------
	Lua 5.1 script tests.
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
#include "LuaEngine.h"
#include "LuaBindingUtils.h"

#include "gkKeyCodes.h"
#include "gkCameraObject.h"
#include "gkSceneObject.h"
#include "Loaders/gkBlendFile.h"
#include "Logic/gkLogicManager.h"
#include "Logic/gkLogicTree.h"
#include "Logic/gkLogicNode.h"
#include "Logic/gkMotionNode.h"
#include "Logic/gkKeyNode.h"
#include "Logic/gkMathNode.h"
#include "Logic/gkPrintNode.h"
#include "Logic/gkAnimationNode.h"


#define ModuleName "KeyCodes"


void createExitHook(gkSceneObject *sc, gkScanCode kc)
{
	gkGameObject *ob = sc->createObject("createExitHook::Exit");

	gkLogicTree *ltree= gkLogicManager::getSingleton().create();

	gkLogicNode *q= ltree->createNode(NT_KEY);
	gkLogicNode *ex= ltree->createNode(NT_EXIT);
	q->getInputSocket(0)->setValue(true);

	gkKeyNode *kn= (gkKeyNode*)q;
	kn->setKey(kc);

	// Exit -> kc
	ex->getInputSocket(0)->link(q->getOutputSocket(1));

	ltree->solveOrder();
	ob->attachLogic(ltree);
}


gkCameraObject* createMouseLook(gkSceneObject *sc, const Ogre::Vector3 &pos, const Ogre::Vector3& ori, 
								const float rotationScale = -0.3f,
								const float translationScale = 0.5f
								)
{
	if (!sc) return 0;

	gkGameObject *y = sc->createObject("createMouseLook::y");
	gkGameObject *z = sc->createObject("createMouseLook::z");
	gkCameraObject *mcam = sc->getMainCamera();
	gkCameraObject *cam = sc->createCamera("createMouseLook::cam");

	if (mcam)
	{
		cam->getCameraProperties() = mcam->getCameraProperties();

		Ogre::Vector3 neul = gkMathUtils::getEulerFromQuat(mcam->getWorldOrientation());
		Ogre::Vector3 zeul = Ogre::Vector3(0, 0, neul.z);

		Ogre::Quaternion& zrot= z->getProperties().orientation;
		Ogre::Quaternion& crot= cam->getProperties().orientation;

		zrot = gkMathUtils::getQuatFromEuler(zeul);
		zrot.normalise();
		crot = gkMathUtils::getQuatFromEuler(ori);
		crot.normalise();

		z->getProperties().position = mcam->getWorldPosition();
	}
	else
	{
		z->getProperties().position = pos;
		cam->getProperties().orientation = gkMathUtils::getQuatFromEuler(ori);
	}

	y->setParent(z);
	cam->setParent(y);

	// zrotation 
	gkLogicTree *ztree= gkLogicManager::getSingleton().create();
	gkLogicNode *zmou = ztree->createNode(NT_MOUSE);
	gkLogicNode *zmot = ztree->createNode(NT_MOTION);
	zmou->getInputSocket(0)->setValue(rotationScale);

	zmot->getInputSocket(0)->link(zmou->getOutputSocket(0));
	zmot->getInputSocket(3)->link(zmou->getOutputSocket(1));

	// yrotation 
	gkLogicTree *ytree= gkLogicManager::getSingleton().create();
	gkLogicNode *ymou = ytree->createNode(NT_MOUSE);
	gkLogicNode *ymot = ytree->createNode(NT_MOTION);
	ymou->getInputSocket(1)->setValue(rotationScale);

	ymot->getInputSocket(0)->link(ymou->getOutputSocket(0));
	ymot->getInputSocket(1)->link(ymou->getOutputSocket(2));

	/// setup 90, -90 clamp
	gkMotionNode *node = (gkMotionNode*)ymot;
	node->setMinX(-90);
	node->setMaxX(90);


	ytree->solveOrder();
	y->attachLogic(ytree);

	// keyboard controlls
	gkKeyNode *fwd = (gkKeyNode*)ztree->createNode(NT_KEY);
	gkKeyNode *bak = (gkKeyNode*)ztree->createNode(NT_KEY);
	gkKeyNode *lft = (gkKeyNode*)ztree->createNode(NT_KEY);
	gkKeyNode *rgt = (gkKeyNode*)ztree->createNode(NT_KEY);
	fwd->getInputSocket(0)->setValue(true);
	bak->getInputSocket(0)->setValue(true);
	lft->getInputSocket(0)->setValue(true);
	rgt->getInputSocket(0)->setValue(true);


	fwd->setKey(KC_UPARROWKEY);
	bak->setKey(KC_DOWNARROWKEY);
	lft->setKey(KC_LEFTARROWKEY);
	rgt->setKey(KC_RIGHTARROWKEY);

	gkMathNode *scale1 = (gkMathNode*)ztree->createNode(NT_MATH);
	gkMathNode *scale2 = (gkMathNode*)ztree->createNode(NT_MATH);
	scale1->setFunction(MTH_MULTIPLY);
	scale2->setFunction(MTH_MULTIPLY);
	scale1->getInputSocket(1)->setValue(translationScale);
	scale2->getInputSocket(1)->setValue(translationScale);



	/// math node 
	// subtract (bak - fwd) out -> link to y movement
	gkMathNode *vecfwd = (gkMathNode*)ztree->createNode(NT_MATH);

	vecfwd->setFunction(MTH_SUBTRACT);
	vecfwd->getInputSocket(0)->link(fwd->getOutputSocket(0));
	vecfwd->getInputSocket(1)->link(bak->getOutputSocket(0));

	scale1->getInputSocket(0)->link(vecfwd->getOutputSocket(0));

	gkMotionNode *fwdmot = (gkMotionNode*)ztree->createNode(NT_MOTION);
	fwdmot->getInputSocket(2)->link(scale1->getOutputSocket(0));
	fwdmot->setMotionType(MT_TRANSLATION);

	// translate relitave to y GameObject
	fwdmot->setSpace(TRANSFORM_PARENT);
	fwdmot->setOtherObject("createMouseLook::y");


	/// math node 
	// subtract (lft - rgt) out -> link to x movement
	gkMathNode *vecstrf = (gkMathNode*)ztree->createNode(NT_MATH);

	vecstrf->setFunction(MTH_SUBTRACT);
	vecstrf->getInputSocket(0)->link(rgt->getOutputSocket(0));
	vecstrf->getInputSocket(1)->link(lft->getOutputSocket(0));

	scale2->getInputSocket(0)->link(vecstrf->getOutputSocket(0));
	fwdmot->getInputSocket(1)->link(scale2->getOutputSocket(0));

	ztree->solveOrder();
	z->attachLogic(ztree);
	cam->load();
	cam->setMainCamera(true);
	return cam;
}
