/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#include "akDemo.h"

#include "../BlenderLoader/akBLoader.h"
#include "../akEntity.h"

#include "akMesh.h"
#include "akSkeletonPose.h"
#include "akGeometryDeformer.h"
#include "akAnimationPlayerSet.h"

akDemo::akDemo() : akDemoBase()
{

}

akDemo::~akDemo()
{
	
}


void akDemo::init(void)
{
	akBLoader loader(this);
	//loader.loadFile("Blu.blend", false, true);
	//loader.loadFile("Sintel.blend", false, true);
	loader.loadFile("Test3.blend", false, true);
	
	// Set some animation data the loader cannot detect
	akEntity* square = getEntity("Plane");
	if(square)
		square->setPositionAnimated(true);
	
	// Join the morph action and the rest action together.
	akAnimationClip* bluc = getAnimation("Rest");
	akAnimationClip* morphc = getAnimation("KeyAction");
	if(bluc && morphc)
	{
		akAnimationChannel* chan = morphc->getChannel("Key 1");
		if(chan)
		{
			morphc->removeChannel(chan);
			bluc->addChannel(chan);
		}
	}
	
	// Joint mask test
	akEntity* blu = getEntity("Blu");
	if(blu)
	{
		akAnimationPlayer* play = blu->getAnimationPlayers()->getAnimationPlayer(0);
		akSkeleton* skel = blu->getSkeleton();
		if(play && skel)
		{
			UTuint32 bid = skel->getIndex("Arm.L");
			if(bid>0)
			{
				play->createJointMask(skel);
				play->setJointMaskWeight(bid, 0.5f);
			}
		}
	}
	
}


int main(int argc, char** argv)
{
	akDemo* demo = new akDemo();
	
	startGlutDemo(argc, argv, demo,"AnimKit OpenGL Demo");
	return 0;
}

