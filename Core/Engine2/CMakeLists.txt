# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

# ---------------------------------------------------------
# Core Files
# ---------------------------------------------------------

set(Core_SOURCE
    # ----- Source ----- 
    gkCamera.cpp
    gkConstraint.cpp
    gkCoreApplication.cpp
    gkEngine.cpp
    gkEntity.cpp
    gkGameObject.cpp
    gkGameObjectGroup.cpp
    gkLight.cpp
    gkLogger.cpp
    gkMathUtils.cpp
    gkObject.cpp
    gkPath.cpp
    gkTextFile.cpp
    gkTextManager.cpp
    gkRenderFactory.cpp
    gkScene.cpp
    gkSceneManager.cpp
    gkSerialize.cpp
    gkSkeleton.cpp
    gkUserDefs.cpp
    gkUtils.cpp
    gkWindowSystem.cpp
)

set(Core_HEADER
    # ----- Headers ----- 
    gkCamera.h
    gkCommon.h
    gkConstraint.h
    gkCoreApplication.h
    gkEngine.h
    gkEntity.h
    gkGameObject.h
    gkGameObjectGroup.h
    gkHashedString.h
    gkInput.h
    gkLight.h
    gkLogger.h
    gkMathUtils.h
    gkMemoryTest.h
    gkObject.h
    gkPath.h
    gkTextFile.h
    gkTextManager.h
    gkRect.h
    gkRenderFactory.h
    gkScene.h
    gkSceneManager.h
    gkSerialize.h
    gkSkeleton.h
    gkString.h
    gkTransformState.h
    gkUserDefs.h
    gkUtils.h
    gkVariable.h
    gkWindowSystem.h
)

source_group(Core\\Header FILES ${Core_HEADER})
source_group(Core\\Source FILES ${Core_SOURCE})

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
    Logic/gkAnimationNode.cpp
    Logic/gkExitNode.cpp
    Logic/gkExpressionNode.cpp
    Logic/gkGroupNode.cpp
    Logic/gkIfNode.cpp
    Logic/gkKeyNode.cpp
    Logic/gkNodeManager.cpp
    Logic/gkLogicNode.cpp
    Logic/gkLogicSocket.cpp
    Logic/gkLogicTree.cpp
    Logic/gkMathNode.cpp
    Logic/gkMotionNode.cpp
    Logic/gkMouseNode.cpp
    Logic/gkMouseButtonNode.cpp
    Logic/gkObjectNode.cpp
    Logic/gkPrintNode.cpp
    Logic/gkRandomNode.cpp
    Logic/gkSwitchNode.cpp
    Logic/gkTimerNode.cpp
    Logic/gkValueNode.cpp
    Logic/gkVariableNode.cpp
    Logic/gkArcBallNode.cpp
)

set(NLogic_HEADER
    # ----- Headers ----- 
    Logic/gkAnimationNode.h
    Logic/gkExitNode.h
    Logic/gkExpressionNode.h
    Logic/gkGroupNode.h
    Logic/gkIfNode.h
    Logic/gkKeyNode.h
    Logic/gkLogicCommon.h
    Logic/gkNodeManager.h
    Logic/gkLogicNode.h
    Logic/gkLogicSocket.h
    Logic/gkLogicTree.h
    Logic/gkMathNode.h
    Logic/gkMotionNode.h
    Logic/gkMouseNode.h
    Logic/gkMouseButtonNode.h
    Logic/gkObjectNode.h
    Logic/gkPrintNode.h
    Logic/gkRandomNode.h
    Logic/gkSwitchNode.h
    Logic/gkTimerNode.h
    Logic/gkValueNode.h
    Logic/gkVariableNode.h
    Logic/gkArcBallNode.h
)

source_group(LogicNodes\\Header FILES ${NLogic_HEADER})
source_group(LogicNodes\\Source FILES ${NLogic_SOURCE})

# ---------------------------------------------------------
# Logic Bricks
# ---------------------------------------------------------
set(Logic_SOURCE
    # ----- Source ----- 
    LogicBricks/gkLogicActuator.cpp
    LogicBricks/gkLogicBrick.cpp
    LogicBricks/gkLogicController.cpp
    LogicBricks/gkLogicDispatcher.cpp
    LogicBricks/gkLogicLink.cpp
    LogicBricks/gkLogicManager.cpp
    LogicBricks/gkLogicSensor.cpp
)

