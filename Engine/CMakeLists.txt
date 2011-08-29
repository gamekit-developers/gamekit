# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

# ---------------------------------------------------------
# Core Files
# ---------------------------------------------------------

set(Core_SOURCE
	# ----- Source -----
	gkBone.cpp
	gkCamera.cpp
	gkCam2ViewportRay.cpp
	gkCoreApplication.cpp
	gkDebugFps.cpp
	gkDebugger.cpp
	gkDebugProperty.cpp
	gkDebugScreen.cpp
	gkEngine.cpp
	gkEntity.cpp
	gkFont.cpp
	gkFontManager.cpp
	gkGameObject.cpp
	gkGameObjectManager.cpp
	gkGameObjectGroup.cpp
	gkGameObjectInstance.cpp
	gkGroupManager.cpp
	gkInstancedManager.cpp
	gkInstancedObject.cpp
	gkLight.cpp
	gkLogger.cpp
	gkMesh.cpp
	gkMeshManager.cpp
	gkMessageManager.cpp
	gkMathUtils.cpp
	gkPath.cpp
	gkTextFile.cpp
	gkTickState.cpp
	gkTextManager.cpp
	gkRenderFactory.cpp
	gkResource.cpp
	gkResourceManager.cpp
	gkResourceGroupManager.cpp
	gkScene.cpp
	gkSceneManager.cpp
	gkSkeleton.cpp
	gkSkeletonManager.cpp
	gkSkeletonResource.cpp
	gkStats.cpp
	gkUserDefs.cpp
	gkUtils.cpp
	gkWindow.cpp
	gkViewport.cpp
	gkVariable.cpp
)

set(Core_SOURCE_2
	gkWindowSystem.cpp	
)	

set(Core_HEADER
	# ----- Headers -----
	gkBone.h
	gkCamera.h
	gkCam2ViewportRay.h
	gkCommon.h
	gkCoreApplication.h
	gkDebugFps.h
	gkDebugger.h
	gkDebugProperty.h
	gkDebugScreen.h
	gkEngine.h
	gkEntity.h
	gkFont.h
	gkFontManager.h
	gkGameObject.h
	gkGameObjectManager.h
	gkGameObjectGroup.h
	gkGameObjectInstance.h
	gkGroupManager.h
	gkInstancedManager.h
	gkInstancedObject.h
	gkHashedString.h
	gkInput.h
	gkLight.h
	gkLogger.h
	gkMesh.h
	gkMeshManager.h
	gkMessageManager.h
	gkMathUtils.h
	gkMemoryTest.h
	gkPath.h
	gkTextFile.h
	gkTickState.h
	gkTextManager.h
	gkTickState.h
	gkRenderFactory.h
	gkResource.h
	gkResourceName.h
	gkResourceManager.h
	gkResourceGroupManager.h
	gkScene.h
	gkSceneManager.h
	gkSerialize.h
	gkSkeleton.h
	gkSkeletonManager.h
	gkSkeletonResource.h
	gkStats.h
	gkString.h
	gkTransformState.h
	gkUserDefs.h
	gkUtils.h
	gkValue.h
	gkVariable.h
	gkWindowSystem.h
	gkWindow.h
	gkViewport.h
	${CMAKE_BINARY_DIR}/Engine/gkSettings.h
	OgreKit.h
)

if (OGREKIT_BUILD_IPHONE)
	list(APPEND Core_HEADER iOS/gkWindowIOS.h)
	list(APPEND Core_SOURCE iOS/gkWindowIOS.mm)
endif()

if (OGREKIT_BUILD_ANDROID)
	list(APPEND Core_HEADER Android/gkWindowAndroid.h)
	list(APPEND Core_SOURCE Android/gkWindowAndroid.cpp)
endif()

if(APPLE)
	source_group(Core FILES ${Core_HEADER} ${Core_SOURCE} ${Core_SOURCE_2})
else()
	source_group(Core\\Header FILES ${Core_HEADER})
	source_group(Core\\Source FILES ${Core_SOURCE} ${Core_SOURCE_2})
endif()

# ---------------------------------------------------------
# AI Files
# ---------------------------------------------------------

set(AI_SOURCE
	# ----- Source -----	
	AI/gkNavMeshData.cpp	
	AI/gkSteering.cpp
	AI/gkFSM.cpp
)

set(AI_HEADER
	# ----- Headers -----
	AI/gkMeshData.h
	AI/gkNavMeshData.h
	AI/gkSteering.h
	AI/gkFSM.h
)

