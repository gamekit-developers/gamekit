# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

#SET(CMAKE_SUPPRESS_REGENERATION  1)
#SET(CMAKE_USE_RELATIVE_PATHS 1)
#SET(INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES 1)

project(OGREKIT)
SET(OGREKIT 1.0)
set(OGRE_STATIC TRUE)

if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

if (MSVC)
	add_definitions( -D_CRT_SECURE_NO_WARNINGS )
	add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
	add_definitions( -DNOMINMAX)
endif()

# ---------------------------------------------------------
set(CMAKE_MODULE_PATH
    ${OGREKIT_SOURCE_DIR}/CMake
    ${OGREKIT_SOURCE_DIR}/CMake/Utils
    ${OGREKIT_SOURCE_DIR}/CMake/Packages
    ${OGREKIT_SOURCE_DIR}/CMake/Templates
    )
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)
include(ConfigOgreKit)
include(ConfigGameKit)
include(ConfigIPhone)
include(OgreAddTargets)

configure_gamekit(${OGREKIT_SOURCE_DIR}/Dependencies/Source)

 set(GAMEKIT_GRAPHIC_BACKEND 1 CACHE STRING 
	"Select the graphic backend for GameKit. Possible values:
		 1 - Ogre 1.8
		 2 - Ogre 1.7
		 3 - GLUT
	"
	)
	
if (GAMEKIT_GRAPHIC_BACKEND EQUAL 1)
	set(GAMEKIT_GRAPHIC_BACKEND_DIR Ogre-1.8)	
elseif (GAMEKIT_GRAPHIC_BACKEND EQUAL 2)	
	set(GAMEKIT_GRAPHIC_BACKEND_DIR Ogre-1.7)
else()
	message(FATAL_ERROR "Invalid graphice backend or not supported yet.")
endif()

message(STATUS "-----------------------------------------------------------------------------")
message(STATUS "-- Graphic backend is " ${GAMEKIT_GRAPHIC_BACKEND_DIR})
message(STATUS "-----------------------------------------------------------------------------")

configure_ogrekit(${OGREKIT_SOURCE_DIR} ${OGREKIT_SOURCE_DIR}/${GAMEKIT_GRAPHIC_BACKEND_DIR})


set(OGREKIT_BULLET_LIBS BulletDynamics BulletCollision  LinearMath)	
set(OGREKIT_BULLET_INCLUDE ${OGREKIT_SOURCE_DIR}/bullet/src)
if (OGREKIT_COMPILE_SOFTBODY)
	list(APPEND OGREKIT_BULLET_LIBS BulletSoftBody)
endif()

if (NOT APPLE AND OGREKIT_COMPILE_WXWIDGETS)
	include(wxSetup)
	configure_wx_build(${OGREKIT_SOURCE_DIR})
	setup_wx()
endif()



if (APPLE)
    if (${XCODE_VERSION} STRGREATER "4.0.0")
        set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "4.2")
    endif()
endif()


IF(MSVC)

	#We statically link to reduce dependancies
	FOREACH(flag_var    CMAKE_CXX_FLAGS 
	                    CMAKE_CXX_FLAGS_DEBUG 
	                    CMAKE_CXX_FLAGS_RELEASE 
	                    CMAKE_CXX_FLAGS_MINSIZEREL 
	                    CMAKE_CXX_FLAGS_RELWITHDEBINFO
	                    CMAKE_C_FLAGS_DEBUG
	                    CMAKE_C_FLAGS_RELEASE
	                    CMAKE_C_FLAGS_MINSIZEREL 
	                    CMAKE_C_FLAGS_RELWITHDEBINFO
	                    )
		IF(${flag_var} MATCHES "/MD")
			STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
		ENDIF(${flag_var} MATCHES "/MD")
		IF(${flag_var} MATCHES "/MDd")
			STRING(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
		ENDIF(${flag_var} MATCHES "/MDd")
	ENDFOREACH(flag_var)

ENDIF(MSVC)


# ---------------------------------------------------------
# Common base paths
set(OGREKIT_INCLUDE ${OGREKIT_OGRE_INCLUDE}
                    ${OGREKIT_SOURCE_DIR}/Engine
                    ${OGREKIT_SOURCE_DIR}/Engine/AI
                    ${OGREKIT_BULLET_INCLUDE}
                    ${OGREKIT_DEP_INCLUDE}
                    )

set(OGREKIT_LIB OgreKitCore ${OGREKIT_OGRE_LIBS})


subdirs(bullet)
subdirs(Dependencies)
subdirs(${GAMEKIT_GRAPHIC_BACKEND_DIR})
subdirs(Engine)

if (NOT OGREKIT_USE_BPARSE OR OGREKIT_USE_FILETOOLS)
	subdirs(Tools/FileTools)
endif()

if (OGREKIT_COMPILE_WXWIDGETS)
	subdirs(${OGREKIT_SOURCE_DIR}/${WX_VERSION})
endif()

subdirs(Samples)

if (OGREKIT_UPDATE_CPP_DOCS)
	subdirs(Docs/CppAPI)
endif()

if (OGREKIT_UPDATE_LUA_DOCS)
	subdirs(Docs/LuaAPI)
endif()

if (OGREKIT_UPDATE_FBT_DOCS)
	subdirs(Docs/FbtAPI)
endif()

if (OGREKIT_ENABLE_UNITTESTS)
	subdirs(UnitTests)
endif()