set(Logic_SSOURCE
    # ----- Sensors ----- 
    LogicBricks/gkAlwaysSensor.cpp
    LogicBricks/gkMouseSensor.cpp
)
set(Logic_CSOURCE
    # ----- Controllers ----- 
    LogicBricks/gkLogicOpController.cpp
)
set(Logic_ASOURCE
    # ----- Actuators ----- 
    LogicBricks/gkMotionActuator.cpp
    LogicBricks/gkStateActuator.cpp
)

set(Logic_HEADER
    # ----- Headers ----- 
    LogicBricks/gkLogicActuator.h
    LogicBricks/gkLogicBrick.h
    LogicBricks/gkLogicController.h
    LogicBricks/gkLogicDispatcher.h
    LogicBricks/gkLogicLink.h
    LogicBricks/gkLogicManager.h
    LogicBricks/gkLogicSensor.h
)

set(Logic_SHEADER
    # ----- Sensors ----- 
    LogicBricks/gkAlwaysSensor.h
    LogicBricks/gkMouseSensor.h
)
set(Logic_CHEADER
    # ----- Controllers ----- 
    LogicBricks/gkLogicOpController.h
)
set(Logic_AHEADER
    # ----- Actuators ----- 
    LogicBricks/gkMotionActuator.h
    LogicBricks/gkStateActuator.h
)

source_group(LogicBricks\\Header                FILES ${Logic_HEADER})
source_group(LogicBricks\\Header\\Sensors       FILES ${Logic_SHEADER})
source_group(LogicBricks\\Header\\Controllers   FILES ${Logic_CHEADER})
source_group(LogicBricks\\Header\\Actuators     FILES ${Logic_AHEADER})

source_group(LogicBricks\\Source                FILES ${Logic_SOURCE})
source_group(LogicBricks\\Source\\Sensors       FILES ${Logic_SSOURCE})
source_group(LogicBricks\\Source\\Controllers   FILES ${Logic_CSOURCE})
source_group(LogicBricks\\Source\\Actuators     FILES ${Logic_ASOURCE})


# ---------------------------------------------------------
# BlendFile Loaders
# ---------------------------------------------------------
set(Blend_SOURCE 
    # ----- Public Source ----- 
    Loaders/Blender/gkBlendFile.cpp
    Loaders/Blender/gkBlendLoader.cpp
    Loaders/Blender/gkLoaderUtils.cpp
    Loaders/Blender/gkMaterialLoader.cpp
    Loaders/Blender/gkMeshLoader.cpp
    Loaders/Blender/gkLogicLoader.cpp
    Loaders/Blender/gkObjectLoader.cpp
    Loaders/Blender/gkSceneLoader.cpp
    Loaders/Blender/gkSkeletonLoader.cpp
    Loaders/Blender/gkBlenderSkyBox.cpp
    Loaders/Blender/gkTextureLoader.cpp
)

set(Blend_HEADER
    # ----- Public Headers ----- 
    Loaders/Blender/gkBlendFile.h
    Loaders/Blender/gkBlendLoader.h
    Loaders/Blender/gkLoaderCommon.h
    Loaders/Blender/gkBlenderDefines.h
    Loaders/Blender/gkLoaderUtils.h
    Loaders/Blender/gkMaterialLoader.h
    Loaders/Blender/gkMeshLoader.h
    Loaders/Blender/gkMeshUtils.h
    Loaders/Blender/gkLogicLoader.h
    Loaders/Blender/gkObjectLoader.h
    Loaders/Blender/gkSceneLoader.h
    Loaders/Blender/gkSkeletonLoader.h
    Loaders/Blender/gkBlenderSkyBox.h
    Loaders/Blender/gkTextureLoader.h
)

source_group(Blender\\Source FILES ${Blend_SOURCE})
source_group(Blender\\Header FILES ${Blend_HEADER})


# ---------------------------------------------------------
# Constraints 
# ---------------------------------------------------------
set(Constraint_SOURCE
    # ----- Source ----- 
    Constraints/gkLimitLocConstraint.cpp
    Constraints/gkLimitRotConstraint.cpp
)

set(Constraint_HEADER 
    # ----- Header ----- 
    Constraints/gkLimitLocConstraint.h
    Constraints/gkLimitRotConstraint.h
)

source_group(Constraints\\Header FILES ${Constraint_HEADER})
source_group(Constraints\\Source FILES ${Constraint_SOURCE})


# ---------------------------------------------------------
# Physics 
# ---------------------------------------------------------
set(Physics_SOURCE
    # ----- Source ----- 
	Physics/gkRigidBody.cpp
	Physics/gkDynamicsWorld.cpp
	Physics/gkRagDoll.cpp
	Physics/gkCharacter.cpp
	Physics/gkVehicle.cpp
	Physics/gkSoftBody.cpp
	Physics/gkPhysicsDebug.cpp
)