if (OGREKIT_COMPILE_RECAST)
list(APPEND AI_SOURCE AI/gkRecast.cpp)
list(APPEND AI_HEADER AI/gkRecast.h)
endif()

if (OGREKIT_COMPILE_OPENSTEER)
list(APPEND AI_SOURCE
	AI/gkNavPath.cpp
	AI/gkSteeringObject.cpp
	AI/gkSteeringCapture.cpp
	AI/gkSteeringPathFollowing.cpp
	AI/gkSteeringWander.cpp
	AI/gkSceneObstacle.cpp
)
list(APPEND AI_HEADER 
	AI/gkNavPath.h
	AI/gkSteeringObject.h
	AI/gkSteeringCapture.h
	AI/gkSteeringPathFollowing.h
	AI/gkSteeringWander.h
	AI/gkSceneObstacle.h
)
endif()

if(APPLE)
	source_group(AI FILES ${AI_HEADER} ${AI_SOURCE})

else()
	source_group(AI\\Header FILES ${AI_HEADER})
	source_group(AI\\Source FILES ${AI_SOURCE})
endif()


# ---------------------------------------------------------
# Thread Files
# ---------------------------------------------------------

set(Thread_SOURCE
	# ----- Source -----
	Thread/gkActiveObject.cpp
	Thread/gkCriticalSection.cpp
	Thread/gkPtrRef.cpp
	Thread/gkThread.cpp
)

set(Thread_HEADER
	# ----- Headers -----
	Thread/gkAsyncResult.h
	Thread/gkActiveObject.h
	Thread/gkCriticalSection.h
	Thread/gkNonCopyable.h
	Thread/gkPtrRef.h
	Thread/gkQueue.h
	Thread/gkSyncObj.h
	Thread/gkThread.h
)

set(Thread_SOURCE_2
	Thread/gkSyncObj.cpp
)

if(APPLE)
	source_group(Thread FILES ${Thread_HEADER} ${Thread_SOURCE} ${Thread_SOURCE_2})

else()
	source_group(Thread\\Header FILES ${Thread_HEADER})
	source_group(Thread\\Source FILES ${Thread_SOURCE} ${Thread_SOURCE_2})
endif()


# ---------------------------------------------------------
# Animation Files
# ---------------------------------------------------------

set(Animation_SOURCE
	# ----- Source -----
	Animation/gkAnimation.cpp
	Animation/gkAnimationManager.cpp
)


set(Animation_HEADER
	# ----- Headers -----
	Animation/gkAnimation.h
	Animation/gkAnimationDefs.h
	Animation/gkAnimationManager.h
)

if(APPLE)
	source_group(Animation FILES ${Animation_HEADER}  ${Animation_SOURCE})

else()
	source_group(Animation\\Header FILES ${Animation_HEADER})
	source_group(Animation\\Source FILES ${Animation_SOURCE})
endif()


# ---------------------------------------------------------
# Particles Files
# ---------------------------------------------------------

if(OGREKIT_USE_PARTICLE)

set(Particles_SOURCE
	# ----- Source -----
	Particles/gkParticleObject.cpp
	Particles/gkParticleResource.cpp	
	Particles/gkParticleManager.cpp

	# Ogre particle implementation
	Particles/Ogre/gkOgreParticleObject.cpp
	Particles/Ogre/gkOgreParticleResource.cpp
	Particles/Ogre/gkOgreParticleAffector.cpp
	Particles/Ogre/gkOgreParticleEmitter.cpp
	Particles/Ogre/gkOgreParticleRenderer.cpp
)


set(Particles_HEADER
	# ----- Headers -----
	Particles/gkParticleObject.h
	Particles/gkParticleResource.h
	Particles/gkParticleManager.h

	# Ogre particle implementation
	Particles/Ogre/gkOgreParticleObject.h
	Particles/Ogre/gkOgreParticleResource.h
	Particles/Ogre/gkOgreParticleAffector.h
	Particles/Ogre/gkOgreParticleEmitter.h
	Particles/Ogre/gkOgreParticleRenderer.h
)

if(APPLE)
	source_group(Particles FILES ${Particles_HEADER}  ${Particles_SOURCE})

else()
	source_group(Particles\\Header FILES ${Particles_HEADER})
	source_group(Particles\\Source FILES ${Particles_SOURCE})
endif()

endif()

# ---------------------------------------------------------
# Logic Nodes
# ---------------------------------------------------------
if (OGREKIT_USE_NNODE)

