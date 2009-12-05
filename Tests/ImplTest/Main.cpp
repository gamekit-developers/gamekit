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
#include "gkEngine.h"
#include "gkLogger.h"
#include "gkSceneObjectManager.h"
#include "gkSceneObject.h"
#include "gkCameraObject.h"
#include "gkMemoryTest.h"
#include "gkUserDefs.h"
#include "gkConstraint.h"
#include "gkPath.h"
#include "Loaders/gkBlendFile.h"
#include "Logic/gkLogicManager.h"
#include "Logic/gkLogicTree.h"
#include "Logic/gkLogicNode.h"
#include "Logic/gkMotionNode.h"
#include "Logic/gkKeyNode.h"
#include "Logic/gkMathNode.h"
#include "Logic/gkPrintNode.h"
#include "Logic/gkAnimationNode.h"
#include "PropertyScript/gkPropertyScript.h"


// ----------------------------------------------------------------------------
gkCameraObject* createPivotCamera(gkSceneObject* ob, const Ogre::Vector3 &root, const Ogre::Vector3 &offset, const Ogre::Vector3& ori)
{
	gkGameObject *gob= ob->createObject("createPivotCamera::Ogre::Root");
	gkCameraObject* cam= ob->createCamera("createPivotCamera::Camera");

	cam->setParent(gob);

	gob->getProperties().position= root;
	cam->getProperties().position= root + offset;
	cam->getProperties().orientation= gkMathUtils::getQuatFromEuler(ori);

	/// TODO: node sockets need enumeration values
	/// (So you don't have to count indices!)

	/// setup gamelogic
	gkLogicTree *ltree= gkLogicManager::getSingleton().create();

	gkLogicNode *mouse= ltree->createNode(NT_MOUSE);
	gkLogicNode *motion= ltree->createNode(NT_MOTION);

	/// Mouse RelX -> Motion Z
	gkLogicSocket *rx= mouse->getOutputSocket(1);
	gkLogicSocket *mz= motion->getInputSocket(3);

	/// flag update : If Motion -> Update
	motion->getInputSocket(0)->link(mouse->getOutputSocket(0));


	/// scale RelX mouse a bit ScaleX
	mouse->getInputSocket(0)->setValue(-0.3f);

	/// add Q exit hook
	gkLogicNode *q= ltree->createNode(NT_KEY);
	gkLogicNode *ex= ltree->createNode(NT_EXIT);
	q->getInputSocket(0)->setValue(true);

	gkKeyNode *kn= (gkKeyNode*)q;
	kn->setKey(KC_QKEY);

	// Exit -> QKey
	ex->getInputSocket(0)->link(q->getOutputSocket(1));

	mz->link(rx);

	/// attach it to root pivot
	gob->attachLogic(ltree);
	return cam;
}

// ----------------------------------------------------------------------------
void testAnim(gkEngine &eng)
{
	gkBlendFile *blend= eng.loadBlendFile("momo_ogreSmallAnim.blend");
	if (!blend)
		return;

	gkSceneIterator it= blend->getSceneIterator();
	if (!it.hasMoreElements())
		return;

	// TODO: load default scene from .blend
	gkSceneObject *scene= it.peekNext();
	if (!scene)
		return;

	/// create a pivot camera sys
	gkCameraObject *cam= createPivotCamera(scene, Ogre::Vector3::ZERO, Ogre::Vector3(-1, 1.5, 0), Ogre::Vector3(90, 0, 212));
	cam->setMainCamera(true);

	// load the blender data
	scene->load();

	// grab the mesh
	gkGameObject *ob= scene->getObject("MeshMomo");


	gkLogicTree *animtree= gkLogicManager::getSingleton().create();
	gkAnimationNode *anim= (gkAnimationNode*)animtree->createNode(NT_ANIM);
	anim->setFunction(gkAnimationNode::AF_LOOP);
	anim->setAnim("Momo_Run"); // the Action ID name


	gkLogicNode* mouse= animtree->createNode(NT_MOUSE);

	/// TODO: node sockets need enumeration values
	/// (So you don't have to count indices!)
	//
	/// TODO: for setting values it should be
	///	 node->setInput(index, value)
	///	 node->setOutput(index, value)

	/// Tell the Node to update
	anim->getInputSocket(0)->setValue(true);	// Update

	// 1 == Blending
	anim->getInputSocket(2)->setValue(1.f);	 // Start
	anim->getInputSocket(3)->setValue(25.f);	// End
	anim->getInputSocket(4)->setValue(25.f);	// Speed

	// link RelX -> Ogre::Animation Speed
	//anim->getInputSocket(4)->link(mouse->getOutputSocket(1));	// Speed
	ob->attachLogic(animtree);
}


// ----------------------------------------------------------------------------
void createScene(gkEngine &eng)
{
	testAnim(eng);
}