set(Physics_HEADER 
    # ----- Header ----- 
	Physics/gkRigidBody.h
	Physics/gkDynamicsWorld.h
	Physics/gkRagDoll.h
	Physics/gkCharacter.h
	Physics/gkVehicle.h
	Physics/gkSoftBody.h
	Physics/gkPhysicsDebug.h
)


source_group(Physics\\Header FILES ${Physics_HEADER})
source_group(Physics\\Source FILES ${Physics_SOURCE})

# ---------------------------------------------------------
# Script Files
# ---------------------------------------------------------
set(Script_SOURCE
    # ----- Source ----- 
    Script/gkLuaManager.cpp
    Script/gkLuaType.cpp
    Script/gkLuaSystem.cpp  
)

set(Script_HEADER 
    # ----- Header ----- 
    Script/gkLuaManager.h
    Script/gkLuaType.h
    Script/gkLuaSystem.h
)


source_group(Script\\Header FILES ${Script_HEADER})
source_group(Script\\Source FILES ${Script_SOURCE})


# ---------------------------------------------------------
# Sound Files
# ---------------------------------------------------------
set(Sound_SOURCE
    # ----- Source ----- 
    Sound/gkSoundManager.cpp
)

set(Sound_HEADER 
    # ----- Header ----- 
    Sound/gkSoundManager.h
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
# 2D Graphics Files
# ---------------------------------------------------------
set(Graphics_SOURCE
    # ----- Source ----- 
    Graphics/gk2DContext.cpp
    Graphics/gk2DFont.cpp
    Graphics/gk2DImage.cpp
    Graphics/gk2DPaint.cpp
    Graphics/gk2DPath.cpp
    Graphics/gk2DRenderer.cpp
    Graphics/gk2DVertex.cpp
)


set(Graphics_HEADER
    # ----- Headers ----- 
    Graphics/gk2DCommon.h
    Graphics/gk2DContext.h
    Graphics/gk2DFont.h
    Graphics/gk2DImage.h
    Graphics/gk2DPaint.h
    Graphics/gk2DPath.h
    Graphics/gk2DRenderer.h
    Graphics/gk2DVertex.h
)

source_group(Graphics\\Header FILES ${Graphics_HEADER})
source_group(Graphics\\Source FILES ${Graphics_SOURCE})


# ---------------------------------------------------------
# Build setup
# ---------------------------------------------------------
if (APPLE)
include_directories( ${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX )
endif ()

include_directories(
    ${OGREKIT_OGRE_INCLUDE} 
    ${OGREKIT_OIS_INCLUDE}
    ${OGREKIT_LUA_INCLUDE}
    ${GAMEKIT_SERIALIZE_BLENDER}
    ${GAMEKIT_SERIALIZE_BULLET}
    ${GAMEKIT_SERIALIZE_BLENDER_HEADERS}
    ./ 
    ./Animation
    ./Constraints
    ./LogicBricks
    ./Logic
    ./Script 
    ./Sound 
    ./Loaders/Blender
    ./Physics
    ./Utils/Src
    )


link_libraries(
    ${OGREKIT_OGRE_LIBS}
    ${GAMEKIT_SERIALIZE_BLENDER_TARGET}
    ${GAMEKIT_SERIALIZE_BULLET_TARGET}
    ${OGREKIT_BULLET_LIBS}
    Lua
    OIS
    )

link_directories(${OGRE_LIBDIR})
include_directories(${OGREKIT_BULLET_INCLUDE})

configure_rendersystem()


add_library(OgreKitCore STATIC
            ${Core_SOURCE}       ${Core_HEADER}
            ${Logic_SOURCE}      ${Logic_HEADER}
            ${Logic_SSOURCE}     ${Logic_SHEADER}
            ${Logic_CSOURCE}     ${Logic_CHEADER}
            ${Logic_ASOURCE}     ${Logic_AHEADER}
            ${Blend_SOURCE}      ${Blend_HEADER}
            ${Animation_SOURCE}  ${Animation_HEADER}
            ${Constraint_SOURCE} ${Constraint_HEADER}
            ${Physics_SOURCE}    ${Physics_HEADER}
            ${Script_SOURCE}     ${Script_HEADER}
            ${Sound_SOURCE}      ${Sound_HEADER}
            ${Network_SOURCE}    ${Network_HEADER}
            ${Graphics_SOURCE}   ${Graphics_HEADER}
            ${NLogic_SOURCE}     ${NLogic_HEADER}
    )