set(NLogic_SOURCE
	# ----- Source -----
	Logic/gkButtonNode.cpp
	Logic/gkCameraNode.cpp
	Logic/gkCharacterNode.cpp
	Logic/gkCollisionNode.cpp
	Logic/gkExitNode.cpp
	#Logic/gkExpressionNode.cpp
	Logic/gkGrabNode.cpp
	Logic/gkGroupNode.cpp
	Logic/gkNodeManager.cpp
	Logic/gkLogicNode.cpp
	Logic/gkLogicSocket.cpp
	Logic/gkLogicTree.cpp
	Logic/gkMotionNode.cpp
	Logic/gkMouseNode.cpp
	Logic/gkObjectNode.cpp
	Logic/gkPrintNode.cpp
	Logic/gkRandomNode.cpp
	Logic/gkRayTestNode.cpp
	#Logic/gkSwitchNode.cpp
	Logic/gkTimerNode.cpp
	Logic/gkValueNode.cpp
	Logic/gkVariableNode.cpp
	Logic/gkCursorNode.cpp
	Logic/gkParticleNode.cpp
	Logic/gkPickNode.cpp
	Logic/gkPulseNode.cpp
	Logic/gkShowPhysicsNode.cpp
	Logic/gkStateMachineNode.cpp
)

set(NLogic_HEADER
	# ----- Headers -----
	Logic/gkButtonNode.h
	Logic/gkCameraNode.h
	Logic/gkCharacterNode.h
	Logic/gkCollisionNode.h
	Logic/gkExitNode.h
	Logic/gkExpressionNode.h
	Logic/gkFunctionNode.h
	Logic/gkGrabNode.h
	Logic/gkGroupNode.h
	Logic/gkIfNode.h
	Logic/gkLogicCommon.h
	Logic/gkNodeManager.h
	Logic/gkLogicNode.h
	Logic/gkLogicSocket.h
	Logic/gkLogicTree.h
	Logic/gkMapNode.h
	Logic/gkMathNode.h
	Logic/gkMotionNode.h
	Logic/gkMouseNode.h
	Logic/gkMultiplexerNode.h
	Logic/gkObjectNode.h
	Logic/gkPrintNode.h
	Logic/gkRandomNode.h
	Logic/gkRayTestNode.h
	Logic/gkSwitchNode.h
	Logic/gkTimerNode.h
	Logic/gkValueNode.h
	Logic/gkVariableNode.h
	Logic/gkCursorNode.h
	Logic/gkParticleNode.h
	Logic/gkPickNode.h
	Logic/gkPulseNode.h
	Logic/gkShowPhysicsNode.h
	Logic/gkStateMachineNode.h
	Logic/gkQuaternionToEulerNode.h
	Logic/gkEulerToQuaternionNode.h
	Logic/gkVectorComposeNode.h
	Logic/gkVectorDecomposeNode.h
)

if(APPLE)
	source_group(LogicNodes FILES ${NLogic_HEADER} ${NLogic_SOURCE})

else()
	source_group(LogicNodes\\Header FILES ${NLogic_HEADER})
	source_group(LogicNodes\\Source FILES ${NLogic_SOURCE})
endif()

endif()

# ---------------------------------------------------------
# Logic Bricks
# ---------------------------------------------------------
set(Logic_SOURCE
	LogicBricks/gkActionActuator.cpp
	LogicBricks/gkActuatorSensor.cpp
	LogicBricks/gkAlwaysSensor.cpp
	LogicBricks/gkCollisionSensor.cpp
	LogicBricks/gkDelaySensor.cpp
	LogicBricks/gkEditObjectActuator.cpp
	LogicBricks/gkExpressionController.cpp
	LogicBricks/gkGameActuator.cpp
	LogicBricks/gkJoystickSensor.cpp
	LogicBricks/gkKeyboardSensor.cpp
	LogicBricks/gkLogicActuator.cpp
	LogicBricks/gkLogicBrick.cpp
	LogicBricks/gkLogicController.cpp
	LogicBricks/gkLogicDispatcher.cpp
	LogicBricks/gkLogicLink.cpp
	LogicBricks/gkLogicManager.cpp
	LogicBricks/gkLogicOpController.cpp
	LogicBricks/gkLogicSensor.cpp
	LogicBricks/gkMotionActuator.cpp
	LogicBricks/gkMouseSensor.cpp
	LogicBricks/gkMessageActuator.cpp
	LogicBricks/gkMessageSensor.cpp
	LogicBricks/gkNearSensor.cpp
	LogicBricks/gkParentActuator.cpp
	LogicBricks/gkRadarSensor.cpp
	LogicBricks/gkRandomActuator.cpp
	LogicBricks/gkRandomSensor.cpp
	LogicBricks/gkRaySensor.cpp
	LogicBricks/gkSceneActuator.cpp
	LogicBricks/gkScriptController.cpp
	LogicBricks/gkStateActuator.cpp
	LogicBricks/gkVisibilityActuator.cpp
	LogicBricks/gkPropertySensor.cpp
	LogicBricks/gkPropertyActuator.cpp
	LogicBricks/gkSoundActuator.cpp
)


