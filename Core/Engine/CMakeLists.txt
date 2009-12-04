# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

# ---------------------------------------------------------
# Core Files
# ---------------------------------------------------------

set(Core_SOURCE
    # ----- Source ----- 
    gkCameraObject.cpp
    gkConsole.cpp
    gkConstraint.cpp
    gkDebugPage.cpp
    gkEngine.cpp
    gkEntityObject.cpp
    gkGameObject.cpp
    gkGameObjectGroup.cpp
    gkInternalWindowSystem.cpp
    gkLightObject.cpp
    gkLogger.cpp
    gkMathUtils.cpp
    gkPath.cpp
    gkRenderFactory.cpp
    gkSceneObject.cpp
    gkSceneObjectManager.cpp
    gkUserDefs.cpp
    gkUtils.cpp
    gkWindowSystem.cpp
)

set(Core_HEADER
    # ----- Headers ----- 
    gkArray.h
    gkCameraObject.h
    gkCameraObjectTypes.h
    gkCommon.h
    gkConsole.h
    gkConstraint.h
    gkDebugPage.h
    gkEngine.h
    gkEntityObject.h
    gkEntityObjectTypes.h
    gkGameObject.h
    gkGameObjectGroup.h
    gkGameObjectTypes.h
    gkHash.h
    gkHashTable.h
    gkHashedString.h
    gkInternalWindowSystem.h
    gkKeyCodes.h
    gkKeyboard.h
    gkLightObject.h
    gkLightObjectTypes.h
    gkList.h
    gkLogger.h
    gkManualLoader.h
    gkMathUtils.h
    gkMemoryTest.h
    gkMouse.h
    gkPath.h
    gkRect.h
    gkRenderFactory.h
    gkSceneObject.h
    gkSceneObjectManager.h
    gkSceneObjectTypes.h
    gkStack.h
    gkTransformState.h
    gkUserDefs.h
    gkUtils.h
    gkVariable.h
    gkWindowSystem.h
    # ----- Inlines ----- 
    gkMacros.inl
    gkWarningTemp.inl
)

source_group(Core\\Header FILES ${Core_HEADER})
source_group(Core\\Source FILES ${Core_SOURCE})


# ---------------------------------------------------------
# Logic Files
# ---------------------------------------------------------
set(Logic_SOURCE
    # ----- Source ----- 
    Logic/gkAnimationNode.cpp
    Logic/gkExitNode.cpp
    Logic/gkExpressionNode.cpp
    Logic/gkGroupNode.cpp
    Logic/gkIfNode.cpp
    Logic/gkKeyNode.cpp
    Logic/gkLogicManager.cpp
    Logic/gkLogicNode.cpp
    Logic/gkLogicSocket.cpp
    Logic/gkLogicTree.cpp
    Logic/gkMathNode.cpp
    Logic/gkMotionNode.cpp
    Logic/gkMouseNode.cpp
    Logic/gkObjectNode.cpp
    Logic/gkPrintNode.cpp
    Logic/gkRandomNode.cpp
    Logic/gkSwitchNode.cpp
    Logic/gkTimerNode.cpp
    Logic/gkValueNode.cpp
    Logic/gkVariableNode.cpp
)

set(Logic_HEADER
    # ----- Headers ----- 
    Logic/gkAnimationNode.h
    Logic/gkExitNode.h
    Logic/gkExpressionNode.h
    Logic/gkGroupNode.h
    Logic/gkIfNode.h
    Logic/gkKeyNode.h
    Logic/gkLogicCommon.h
    Logic/gkLogicManager.h
    Logic/gkLogicNode.h
    Logic/gkLogicSocket.h
    Logic/gkLogicTree.h
    Logic/gkMathNode.h
    Logic/gkMotionNode.h
    Logic/gkMouseNode.h
    Logic/gkObjectNode.h
    Logic/gkPrintNode.h
    Logic/gkRandomNode.h
    Logic/gkSwitchNode.h
    Logic/gkTimerNode.h
    Logic/gkValueNode.h
    Logic/gkVariableNode.h
)


source_group(Logic\\Header FILES ${Logic_HEADER})
source_group(Logic\\Source FILES ${Logic_SOURCE})


# ---------------------------------------------------------
# BlendFile Loaders
# ---------------------------------------------------------
set(Blend_SOURCE 
    # ----- Public Source ----- 
    Loaders/gkBlendFile.cpp
    Loaders/gkBlendLoader.cpp
)

set(Blend_SOURCE_INTERN
    # ----- Internal Source ----- 
    Loaders/Internal/gkLoaderUtils.cpp
    Loaders/Internal/gkMaterialLoader.cpp
    Loaders/Internal/gkMeshLoader.cpp
    Loaders/Internal/gkObjectLoader.cpp
    Loaders/Internal/gkSceneLoader.cpp
    Loaders/Internal/gkSkeletonLoader.cpp
    Loaders/Internal/gkTextureLoader.cpp
)

set(Blend_HEADER
    # ----- Public Headers ----- 
    Loaders/gkBlendFile.h
    Loaders/gkBlendLoader.h
    Loaders/gkLoaderCommon.h
)

