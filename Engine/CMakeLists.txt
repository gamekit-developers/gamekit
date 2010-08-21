# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

# ---------------------------------------------------------
# Core Files
# ---------------------------------------------------------

set(Core_SOURCE
    # ----- Source -----
    gkCamera.cpp
    gkCam2ViewportRay.cpp
    gkCoreApplication.cpp
    gkDebugFps.cpp
    gkDebugger.cpp
    gkDebugProperty.cpp
    gkDebugScreen.cpp
    gkEngine.cpp
    gkEntity.cpp
    gkGameObject.cpp
    gkGameObjectGroup.cpp
    gkGameObjectInstance.cpp
    gkGroupManager.cpp
    gkLight.cpp
    gkLogger.cpp
    gkMesh.cpp
    gkMeshManager.cpp
    gkMessageManager.cpp
    gkMathUtils.cpp
    gkObject.cpp
    gkPath.cpp
    gkTextFile.cpp
    gkTickState.cpp
    gkTextManager.cpp
    gkTickState.cpp
    gkRenderFactory.cpp
    gkScene.cpp
    gkSceneManager.cpp
    gkSkeleton.cpp
    gkStats.cpp
    gkUserDefs.cpp
    gkUtils.cpp
    gkWindowSystem.cpp
    gkVariable.cpp
    Utils/utString.cpp
    Utils/utStreams.cpp
    Utils/utRandom.cpp
)

set(Core_HEADER
    # ----- Headers -----
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
    gkGameObject.h
    gkGameObjectGroup.h
    gkGameObjectInstance.h
    gkGroupManager.h
    gkHashedString.h
    gkInput.h
    gkLight.h
    gkLogger.h
    gkMesh.h
    gkMeshManager.h
    gkMessageManager.h
    gkMathUtils.h
    gkMemoryTest.h
    gkObject.h
    gkPath.h
    gkTextFile.h
    gkTickState.h
    gkTextManager.h
    gkTickState.h
    gkRenderFactory.h
    gkScene.h
    gkSceneManager.h
    gkSerialize.h
    gkSkeleton.h
    gkStats.h
    gkString.h
    gkTransformState.h
    gkUserDefs.h
    gkUtils.h
    gkValue.h
    gkVariable.h
    gkWindowSystem.h
    OgreKit.h
    Utils/utCommon.h
    Utils/utString.h
    Utils/utTypes.h
    Utils/utStreams.h
    Utils/utMemoryPool.h
    Utils/utStreams.h
    Utils/utRandom.h
)

source_group(Core\\Header FILES ${Core_HEADER})
source_group(Core\\Source FILES ${Core_SOURCE})

# ---------------------------------------------------------
# AI Files
# ---------------------------------------------------------

set(AI_SOURCE
    # ----- Source -----
    AI/gkRecast.cpp
    AI/gkNavMeshData.cpp
    AI/gkNavPath.cpp
    AI/gkSteering.cpp
    AI/gkSteeringObject.cpp
    AI/gkSteeringCapture.cpp
    AI/gkSteeringPathFollowing.cpp
    AI/gkSteeringWander.cpp
    AI/gkSceneObstacle.cpp
    AI/gkFSM.cpp
)

set(AI_HEADER
    # ----- Headers -----
    AI/gkMeshData.h
    AI/gkRecast.h
    AI/gkNavMeshData.h
    AI/gkNavPath.h
    AI/gkSteering.h
    AI/gkSteeringObject.h
    AI/gkSteeringCapture.h
    AI/gkSteeringPathFollowing.h
    AI/gkSteeringWander.h
    AI/gkSceneObstacle.h
    AI/gkFSM.h
)

source_group(AI\\Header FILES ${AI_HEADER})
source_group(AI\\Source FILES ${AI_SOURCE})

# ---------------------------------------------------------
# Thread Files
# ---------------------------------------------------------