set(Logic_HEADER
	LogicBricks/gkActionActuator.h
	LogicBricks/gkActuatorSensor.h
	LogicBricks/gkAlwaysSensor.h
	LogicBricks/gkCollisionSensor.h
	LogicBricks/gkDelaySensor.h
	LogicBricks/gkEditObjectActuator.h
	LogicBricks/gkExpressionController.h
	LogicBricks/gkGameActuator.h
	LogicBricks/gkJoystickSensor.h
	LogicBricks/gkKeyboardSensor.h
	LogicBricks/gkLogicActuator.h
	LogicBricks/gkLogicBrick.h
	LogicBricks/gkLogicController.h
	LogicBricks/gkLogicDispatcher.h
	LogicBricks/gkLogicLink.h
	LogicBricks/gkLogicManager.h
	LogicBricks/gkLogicOpController.h
	LogicBricks/gkLogicSensor.h
	LogicBricks/gkMotionActuator.h
	LogicBricks/gkMouseSensor.h
	LogicBricks/gkMessageActuator.h
	LogicBricks/gkMessageSensor.h
	LogicBricks/gkNearSensor.h
	LogicBricks/gkParentActuator.h
	LogicBricks/gkRadarSensor.h
	LogicBricks/gkRandomActuator.h
	LogicBricks/gkRandomSensor.h
	LogicBricks/gkRaySensor.h
	LogicBricks/gkSceneActuator.h
	LogicBricks/gkScriptController.h
	LogicBricks/gkStateActuator.h
	LogicBricks/gkVisibilityActuator.h
	LogicBricks/gkPropertySensor.h
	LogicBricks/gkPropertyActuator.h
	LogicBricks/gkSoundActuator.h
)

if(APPLE)
	source_group(LogicBricks FILES ${Logic_HEADER} ${Logic_SOURCE})
else()
	source_group(LogicBricks\\Header FILES ${Logic_HEADER})
	source_group(LogicBricks\\Source FILES ${Logic_SOURCE})
endif()

# ---------------------------------------------------------
# BlendFile Loaders
# ---------------------------------------------------------
set(Blend_SOURCE
	# ----- Public Source -----	
	Loaders/Blender2/gkBlendFile.cpp
	Loaders/Blender2/gkBlendInternalFile.cpp
	Loaders/Blender2/gkBlendLoader.cpp
	Loaders/Blender2/gkTextureLoader.cpp
	Loaders/Blender2/gkBlenderSceneConverter.cpp	
	Loaders/Blender2/Converters/gkAnimationConverter.cpp
	Loaders/Blender2/Converters/gkLogicBrickConverter.cpp
	Loaders/Blender2/Converters/gkMeshConverter.cpp
	Loaders/Blender2/Converters/gkSkeletonConverter.cpp	
)

set(Blend_HEADER
	# ----- Public Headers -----	
	Loaders/Blender2/gkBlendFile.h
	Loaders/Blender2/gkBlendInternalFile.h
	Loaders/Blender2/gkBlendLoader.h
	Loaders/Blender2/gkLoaderCommon.h
	Loaders/Blender2/gkBlenderDefines.h
	Loaders/Blender2/gkTextureLoader.h
	Loaders/Blender2/gkBlenderSceneConverter.h
	Loaders/Blender2/Converters/gkAnimationConverter.h
	Loaders/Blender2/Converters/gkLogicBrickConverter.h
	Loaders/Blender2/Converters/gkMeshConverter.h
	Loaders/Blender2/Converters/gkSkeletonConverter.h	
)

if(OGREKIT_USE_PARTICLE)
	list(APPEND Blend_SOURCE Loaders/Blender2/Converters/gkParticleConverter.cpp)
	list(APPEND Blend_HEADER Loaders/Blender2/Converters/gkParticleConverter.h)
endif()

if(APPLE)
	source_group(Blender FILES  ${Blend_HEADER} ${Blend_SOURCE})

else()
	source_group(Blender\\Source FILES ${Blend_SOURCE})
	source_group(Blender\\Header FILES ${Blend_HEADER})
endif()