// ----------------------------------------------------------------------------
void createExitHook(gkSceneObject *sc)
{
	gkGameObject *ob = sc->createObject("createExitHook::Exit");

	gkLogicTree *ltree= gkLogicManager::getSingleton().create();

	gkLogicNode *q= ltree->createNode(NT_KEY);
	gkLogicNode *ex= ltree->createNode(NT_EXIT);
	q->getInputSocket(0)->setValue(true);

	gkKeyNode *kn= (gkKeyNode*)q;
	kn->setKey(KC_QKEY);

	// Exit -> QKey
	ex->getInputSocket(0)->link(q->getOutputSocket(1));

	ltree->solveOrder();
	ob->attachLogic(ltree);
}


// ----------------------------------------------------------------------------
gkCameraObject* createMouseLook(gkSceneObject *sc,  const Ogre::Vector3 &pos, const Ogre::Vector3& ori)
{
	gkGameObject *y = sc->createObject("createMouseLook::y");
	gkGameObject *z = sc->createObject("createMouseLook::z");

	gkCameraObject *mcam = sc->getMainCamera();

	gkCameraObject *cam = sc->createCamera("createMouseLook::cam");

	if (mcam)
	{
		cam->getCameraProperties() = mcam->getCameraProperties();


		Ogre::Vector3 neul = gkMathUtils::getEulerFromQuat(mcam->getProperties().orientation);
		neul.x = 0.0;
		neul.y = 0.0;
		z->getProperties().orientation = gkMathUtils::getQuatFromEuler(neul);
		z->getProperties().position = mcam->getProperties().position;
		cam->getProperties().orientation = gkMathUtils::getQuatFromEuler(ori);
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
	zmou->getInputSocket(0)->setValue(-0.3f);

	zmot->getInputSocket(0)->link(zmou->getOutputSocket(0));
	zmot->getInputSocket(3)->link(zmou->getOutputSocket(1));

	ztree->solveOrder();
	z->attachLogic(ztree);

	// yrotation 
	gkLogicTree *ytree= gkLogicManager::getSingleton().create();
	gkLogicNode *ymou = ytree->createNode(NT_MOUSE);
	gkLogicNode *ymot = ytree->createNode(NT_MOTION);
	ymou->getInputSocket(1)->setValue(-0.3f);

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

	const Ogre::Real scale = 0.5; 

	gkMathNode *scale1 = (gkMathNode*)ztree->createNode(NT_MATH);
	gkMathNode *scale2 = (gkMathNode*)ztree->createNode(NT_MATH);
	scale1->setFunction(MTH_MULTIPLY);
	scale2->setFunction(MTH_MULTIPLY);
	scale1->getInputSocket(1)->setValue(scale);
	scale2->getInputSocket(1)->setValue(scale);



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

	if (mcam->isLoaded())
		cam->load();
	return cam;
}

// ----------------------------------------------------------------------------
void loadBlend(gkEngine &eng, const char *fname)
{
	if (strcmp(fname, "momo_ogreSmallAnim.blend")==0)
	{
		createScene(eng);
		return;
	}

	gkBlendFile *blend= eng.loadBlendFile(fname);
	if (!blend)
		return;

	gkSceneIterator it= blend->getSceneIterator();
	if (!it.hasMoreElements())
		return;

	gkSceneObject *scene= it.peekNext();
	if (!scene)
		return;

	createExitHook(scene);

	scene->load();

	gkCameraObject *ncam = createMouseLook(scene, Ogre::Vector3(0, -7, 2), Ogre::Vector3(90, 0,0));
	ncam->setMainCamera(true);
}

// ----------------------------------------------------------------------------
void testPropertyScript(void)
{
	// TODO: for setting up testing nodes from inside blender with TX files
	// so the above hard coded code is obsolete
	gkPropertyScript ps;
	ps.parseFile("test.prop");

	gkNodeIterator it= ps.getPropertyTree().getNodeIterator();
	while (it.hasMoreElements())
	{
		gkPropertyNode *node= it.peekNext();
		gkPrintf("Parsed Node %s type= %s\n", node->getName().c_str(), node->getType().c_str());
		it.next();
	}
}

// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	TestMemory;

	try
	{
		char *fname = "clubsilo_packed.blend";
		if (argc > 1)
			fname = argv[argc-1];
		gkEngine eng;
		gkUserDefs &defs= eng.getUserDefs();

		gkPath p(argv[0]);
		if (p.isFile())
		{
			p = p.directory();
			p.append("OgreKitStartup.conf");
			defs.load(p.getPath());
		}


		defs.animspeed= Ogre::Real(25.0);
		defs.tickrate= Ogre::Real(60.0);
		defs.winsize= Ogre::Vector2(800, 600);
		defs.wintitle= "OgreKit Test (Press Q to exit) [";
		defs.wintitle += fname;
		defs.wintitle += "]";

		eng.initialize();
		loadBlend(eng, fname);
		eng.run();
	}

	catch (Ogre::Exception &e)
	{
		gkPrintf("%s\n", e.getDescription().c_str());
	}
	return 0;
}
