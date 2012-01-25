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

#include "../akEntity.h"

#include "akMesh.h"
#include "akVertexGroup.h"
#include "akAnimationChannel.h"
#include "akSkeletonPose.h"
#include "akGeometryDeformer.h"
#include "akAnimationPlayerSet.h"
#include "akAnimationEngine.h"
#include "akAnimatedObject.h"

akDemo::akDemo() : akDemoBase()
{

}

akDemo::~akDemo()
{
}

#define NUM_VERTS 50
#define NUM_TRIANGLES 28
#define NUM_JOINTS 3

#define NUM_WEIGHTS0 40
#define NUM_WEIGHTS1 40
#define NUM_WEIGHTS2 38


void akDemo::init(void)
{

	m_camera->m_transform = akTransformState::identity();
	m_camera->m_transform.loc = akVector3(7.4811316,-6.5076399,5.3436651);
	m_camera->m_transform.rot = akQuat(0.48170695,0.21292172,0.33425143,0.78159994);
	m_camera->m_transform.scale = akVector3(1,1,1);
	m_camera->m_fov = 49.134342;
	m_camera->m_clipStart = 0.1;
	m_camera->m_clipEnd = 100;


	akMesh* mesh = new akMesh();
	m_canUseVbo = true;
	bool hasNormals= false;
	bool hasColors=false;
	int uvlayers=0;
	utArray<float> uvs;

	akSubMesh* subMesh = new akSubMesh(akSubMesh::ME_TRIANGLES,hasNormals,hasColors,uvlayers);
	akVector3 xyz[NUM_VERTS]={
		akVector3(1.000000,1.000000,-1.000000),
 akVector3(1.000000,-1.000000,-1.000000),
 akVector3(-1.000000,-1.000000,-1.000000),
 akVector3(-1.000000,1.000000,-1.000000),
 akVector3(1.000000,1.000000,-1.000000),
 akVector3(1.000000,0.999999,1.000000),
 akVector3(1.000000,-1.000000,-1.000000),
 akVector3(0.999999,-1.000001,1.000000),
 akVector3(1.000000,-1.000000,-1.000000),
 akVector3(0.999999,-1.000001,1.000000),
 akVector3(-1.000000,-1.000000,-1.000000),
 akVector3(-1.000000,-1.000000,1.000000),
 akVector3(-1.000000,-1.000000,-1.000000),
 akVector3(-1.000000,-1.000000,1.000000),
 akVector3(-1.000000,1.000000,1.000000),
 akVector3(-1.000000,1.000000,-1.000000),
 akVector3(1.000000,0.999999,1.000000),
 akVector3(1.000000,1.000000,-1.000000),
 akVector3(-1.000000,1.000000,1.000000),
 akVector3(-1.000000,1.000000,-1.000000),
 akVector3(-1.000000,1.000000,1.000000),
 akVector3(-1.000000,1.000000,3.000000),
 akVector3(-1.000000,-1.000000,3.000000),
 akVector3(-1.000000,-1.000000,1.000000),
 akVector3(-1.000000,-1.000000,3.000000),
 akVector3(0.999999,-1.000001,3.000000),
 akVector3(-1.000000,1.000000,1.000000),
 akVector3(1.000000,0.999999,3.000000),
 akVector3(-1.000000,1.000000,3.000000),
 akVector3(0.999999,-1.000001,1.000000),
 akVector3(0.999999,-1.000001,3.000000),
 akVector3(1.000000,0.999999,3.000000),
 akVector3(0.999999,-1.000001,3.000000),
 akVector3(1.000000,0.999999,3.000000),
 akVector3(0.999999,-1.000001,5.000000),
 akVector3(1.000000,0.999999,5.000000),
 akVector3(1.000000,0.999999,3.000000),
 akVector3(-1.000000,1.000000,3.000000),
 akVector3(1.000000,0.999999,5.000000),
 akVector3(-1.000000,1.000000,5.000000),
 akVector3(-1.000000,-1.000000,3.000000),
 akVector3(0.999999,-1.000001,3.000000),
 akVector3(-1.000000,-1.000000,5.000000),
 akVector3(0.999999,-1.000001,5.000000),
 akVector3(-1.000000,1.000000,3.000000),
 akVector3(-1.000000,-1.000000,3.000000),
 akVector3(-1.000000,1.000000,5.000000),
 akVector3(-1.000000,-1.000000,5.000000),
 akVector3(1.000000,0.999999,5.000000),
 akVector3(-1.000000,1.000000,5.000000)	};


	int indices[NUM_TRIANGLES*3]={0,1,2,2,3,0,4,5,6,6,5,7,8,9,10,10,9,11,12,13,14,14,15,12,16,17,18,18,17,19,20,13
		,21,21,13,22,23,9,24,24,9,25,16,26,27,27,26,28,29,5,30,30,5,31,32,33,34,34,33,35
		,36,37,38,38,37,39,40,41,42,42,41,43,44,45,46,46,45,47,48,49,34,34,49,47
	};

	akVector3 normal(0,1,0);//s[NUM_VERTS];
	unsigned int color = 0;

	int numIndices = sizeof(indices)/sizeof(int);
	for (int v=0;v<NUM_VERTS;v++)
	{
		subMesh->addVertex(xyz[v],normal,color,uvs);
	}

	for (int i=0;i<NUM_TRIANGLES*3;i++)
	{
		subMesh->addIndex(indices[i]);
	}


	mesh->addSubMesh(subMesh);
	
	int numTris = mesh->getTriangleCount();

	
	utHashedString meshName("myMesh");
	m_animengine->addMesh(meshName, mesh);
	
	akEntity* entity = new akEntity(utHashedString("MyEntity"));
	akTransformState trans = akTransformState::identity();
	entity->setTransformState(trans);
	entity->setMesh(mesh);
	this->addEntity(utHashedString("MyEntity"),entity);
	utHashedString jointNames[NUM_JOINTS]={utHashedString("Bone"),utHashedString("Bone.001"),utHashedString("Bone.002")};
	
	akSkeleton* skeleton = new akSkeleton();
	int parent = skeleton->addJoint(jointNames[0],AK_JOINT_NO_PARENT);
	parent = skeleton->addJoint(jointNames[1],parent);
	parent = skeleton->addJoint(jointNames[2],parent);

	akSkeletonPose* bindPose = new akSkeletonPose(skeleton, akSkeletonPose::SP_MODEL_SPACE);

	akTransformState jointPoses[NUM_JOINTS] = {
		akTransformState(akVector3(0,0,0),akQuat(0.70710671, 0.00000000,0.00000000,0.70710677)),
		akTransformState(akVector3(0,0,1),akQuat(0.0,0.70710671,0.70710677,0.0)),
		akTransformState(akVector3(0,0,2),akQuat(0.0,0.70710671,0.70710677,0.f))};

	for (int i=0;i<NUM_JOINTS;i++)
	{
		*bindPose->getJointPose(i) = jointPoses[i];
	}

	skeleton->setBindingPose(bindPose);
	
	m_animengine->addSkeleton(utHashedString("MySkeleton"),skeleton);

	entity->setSkeleton(skeleton);

	for(int i=0;i<NUM_JOINTS; i++)
	{
		akVertexGroup* vg = new akVertexGroup();
		vg->setName(jointNames[i].str());
		mesh->getSubMesh(0)->addVertexGroup(vg);
	}


	int indices0[40]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,36,37,40,41,44,45};
	int indices1[40]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,36,37,40,41,44,45};
	int indices2[38]={5,7,9,11,13,14,16,18,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49};

	float weights0[NUM_WEIGHTS0]={
		0.895997,0.895092,0.895997,0.895092,0.895997,0.496942,0.895092,0.496901,0.895092
		,0.496901,0.895997,0.496942,0.895997,0.496942,0.496901,0.895092,0.496942,0.895997,
		0.496901,0.895092,0.496901,0.051515,0.051474,0.496942,0.051474,0.051515,0.496901,
		0.051474,0.051515,0.496901,0.051515,0.051474,0.051515,0.051474,0.051474,0.051515,
		0.051474,0.051515,0.051515,0.051474};

	float weights1[NUM_WEIGHTS1]={0.093034,0.093843,0.093034,0.093843,0.093034,0.449999,0.093843,0.450039,0.093843,
			0.450039,0.093034,0.449999,0.093034,0.449999,0.450039,0.093843,0.449999,0.093034,
			0.450039,0.093843,0.450039,0.043311,0.043225,0.449999,0.043225,0.043311,0.450039,
			0.043225,0.043311,0.450039,0.043311,0.043225,0.043311,0.043225,0.043225,0.043311,
			0.043225,0.043311,0.043311,0.043225};

	float weights2[NUM_WEIGHTS2]={0.053059,0.053060,0.053060,0.053059,0.053059,0.053060,0.053059,0.053060,0.053060,
		0.901830,0.901914,0.053059,0.901914,0.901830,0.053060,0.901914,0.901830,0.053060,
		0.901830,0.901914,0.901830,0.901914,0.964282,0.965462,0.901914,0.901830,0.965462,
		0.964282,0.901914,0.901830,0.965462,0.964282,0.901830,0.901914,0.964282,0.965462,
		0.965462,0.964282};

	for (int i=0;i<NUM_WEIGHTS0;i++)
	{
		mesh->getSubMesh(0)->getVertexGroup(0)->add(indices0[i],weights0[i]);
	}
	for (int i=0;i<NUM_WEIGHTS1;i++)
	{
		mesh->getSubMesh(0)->getVertexGroup(1)->add(indices1[i],weights1[i]);
	}
	for (int i=0;i<NUM_WEIGHTS2;i++)
	{
		mesh->getSubMesh(0)->getVertexGroup(2)->add(indices2[i],weights2[i]);
	}

	mesh->generateBoneWeightsFromVertexGroups(skeleton, true);


	akAnimationClip* act = new akAnimationClip();
	

	
	{
		akAnimationChannel* chan = new akAnimationChannel(akAnimationChannel::AC_BONE, jointNames[0]);
			akAnimationCurve* spline = 0;
			spline = new akAnimationCurve(1, akAnimationCurve::AC_CODE_ROT_QUAT_W, akAnimationCurve::BEZ_CUBIC);
			spline->setSample(0,0.00000000,1.0000000,-0.041666668,1.0000000,0.041666668	,1.0000000);
			chan->addCurve(spline);

			spline = new akAnimationCurve(1, akAnimationCurve::AC_CODE_ROT_QUAT_X, akAnimationCurve::BEZ_CUBIC);
			spline->setSample(0,0.00000000,0.00000000,-0.041666668,0.00000000,0.041666668,0.00000000);
			chan->addCurve(spline);

			spline = new akAnimationCurve(1, akAnimationCurve::AC_CODE_ROT_QUAT_Y, akAnimationCurve::BEZ_CUBIC);
			spline->setSample(0,0.00000000,0.00000000,-0.041666668,0.00000000,0.041666668,0.00000000);
			chan->addCurve(spline);

			spline = new akAnimationCurve(1, akAnimationCurve::AC_CODE_ROT_QUAT_Z, akAnimationCurve::BEZ_CUBIC);
			spline->setSample(0,0.00000000,0.00000000,-0.041666668,0.00000000,0.041666668,0.00000000);
			chan->addCurve(spline);
		
		act->addChannel(chan);
	}

	{
		akAnimationChannel* chan = new akAnimationChannel(akAnimationChannel::AC_BONE, jointNames[1]);
			akAnimationCurve* spline = 0;
			spline = new akAnimationCurve(2, akAnimationCurve::AC_CODE_ROT_QUAT_W, akAnimationCurve::BEZ_CUBIC);
			spline->setSample(0,0.00000000,1.0000000,-0.89469218,1.0000000,0.89469218,1.0000000);
			spline->setSample(1,2.2916667,0.92387956,1.3969746,0.92387956,3.1863589,0.92387956);
			chan->addCurve(spline);
			spline = new akAnimationCurve(2, akAnimationCurve::AC_CODE_ROT_QUAT_X, akAnimationCurve::BEZ_CUBIC);
			spline->setSample(0,0.00000000	,0.00000000	,-0.89467138	,0.00000000	,0.89467138	,0.00000000	);
			spline->setSample(1	,2.2916667,0.38268343,1.3969953,0.38268343	,3.1863382	,0.38268343);
			chan->addCurve(spline);
			spline = new akAnimationCurve(2, akAnimationCurve::AC_CODE_ROT_QUAT_Y, akAnimationCurve::BEZ_CUBIC);
			spline->setSample(0,0.00000000,0.00000000,-0.89469308,0.00000000,0.89469308	,0.00000000	);
			spline->setSample(1,2.2916667	,-3.8454296e-015	,1.3969736	,-3.8454296e-015	,3.1863599	,-3.8454296e-015	);
			chan->addCurve(spline);

			spline = new akAnimationCurve(2, akAnimationCurve::AC_CODE_ROT_QUAT_Z, akAnimationCurve::BEZ_CUBIC);
			spline->setSample(0,0.00000000	,0.00000000	,-0.89469308	,0.00000000	,0.89469308	,0.00000000);
			spline->setSample(1	,2.2916667	,5.7783591e-008	,1.3969736	,5.7783591e-008	,3.1863599	,5.7783591e-008	);
			chan->addCurve(spline);
		act->addChannel(chan);
	}

	{
		akAnimationChannel* chan = new akAnimationChannel(akAnimationChannel::AC_BONE, jointNames[2]);
			akAnimationCurve* spline = 0;

			spline = new akAnimationCurve(2, akAnimationCurve::AC_CODE_ROT_QUAT_W, akAnimationCurve::BEZ_CUBIC);
			spline->setSample(0	,0.00000000	,1.0000000	,-0.40667677	,1.0000000	,0.40667677	,1.0000000);
			spline->setSample(1,1.0416667	,0.92387956	,0.63498992	,0.92387956	,1.4483435	,0.92387956	);
			chan->addCurve(spline);

			spline = new akAnimationCurve(2, akAnimationCurve::AC_CODE_ROT_QUAT_X, akAnimationCurve::BEZ_CUBIC);
			spline->setSample(0,0.00000000	,0.00000000	,-0.40663099	,0.00000000	,0.40663099	,0.00000000	);
			spline->setSample(1,1.0416667	,0.38268343	,0.63503569	,0.38268343	,1.4482977	,0.38268343	);
			chan->addCurve(spline);
			
			spline = new akAnimationCurve(2, akAnimationCurve::AC_CODE_ROT_QUAT_Y, akAnimationCurve::BEZ_CUBIC);
			spline->setSample(0	,0.00000000	,0.00000000	,-0.40667865	,0.00000000	,0.40667865	,0.00000000	);
			spline->setSample(	1,1.0416667	,-3.8454296e-015	,0.63498801	,-3.8454296e-015	,1.4483454	,-3.8454296e-015);
			chan->addCurve(spline);
			
			spline = new akAnimationCurve(2, akAnimationCurve::AC_CODE_ROT_QUAT_Z, akAnimationCurve::BEZ_CUBIC);
			spline->setSample(	0,0.00000000	,0.00000000	,-0.40667865	,0.00000000	,0.40667865	,0.00000000	);
			spline->setSample(	1,1.0416667	,3.3455247e-008	,0.63498801	,3.3455247e-008	,1.4483454	,3.3455247e-008	);
			chan->addCurve(spline);

		act->addChannel(chan);
	}
	
	float animfps = 24.f;
	float actionStart = 5.f;
	float actionEnd = 60.f;

	act->setLength( (actionEnd-actionStart )/animfps);
	m_animengine->addAnimationClip(utHashedString("MyActionClip"), act);
	
	akAnimatedObject* animated = new akAnimatedObject(skeleton);
	m_animengine->addAnimatedObject(utHashedString("MyAnimated"),animated);
	entity->setAnimatedObject(animated);

	akAnimationPlayer* play = animated->getAnimationPlayers()->addNewAnimationPlayer(act);

}


int main(int argc, char** argv)
{
	akDemo* demo = new akDemo();
	
	startGlutDemo(argc, argv, demo,"AnimKit OpenGL Demo");
	
	delete demo;
	return 0;
}