# ---------------------------------------------------------
# Constraints
# ---------------------------------------------------------
set(Constraint_SOURCE
	# ----- Source -----
	Constraints/gkConstraint.cpp
	Constraints/gkConstraintManager.cpp
	Constraints/gkLimitLocConstraint.cpp
	Constraints/gkLimitRotConstraint.cpp
	Constraints/gkLimitVelocityConstraint.cpp
)

set(Constraint_HEADER
	# ----- Header -----
	Constraints/gkConstraint.h
	Constraints/gkConstraintManager.h
	Constraints/gkLimitLocConstraint.h
	Constraints/gkLimitRotConstraint.h
	Constraints/gkLimitVelocityConstraint.h
)

if(APPLE)
	source_group(Constraints FILES ${Constraint_HEADER} ${Constraint_SOURCE})

else()
	source_group(Constraints\\Header FILES ${Constraint_HEADER})
	source_group(Constraints\\Source FILES ${Constraint_SOURCE})
endif()


# ---------------------------------------------------------
# Physics
# ---------------------------------------------------------
set(Physics_SOURCE
	# ----- Source -----
	Physics/gkCharacter.cpp
	Physics/gkDbvt.cpp
	Physics/gkDynamicsWorld.cpp
	Physics/gkPhysicsController.cpp
	Physics/gkPhysicsDebug.cpp
	Physics/gkRagDoll.cpp
	Physics/gkRayTest.cpp
	Physics/gkRigidBody.cpp
	Physics/gkSoftBody.cpp
	Physics/gkSweptTest.cpp
	Physics/gkVehicle.cpp
)

set(Physics_HEADER
	# ----- Header -----
	Physics/gkCharacter.h
	Physics/gkContactTest.h
	Physics/gkDbvt.h
	Physics/gkDynamicsWorld.h
	Physics/gkPhysicsController.h
	Physics/gkPhysicsDebug.h
	Physics/gkRagDoll.h
	Physics/gkRayTest.h
	Physics/gkRigidBody.h
	Physics/gkSoftBody.h
	Physics/gkSweptTest.h
	Physics/gkVehicle.h
)

if(APPLE)
	source_group(Physics FILES ${Physics_HEADER} ${Physics_SOURCE})
else()
	source_group(Physics\\Header FILES ${Physics_HEADER})
	source_group(Physics\\Source FILES ${Physics_SOURCE})
endif()

# ---------------------------------------------------------
# Script Files
# ---------------------------------------------------------
if (OGREKIT_USE_LUA)

if (OGREKIT_COMPILE_SWIG AND SWIG_FOUND)


	set(Script_INC
		# ----- Include Files -----
		Script/Api/OgreKit.i
		Script/Api/gsTypes.i
		Script/Api/gsTypeConverters.i
		Script/Api/gsAI.i
		Script/Api/gsAnimation.i
		Script/Api/gsBricks.i
		Script/Api/gsConstraints.i
		Script/Api/gsCore.i		
		Script/Api/gsMacros.i
		Script/Api/gsMath.i
		Script/Api/gsNetwork.i
		Script/Api/gsNodes.i
		Script/Api/gsParticles.i
		Script/Api/gsPhysics.i
		Script/Api/gsScript.i
		Script/Api/gsSound.i
		Script/Api/gsThread.i
		Script/Api/gsTypes.i
		Script/Api/gsUtils.i
	)

	set(Script_TMPL
		Script/Api/System.lua
		Script/Api/Class.lua
	)
	add_templates(0 GEN_TEMPLATES_OUT Script/Api/Generated/gsTemplates.h ${Script_TMPL})

	set(Script_HEADER
		# ----- Common Files -----
		Script/Lua/gkLuaManager.h
		Script/Lua/gkLuaScript.h
		Script/Lua/gkLuaUtils.h
		Script/Api/gsAI.h
		Script/Api/gsAnimation.h
		Script/Api/gsBricks.h
		Script/Api/gsCommon.h
		Script/Api/gsConstraints.h
		Script/Api/gsCore.h
		Script/Api/gsMath.h
		Script/Api/gsNetwork.h
		Script/Api/gsNodes.h
		Script/Api/gsParticles.h
		Script/Api/gsPhysics.h
		Script/Api/gsScript.h
		Script/Api/gsSound.h
		Script/Api/gsThread.h
		Script/Api/gsUtils.h
		${Script_INC}
		${GEN_TEMPLATES_OUT}
	)

	set(SWIG_INPUT
		Script/Api/OgreKit.i
	)


	swig_source_lua(SWIG_API2_OUTPUT_LUA Script_HEADER ${SWIG_INPUT})


	set(Script_SOURCE
		Script/Lua/gkLuaManager.cpp
		Script/Lua/gkLuaScript.cpp
		Script/Lua/gkLuaUtils.cpp
		Script/Api/gsAI.cpp
		Script/Api/gsAnimation.cpp
		Script/Api/gsBricks.cpp
		Script/Api/gsConstraints.cpp
		Script/Api/gsCore.cpp		
		Script/Api/gsMath.cpp
		Script/Api/gsNetwork.cpp
		Script/Api/gsNodes.cpp
		Script/Api/gsParticles.cpp
		Script/Api/gsPhysics.cpp
		Script/Api/gsScript.cpp
		Script/Api/gsSound.cpp
		Script/Api/gsThread.cpp
		Script/Api/gsUtils.cpp

	)
	
	set(Script_SOURCE_2
		${Script_TMPL}
		${SWIG_API2_OUTPUT_LUA}
	)

	if(APPLE)
		source_group(Script    FILES ${Script_TMPL}
			${SWIG_INPUT} ${Script_INC}
			${Script_HEADER}
			${Script_SOURCE} ${Script_SOURCE_2})
	else()
		source_group(Script\\Builtin   FILES ${Script_TMPL})
		source_group(Script\\Includes  FILES ${SWIG_INPUT} ${Script_INC})
		source_group(Script\\Header    FILES ${Script_HEADER})
		source_group(Script\\Source    FILES ${Script_SOURCE} ${Script_SOURCE_2})
	endif()


