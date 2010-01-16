

set(OgreKitSRC
	OgreKitApplication.cpp
	OgreKitPlugins.cpp
	OgreKitPrefs.cpp
	OgreBlend.cpp
	BulletBlendReaderNew.cpp
	)

set(OgreKitHDR
	OgreKitApplication.h
	OgreKitPlugins.h
	OgreKitPrefs.h
	OgreKitKeyboard.h
	OgreKitMouse.h
	OgreBlend.h
	BulletBlendReaderNew.h
	)


###############################################################################
set(OgreKitUtilsSRC
	Utils/OgreMathUtils.cpp
	Utils/OgrePrint.cpp
	Utils/OgreUtils.cpp
)

set(OgreKitUtilsHDR
	Utils/OgreArray.h
	Utils/OgreFixedString.h
	Utils/OgreHash.h
	Utils/OgreHashedString.h
	Utils/OgreHashTable.h
	Utils/OgreIntTypes.h
	Utils/OgreList.h
	Utils/OgreMathUtils.h
	Utils/OgreMemUtils.h
	Utils/OgrePrint.h
	Utils/OgreStringDefines.h
	Utils/OgreStack.h
	Utils/OgreUtilsAllocator.h
	Utils/OgreUtilsCommon.h
	Utils/OgreUtils.h
	)

###############################################################################
set(OgreKitLoaderSRC
	Loader/OgreBlendConverter.cpp
	Loader/Blender/OgreLoaderUtils.cpp
	Loader/Blender/OgreMaterialLoader.cpp
	Loader/Blender/OgreMeshLoader.cpp
	Loader/Blender/OgreSkeletonLoader.cpp
	Loader/Blender/OgreTextureLoader.cpp
)

set(OgreKitLoaderHDR
	Loader/OgreBlendConverter.h
	Loader/Blender/OgreBlenderDefines.h
	Loader/Blender/OgreFaceBufferObject.h
	Loader/Blender/OgreLoaderUtils.h
	Loader/Blender/OgreMaterialLoader.h
	Loader/Blender/OgreMeshLoader.h
	Loader/Blender/OgreMeshUtils.h
	Loader/Blender/OgreSkeletonLoader.h
	Loader/Blender/OgreSubMeshBuffer.h
	Loader/Blender/OgreTextureLoader.h
	Loader/Blender/OgreVertexBufferObject.h
	)

include_directories(
	${OGREKIT_OGRE_INCLUDE}
	${OGREKIT_BULLET_INCLUDE}
	${GAMEKIT_SERIALIZE_COMMON}
	${GAMEKIT_SERIALIZE_BLENDER}
	${OGREKIT_OIS_INCLUDE}
	Loader
	Loader/Blender
	Utils
	)

link_libraries(
	${OGREKIT_OGRE_LIBS}
	${OGREKIT_BULLET_LIBS}
	${GAMEKIT_SERIALIZE_COMMON_TARGET}
	${GAMEKIT_SERIALIZE_BLENDER_TARGET}
	OIS
)

configure_rendersystem()
add_library(OgreKit
                    ${OgreKitSRC}
                    ${OgreKitHDR}
                    ${OgreKitUtilsSRC}
                    ${OgreKitUtilsHDR}
                    ${OgreKitLoaderSRC}
                    ${OgreKitLoaderHDR}
                    )