set(Thread_SOURCE
    # ----- Source -----
    Thread/gkActiveObject.cpp
    Thread/gkCriticalSection.cpp
    Thread/gkPtrRef.cpp
    Thread/gkSyncObj.cpp
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

source_group(Thread\\Header FILES ${Thread_HEADER})
source_group(Thread\\Source FILES ${Thread_SOURCE})


# ---------------------------------------------------------
# Animation Files
# ---------------------------------------------------------



set(Animation_SOURCE
    # ----- Source -----
    Animation/gkAction.cpp
    Animation/gkActionBake.cpp
    Animation/gkActionChannel.cpp
    Animation/gkActionManager.cpp
    Animation/gkBezierSpline.cpp
    Animation/gkLightAction.cpp
    Animation/gkMaterialAction.cpp
    Animation/gkObjectAction.cpp
    Animation/gkWorldAction.cpp
)


set(Animation_HEADER
    # ----- Headers -----
    Animation/gkAction.h
    Animation/gkActionBake.h
    Animation/gkActionChannel.h
    Animation/gkActionManager.h
    Animation/gkBezierSpline.h
    Animation/gkLightAction.h
    Animation/gkMaterialAction.h
    Animation/gkObjectAction.h
    Animation/gkWorldAction.h
)


source_group(Animation\\Header FILES ${Animation_HEADER})
source_group(Animation\\Source FILES ${Animation_SOURCE})


# ---------------------------------------------------------
# Logic Nodes
# ---------------------------------------------------------
set(NLogic_SOURCE
    # ----- Source -----
    Logic/gkButtonNode.cpp
    Logic/gkCameraNode.cpp
    Logic/gkCharacterNode.cpp
    Logic/gkCollisionNode.cpp
    Logic/gkExitNode.cpp
    Logic/gkExpressionNode.cpp
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
    Logic/gkSwitchNode.cpp
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
)

source_group(LogicNodes\\Header FILES ${NLogic_HEADER})
source_group(LogicNodes\\Source FILES ${NLogic_SOURCE})

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

source_group(LogicBricks\\Header                FILES ${Logic_HEADER})
source_group(LogicBricks\\Source                FILES ${Logic_SOURCE})

# ---------------------------------------------------------
# BlendFile Loaders
# ---------------------------------------------------------
set(Blend_SOURCE
    # ----- Public Source -----
    Loaders/Blender2/gkBlendFile.cpp
    Loaders/Blender2/gkBlendLoader.cpp
    Loaders/Blender2/gkTextureLoader.cpp
    Loaders/Blender2/gkBlenderSceneConverter.cpp
)

set(Blend_HEADER
    # ----- Public Headers -----
    Loaders/Blender2/gkBlendFile.h
    Loaders/Blender2/gkBlendLoader.h
    Loaders/Blender2/gkLoaderCommon.h
    Loaders/Blender2/gkBlenderDefines.h
    Loaders/Blender2/gkTextureLoader.h
    Loaders/Blender2/gkBlenderSceneConverter.h
)

source_group(Blender\\Source FILES ${Blend_SOURCE})
source_group(Blender\\Header FILES ${Blend_HEADER})


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

source_group(Constraints\\Header FILES ${Constraint_HEADER})
source_group(Constraints\\Source FILES ${Constraint_SOURCE})


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


source_group(Physics\\Header FILES ${Physics_HEADER})
source_group(Physics\\Source FILES ${Physics_SOURCE})

# ---------------------------------------------------------
# Script Files
# ---------------------------------------------------------

if (OGREKIT_COMPLIE_SWIG AND SWIG_FOUND)


    set(ScriptAPI2_INCLUDES
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
        Script/Api/gsSound.i
        Script/Api/gsThread.i
        Script/Api/gsTypes.i
        Script/Api/gsUtils.i
    )

    set(TMPL_API_SOURCE
        Script/Api/System.lua
        Script/Api/Class.lua
    )
    add_templates(GEN_TEMPLATES_OUT Script/Api/Generated/gsTemplates.h ${TMPL_API_SOURCE})

    set(ScriptAPI2_HEADER
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
        Script/Api/gsSound.h
        Script/Api/gsThread.h
        Script/Api/gsUtils.h
        ${ScriptAPI2_INCLUDES}
        ${GEN_TEMPLATES_OUT}
    )

    set(SWIG_INPUT
        Script/Api/OgreKit.i
    )


    swig_source_lua(SWIG_API2_OUTPUT_LUA ScriptAPI2_HEADER ${SWIG_INPUT})


    set(ScriptAPI2_SOURCE
        ${TMPL_API_SOURCE}
        ${SWIG_API2_OUTPUT_LUA}
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
        Script/Api/gsSound.cpp
        Script/Api/gsThread.cpp
        Script/Api/gsUtils.cpp
    )

    source_group(Script\\Builtin   FILES ${TMPL_API_SOURCE})
    source_group(Script\\Includes  FILES ${SWIG_INPUT} ${ScriptAPI2_INCLUDES})
    source_group(Script\\Header    FILES ${ScriptAPI2_HEADER})
    source_group(Script\\Source    FILES ${ScriptAPI2_SOURCE})


else()

    set(ScriptAPI2_INCLUDES
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
        Script/Api/gsSound.i
        Script/Api/gsThread.i
        Script/Api/gsTypes.i
        Script/Api/gsUtils.i
    )

    set(ScriptAPI2_HEADER
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
        Script/Api/gsSound.h
        Script/Api/gsThread.h
        Script/Api/gsUtils.h
        ${ScriptAPI2_INCLUDES}
    )


    set(ScriptAPI2_SOURCE
        Script/Api/Generated/OgreKitLua.cpp
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
        Script/Api/gsSound.cpp
        Script/Api/gsThread.cpp
        Script/Api/gsUtils.cpp
    )

    set(TMPL_API_SOURCE
        Script/Api/System.lua
        Script/Api/Class.lua
    )
    
    source_group(Script\\Builtin	FILES ${TMPL_API_SOURCE})
    source_group(Script\\Header    FILES ${ScriptAPI2_HEADER})
    source_group(Script\\Source    FILES ${ScriptAPI2_SOURCE})

endif()

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

source_group(Sound\\Header FILES ${Sound_HEADER})
source_group(Sound\\Source FILES ${Sound_SOURCE})




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

source_group(Network\\Header FILES ${Network_HEADER})
source_group(Network\\Source FILES ${Network_SOURCE})

# ---------------------------------------------------------
# Build setup
# ---------------------------------------------------------
if (APPLE)
include_directories( ${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX )
endif ()

if (OGREKIT_OPENAL_SOUND AND OPENAL_FOUND)
    add_definitions(-DOGREKIT_OPENAL_SOUND)
endif()


include_directories(
    ${OGREKIT_OGRE_INCLUDE}
    ${OGRELITE_SOURCE_DIR}/PlugIns/ParticleFX/include
    ${OGREKIT_RECAST_INCLUDE}
    ${OGREKIT_DETOUR_INCLUDE}
    ${OGREKIT_OPENSTEER_INCLUDE}
    ${OGREKIT_OIS_INCLUDE}
    ${OGREKIT_ZLIB_INCLUDE}
    ${OGREKIT_LUA_INCLUDE}
    ${OGREKIT_OPENAL_INCLUDE}
    ${GAMEKIT_SERIALIZE_BLENDER}
    ${GAMEKIT_SERIALIZE_BULLET}
    ${GAMEKIT_SERIALIZE_BLENDER_HEADERS}
    ./
    ./AI
    ./Animation
    ./Constraints
    ./LogicBricks
    ./Logic
    ./Script
    ./Script/Common # Depreciated Use OgreKitLua Api
    ./Script/Api
    ./Sound
    ./Loaders/Blender2
    ./Physics
    ./Utils
    ./Thread
    )

link_libraries(
    ${OGREKIT_OGRE_LIBS}
    Plugin_ParticleFX
    ${GAMEKIT_SERIALIZE_BLENDER_TARGET}
    ${GAMEKIT_SERIALIZE_BULLET_TARGET}
    ${OGREKIT_BULLET_LIBS}
    OIS
    )

link_directories(${OGRE_LIBDIR})
include_directories(${OGREKIT_BULLET_INCLUDE})

configure_rendersystem()

set(HiddenDepenCMakeLists ../Dependencies/CMakeLists.txt)
source_group(DependenciesCMakeLists FILES ${HiddenDepenCMakeLists})


# for swig no __declspec(dllexport)
add_definitions(-DSTATIC_LINKED)


if (OGREKIT_OPENAL_SOUND AND OPENAL_FOUND)

    set(Extra_SOURCE
        ${Sound_SOURCE} ${Sound_HEADER}
        )

    include_directories( ${OGREKIT_OGGVORBIS_INCLUDE})
    link_libraries(OggVorbis)
endif()


add_library(OgreKitCore STATIC
            ${Core_SOURCE}       ${Core_HEADER}
            ${Logic_SOURCE}      ${Logic_HEADER}
            ${Blend_SOURCE}      ${Blend_HEADER}
            ${AI_SOURCE}         ${AI_HEADER}
            ${Animation_SOURCE}  ${Animation_HEADER}
            ${Constraint_SOURCE} ${Constraint_HEADER}
            ${Physics_SOURCE}    ${Physics_HEADER}
            ${Script_SOURCE}     ${Script_HEADER} ${ScriptAPI2_SOURCE} ${ScriptAPI2_HEADER}
            ${Network_SOURCE}    ${Network_HEADER}
            ${NLogic_SOURCE}     ${NLogic_HEADER}
            ${Thread_SOURCE}     ${Thread_HEADER}
            ${HiddenDepenCMakeLists}
            ${Extra_SOURCE}
    )