else()

	set(Script_INC
		# ----- Include Files -----
		Script/Api/OgreKit.i
		Script/Api/gsTypes.i
		Script/Api/gsTypeConverters.i
		Script/Api/gsAI.i
		Script/Api/gsAnimation.i
		Script/Api/gsBricks.i
		Script/Api/gsConstraints.i
		Script/Api/gsCore.i		
		Script/Api/gsMacros.i
		Script/Api/gsMath.i
		Script/Api/gsNetwork.i
		Script/Api/gsNodes.i
		Script/Api/gsParticles.i
		Script/Api/gsPhysics.i
		Script/Api/gsScript.i
		Script/Api/gsSound.i
		Script/Api/gsThread.i
		Script/Api/gsTypes.i
		Script/Api/gsUtils.i
	)

	set(Script_HEADER
		# ----- Common Files -----
		Script/Api/Generated/gsTemplates.h
		Script/Lua/gkLuaManager.h
		Script/Lua/gkLuaScript.h
		Script/Lua/gkLuaUtils.h
		Script/Api/gsAI.h
		Script/Api/gsAnimation.h
		Script/Api/gsBricks.h
		Script/Api/gsCommon.h
		Script/Api/gsConstraints.h
		Script/Api/gsCore.h
		Script/Api/gsMath.h
		Script/Api/gsNetwork.h
		Script/Api/gsNodes.h
		Script/Api/gsParticles.h
		Script/Api/gsPhysics.h
		Script/Api/gsScript.h
		Script/Api/gsSound.h
		Script/Api/gsThread.h
		Script/Api/gsUtils.h
		${Script_INC}
	)


	set(Script_TMPL
		Script/Api/System.lua
		Script/Api/Class.lua
	)

	set(Script_SOURCE		
		Script/Lua/gkLuaManager.cpp
		Script/Lua/gkLuaScript.cpp
		Script/Lua/gkLuaUtils.cpp
		Script/Api/gsAI.cpp
		Script/Api/gsAnimation.cpp
		Script/Api/gsBricks.cpp
		Script/Api/gsConstraints.cpp
		Script/Api/gsCore.cpp		
		Script/Api/gsMath.cpp
		Script/Api/gsNetwork.cpp
		Script/Api/gsNodes.cpp
		Script/Api/gsParticles.cpp
		Script/Api/gsPhysics.cpp
		Script/Api/gsScript.cpp
		Script/Api/gsSound.cpp
		Script/Api/gsThread.cpp
		Script/Api/gsUtils.cpp
	)
	
	set(Script_SOURCE_2
		${Script_TMPL}
		Script/Api/Generated/OgreKitLua.cpp
	)
	
	if(APPLE)
		source_group(Script	FILES ${Script_TMPL}
			${Script_INC}
			${Script_HEADER}
			${Script_SOURCE} ${Script_SOURCE_2})
	else()
		source_group(Script\\Builtin   FILES ${Script_TMPL})
		source_group(Script\\Include   FILES ${Script_INC})
		source_group(Script\\Header    FILES ${Script_HEADER})
		source_group(Script\\Source    FILES ${Script_SOURCE} ${Script_SOURCE_2})
	endif()