set(Blend_HEADER_INTERN
    # ----- Internal Headers ----- 
    Loaders/Internal/gkBlenderDefines.h
    Loaders/Internal/gkFaceBufferObject.h
    Loaders/Internal/gkLoaderUtils.h
    Loaders/Internal/gkMaterialLoader.h
    Loaders/Internal/gkMeshLoader.h
    Loaders/Internal/gkMeshUtils.h
    Loaders/Internal/gkObjectLoader.h
    Loaders/Internal/gkSceneLoader.h
    Loaders/Internal/gkSkeletonLoader.h
    Loaders/Internal/gkSubMeshBuffer.h
    Loaders/Internal/gkTextureLoader.h
    Loaders/Internal/gkVertexBufferObject.h
)


source_group(Blend\\Header FILES ${Blend_HEADER})
source_group(Blend\\Header\\Internal FILES ${Blend_HEADER_INTERN})
source_group(Blend\\Source FILES ${Blend_SOURCE})
source_group(Blend\\Source\\Internal FILES ${Blend_SOURCE_INTERN})


# ---------------------------------------------------------
# Property Script (custom ogre material scripts)
# ---------------------------------------------------------
set(PropertyScript_SOURCE
    # ----- Source ----- 
    PropertyScript/gkPropertyAttribute.cpp
    PropertyScript/gkPropertyNode.cpp
    PropertyScript/gkPropertyScript.cpp
    PropertyScript/gkPropertyScanner.cpp
    PropertyScript/gkPropertyToken.cpp
    PropertyScript/gkPropertyTree.cpp
)

set(PropertyScript_HEADER 
    # ----- Header ----- 
    PropertyScript/gkPropertyAttribute.h
    PropertyScript/gkPropertyNode.h
    PropertyScript/gkPropertyCommon.h
    PropertyScript/gkPropertyScript.h
    PropertyScript/gkPropertyScanner.h
    PropertyScript/gkPropertyToken.h
    PropertyScript/gkPropertyTree.h
)

source_group(PropertyScript\\Header FILES ${PropertyScript_HEADER})
source_group(PropertyScript\\Source FILES ${PropertyScript_SOURCE})



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
# Script Files (TODO) Python, Squirrel.
# ---------------------------------------------------------
set(Script_SOURCE
    # ----- Source ----- 
    Script/gkScriptManager.cpp
)

set(Script_HEADER 
    # ----- Header ----- 
    Script/gkScriptManager.h
)

source_group(Script\\Header FILES ${Script_HEADER})
source_group(Script\\Source FILES ${Script_SOURCE})


# ---------------------------------------------------------
# Sound Files (TODO) OpenAL .. Others ?
# ---------------------------------------------------------
set(Sound_SOURCE
    # ----- Source ----- 
    Sound/gkSoundManager.cpp
    Sound/gkSoundObject.cpp
)

set(Sound_HEADER 
    # ----- Header ----- 
    Sound/gkSoundManager.h
    Sound/gkSoundObject.h
)

source_group(Sound\\Header FILES ${Sound_HEADER})
source_group(Sound\\Source FILES ${Sound_SOURCE})



# ---------------------------------------------------------
# Build setup
# ---------------------------------------------------------

include_directories(
    ${OGREKIT_OGRE_INCLUDE} 
    ${OGREKIT_OIS_INCLUDE} 
    ${OGREKIT_BPARSE_INCLUDE} 
    ./ 
    Logic 
    Script 
    Sound 
    Loaders
    )

link_libraries(
    ${OGREKIT_OGRE_LIBS}
    ${OGREKIT_BPARSE_TARGET}
    OIS
    )

link_directories(${OGRE_LIBDIR})

if (OGRE_BUILD_RENDERSYSTEM_GL)

    add_definitions(-DOGREKIT_GLRS)
    
    include_directories(
        ${OGREKIT_GLRS_ROOT}/include
        ${OGREKIT_GLRS_ROOT}/src/GLSL/include
        ${OGREKIT_GLRS_ROOT}/src/atifs/include
    )
    
    link_libraries(
        ${OGREKIT_GLRS_LIBS} 
        ${OPENGL_gl_LIBRARY}
        ${OPENGL_glu_LIBRARY}
    )
        
endif (OGRE_BUILD_RENDERSYSTEM_GL)


add_library(OgreKitCore ${OGREKIT_LIB_TYPE} 
    ${Core_SOURCE} 
    ${Core_HEADER}
    ${Logic_HEADER}
    ${Logic_SOURCE}
    ${Blend_HEADER}
    ${Blend_HEADER_INTERN}
    ${Blend_SOURCE}
    ${Blend_SOURCE_INTERN}
    ${Constraint_HEADER}
    ${Constraint_SOURCE}
    ${Script_HEADER}
    ${Script_SOURCE}
    ${Sound_HEADER}
    ${Sound_SOURCE}
    ${PropertyScript_HEADER}
    ${PropertyScript_SOURCE}
    )

set(HomeFiles
    User/OgreKitStartup.conf
)

install(FILES ${HomeFiles} DESTINATION ${OGREKIT_INSTALL_PREFIX})
