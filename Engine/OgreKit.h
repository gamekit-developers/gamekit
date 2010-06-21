/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#ifndef _OgreKit_h_
#define _OgreKit_h_

#include "gkCamera.h"
#include "gkCommon.h"
#include "gkConstraint.h"
#include "gkCoreApplication.h"
#include "gkDebugProperty.h"
#include "gkDebugScreen.h"
#include "gkDebugger.h"
#include "gkEngine.h"
#include "gkEntity.h"
#include "gkGameObject.h"
#include "gkGameObjectGroup.h"
#include "gkHashedString.h"
#include "gkInput.h"
#include "gkLight.h"
#include "gkLogger.h"
#include "gkMathUtils.h"
#include "gkMemoryTest.h"
#include "gkMesh.h"
#include "gkMeshManager.h"
#include "gkObject.h"
#include "gkPath.h"
#include "gkRenderFactory.h"
#include "gkScene.h"
#include "gkSceneManager.h"
#include "gkSerialize.h"
#include "gkSkeleton.h"
#include "gkString.h"
#include "gkTextFile.h"
#include "gkTextManager.h"
#include "gkTransformState.h"
#include "gkUserDefs.h"
#include "gkUtils.h"
#include "gkVariable.h"
#include "gkWindowSystem.h"

#include "AI/gkNavMeshData.h"
#include "AI/gkSteeringObject.h"
#include "AI/gkSteeringCapture.h"
#include "AI/gkSteeringPathFollowing.h"

#include "Animation/gkAction.h"
#include "Animation/gkActionBake.h"
#include "Animation/gkActionChannel.h"
#include "Animation/gkActionManager.h"
#include "Animation/gkBezierSpline.h"
#include "Animation/gkLightAction.h"
#include "Animation/gkMaterialAction.h"
#include "Animation/gkObjectAction.h"
#include "Animation/gkWorldAction.h"


#include "Constraints/gkLimitLocConstraint.h"
#include "Constraints/gkLimitRotConstraint.h"
#include "Constraints/gkLimitVelocityConstraint.h"

#include "Loaders/Blender2/gkBlendFile.h"
#include "Loaders/Blender2/gkBlendLoader.h"

#include "Logic/gkAnimationNode.h"
#include "Logic/gkButtonNode.h"
#include "Logic/gkCameraNode.h"
#include "Logic/gkCharacterNode.h"
#include "Logic/gkCollisionNode.h"
#include "Logic/gkCursorNode.h"
#include "Logic/gkExitNode.h"
#include "Logic/gkExpressionNode.h"
#include "Logic/gkFallTestNode.h"
#include "Logic/gkGrabNode.h"
#include "Logic/gkGroupNode.h"
#include "Logic/gkIfNode.h"
#include "Logic/gkLogicCommon.h"
#include "Logic/gkLogicNode.h"
#include "Logic/gkLogicSocket.h"
#include "Logic/gkLogicTree.h"
#include "Logic/gkMapNode.h"
#include "Logic/gkMathNode.h"
#include "Logic/gkMotionNode.h"
#include "Logic/gkMouseNode.h"
#include "Logic/gkMultiplexerNode.h"
#include "Logic/gkNodeManager.h"
#include "Logic/gkObjectNode.h"
#include "Logic/gkParticleNode.h"
#include "Logic/gkPickNode.h"
#include "Logic/gkPrintNode.h"
#include "Logic/gkPulseNode.h"
#include "Logic/gkRandomNode.h"
#include "Logic/gkRayTestNode.h"
#include "Logic/gkShowPhysicsNode.h"
#include "Logic/gkStateMachineNode.h"
#include "Logic/gkSwitchNode.h"
#include "Logic/gkTimerNode.h"
#include "Logic/gkValueNode.h"
#include "Logic/gkVariableNode.h"

#include "LogicBricks/gkActionActuator.h"
#include "LogicBricks/gkAlwaysSensor.h"
#include "LogicBricks/gkCollisionSensor.h"
#include "LogicBricks/gkEditObjectActuator.h"
#include "LogicBricks/gkGameActuator.h"
#include "LogicBricks/gkKeyboardSensor.h"
#include "LogicBricks/gkLogicActuator.h"
#include "LogicBricks/gkLogicBrick.h"
#include "LogicBricks/gkLogicController.h"
#include "LogicBricks/gkLogicDispatcher.h"
#include "LogicBricks/gkLogicLink.h"
#include "LogicBricks/gkLogicManager.h"
#include "LogicBricks/gkLogicOpController.h"
#include "LogicBricks/gkLogicSensor.h"
#include "LogicBricks/gkMotionActuator.h"
#include "LogicBricks/gkMouseSensor.h"
#include "LogicBricks/gkPropertyActuator.h"
#include "LogicBricks/gkPropertySensor.h"
#include "LogicBricks/gkRaySensor.h"
#include "LogicBricks/gkScriptController.h"
#include "LogicBricks/gkStateActuator.h"
#include "LogicBricks/gkVisibilityActuator.h"

#include "Physics/gkCharacter.h"
#include "Physics/gkDynamicsWorld.h"
#include "Physics/gkPhysicsDebug.h"
#include "Physics/gkRagDoll.h"
#include "Physics/gkRigidBody.h"
#include "Physics/gkSoftBody.h"
#include "Physics/gkVehicle.h"
#include "Physics/gkRayTest.h"
#include "Physics/gkSweptTest.h"

#include "Script/gkLuaManager.h"

#include "Utils/utCommon.h"
#include "Utils/utString.h"
#include "Utils/utTypes.h"

#include "Thread/gkActiveObject.h"
#include "Thread/gkCriticalSection.h"
#include "Thread/gkNonCopyable.h"
#include "Thread/gkNonCopyable.h"
#include "Thread/gkPtrRef.h"
#include "Thread/gkQueue.h"
#include "Thread/gkSyncObj.h"
#include "Thread/gkThread.h"


#endif//_OgreKit_h_