endif()

endif() #OGREKIT_USE_LUA

# ---------------------------------------------------------
# Sound Files
# ---------------------------------------------------------
set(Sound_SOURCE
	# ----- Source -----
	Sound/gkBuffer.cpp
	Sound/gkOgg.cpp
	Sound/gkSource.cpp
	Sound/gkSound.cpp
	Sound/gkSoundManager.cpp
	Sound/gkSoundStream.cpp
	Sound/gkSoundUtil.cpp
	Sound/gkStreamer.cpp
	Sound/gkWaveform.cpp
)

set(Sound_HEADER
	# ----- Header -----
	Sound/gkBuffer.h
	Sound/gkSource.h
	Sound/gkOgg.h
	Sound/gkSound.h
	Sound/gkSoundManager.h
	Sound/gkSoundStream.h
	Sound/gkSoundUtil.h
	Sound/gkStreamer.h
	Sound/gkWaveform.h
)

if(APPLE)
	source_group(Sound FILES ${Sound_HEADER} ${Sound_SOURCE})
else()
	source_group(Sound\\Header FILES ${Sound_HEADER})
	source_group(Sound\\Source FILES ${Sound_SOURCE})
endif()


# ---------------------------------------------------------
# Network Files
# ---------------------------------------------------------
set(Network_SOURCE
	# ----- Source -----
	Network/gkNetworkManager.cpp
)

set(Network_HEADER
	# ----- Header -----
	Network/gkNetworkManager.h
)

if(APPLE)
	source_group(Network FILES ${Network_HEADER} ${Network_SOURCE})

else()
	source_group(Network\\Header FILES ${Network_HEADER})
	source_group(Network\\Source FILES ${Network_SOURCE})
endif()

# ---------------------------------------------------------
# Graphics Files
# ---------------------------------------------------------
set(Graphics_SOURCE
	# ----- Source -----
	Graphics/gkHUD.cpp
	Graphics/gkHUDElement.cpp
	Graphics/gkHUDManager.cpp
)

set(Graphics_HEADER
	# ----- Header -----
	Graphics/gkHUD.h
	Graphics/gkHUDElement.h
	Graphics/gkHUDManager.h
)

if(APPLE)
	source_group(Graphics FILES ${Graphics_HEADER} ${Network_SOURCE})

else()
	source_group(Graphics\\Header FILES ${Graphics_HEADER})
	source_group(Graphics\\Source FILES ${Graphics_SOURCE})
endif()




# ---------------------------------------------------------
# External Engine Files
# ---------------------------------------------------------
set(External_SOURCE
	# ----- Source -----
	External/Ogre/gkOgreMaterialLoader.cpp
	External/Ogre/gkOgreMeshLoader.cpp
	External/Ogre/gkOgreSkeletonLoader.cpp
	External/Ogre/gkOgreSkyBoxGradient.cpp
	External/Ogre/gkOgreBlendArchive.cpp		
)

set(External_HEADER
	# ----- Header -----
	External/Ogre/gkOgreMaterialLoader.h
	External/Ogre/gkOgreMeshLoader.h
	External/Ogre/gkOgreSkeletonLoader.h
	External/Ogre/gkOgreSkyBoxGradient.h
	External/Ogre/gkOgreBlendArchive.h		
)

if (OGREKIT_USE_COMPOSITOR)
	list(APPEND External_SOURCE External/Ogre/gkOgreCompositorManager.cpp External/Ogre/gkOgreCompositorHelper.cpp)
	list(APPEND External_HEADER External/Ogre/gkOgreCompositorManager.h   External/Ogre/gkOgreCompositorHelper.h)
endif()

if(APPLE)
	source_group(External FILES ${External_SOURCE} ${External_HEADER})

else()
	source_group(External\\Header FILES ${External_HEADER})
	source_group(External\\Source FILES ${External_SOURCE})
endif()

# ---------------------------------------------------------
# Resources Files
# ---------------------------------------------------------

set(Resources_FILES)

if (OGREKIT_GENERATE_BUILTIN_RES)


	
	#particle
	set(PARTICLE_TMPL
		User/flare_alpha_64.png
	)
	add_templates(1 GEN_PARTICLE_OUT User/gkParticleTemplates.inl ${PARTICLE_TMPL})
	LIST(APPEND Resources_FILES	${GEN_PARTICLE_OUT})
	
	#rtshader
	set(RTSHADER_TMPL User/RTShaderLib.zip)
	add_templates(1 GEN_RTSHADER_OUT User/gkRTShaderTemplates.inl ${RTSHADER_TMPL})
	LIST(APPEND Resources_FILES	${GEN_RTSHADER_OUT})
	
	#compositor
	set(COMPOSITOR_TMPL User/Compositor.zip)
	add_templates(1 GEN_COMPOSITOR_OUT User/gkCompositorTemplates.inl ${COMPOSITOR_TMPL})
	LIST(APPEND Resources_FILES	${GEN_COMPOSITOR_OUT})
	
	#compositor textures
	set(COMPOSITOR_TEX_TMPL User/CompositorTextures.zip)
	add_templates(1 GEN_COMPOSITOR_TEX_OUT User/gkCompositorTextures.inl ${COMPOSITOR_TEX_TMPL})
	LIST(APPEND Resources_FILES	${GEN_COMPOSITOR_TEX_OUT})
	
	#font, etc...

	source_group(Resources FILES ${Resources_FILES})			

endif()



# ---------------------------------------------------------
# Build setup
# ---------------------------------------------------------

if (APPLE)
	include_directories( ${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX )
endif ()

if(OGREKIT_USE_COCOA)
	set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/Thread/gkSyncObj.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
endif()

if (OGREKIT_BUILD_IPHONE)
	set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/gkWindowSystem.cpp   PROPERTIES COMPILE_FLAGS "-x objective-c++")
	set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/Thread/gkSyncObj.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
	include_directories(./iOS)
endif()

if (OGREKIT_BUILD_ANDROID)
	include_directories(./Android)
endif()


include_directories(
	${OGREKIT_OGRE_INCLUDE}
	${OGRELITE_SOURCE_DIR}/PlugIns/ParticleFX/include
	${OGREKIT_DEP_INCLUDE}
	./
	./AI
	./Animation
	./Constraints
	./LogicBricks
	./Logic
	./Script
	./Script/Api
	./Sound
	./Loaders/Blender2
	./Physics
	./Thread
	./Particles
	./Graphics
	)

#Reconfigure external Bullet libraries ${OGREKIT_BULLET_LIBS} before link 
config_target_if_iphone_build(BulletDynamics TRUE)
config_target_if_iphone_build(BulletCollision TRUE)
if (OGREKIT_COMPILE_SOFTBODY)
	config_target_if_iphone_build(BulletSoftBody TRUE)
endif()	
config_target_if_iphone_build(LinearMath TRUE)

link_libraries(
	${OGREKIT_OGRE_LIBS}
	#Plugin_ParticleFX
	${OGREKIT_BULLET_LIBS}
	)
	
if (OGREKIT_USE_PARTICLE)
	link_libraries(Plugin_ParticleFX)
endif()

link_directories(${OGRE_LIBDIR})
include_directories(${OGREKIT_BULLET_INCLUDE})

configure_rendersystem()

set(HiddenDepenCMakeLists ../Dependencies/CMakeLists.txt)
source_group(DependenciesCMakeLists FILES ${HiddenDepenCMakeLists})


# for swig no __declspec(dllexport)
add_definitions(-DSTATIC_LINKED)

if (OGREKIT_OPENAL_SOUND AND OPENAL_FOUND)
	set(Extra_SOURCE ${Sound_SOURCE} ${Sound_HEADER} )
endif()

ogrekit_add_library(OgreKitCore STATIC
			${Core_SOURCE}       ${Core_HEADER}
			${Logic_SOURCE}      ${Logic_HEADER}
			${Blend_SOURCE}      ${Blend_HEADER}
			${AI_SOURCE}         ${AI_HEADER}
			${Animation_SOURCE}  ${Animation_HEADER}
			${Constraint_SOURCE} ${Constraint_HEADER}
			${Physics_SOURCE}    ${Physics_HEADER}
			${Network_SOURCE}    ${Network_HEADER}
			${Graphics_SOURCE}    ${Graphics_HEADER}
			${NLogic_SOURCE}     ${NLogic_HEADER}
			${Thread_SOURCE}     ${Thread_HEADER}
			${Particles_SOURCE}  ${Particles_HEADER}
			${External_SOURCE}   ${External_HEADER}
			${HiddenDepenCMakeLists}
			${Extra_SOURCE}
			${Resources_FILES}
			${Script_SOURCE}     ${Script_HEADER}
			
			SEPARATE
			
			${Core_SOURCE_2}
			${Script_SOURCE_2}
			${Thread_SOURCE_2} 
	)

config_target_if_iphone_build(OgreKitCore TRUE)
