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
	add_definitions( -D_WINSOCKAPI_ )

	add_definitions( -D_CRT_SECURE_NO_WARNINGS )
	add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
	add_definitions( -DNOMINMAX)

	add_definitions( -D_VARIADIC_MAX=10 ) #vs2012 tuple issue with gtest
endif()

if (OGREKIT_BUILD_IPHONE)    
	add_definitions(-DARM_NEON_GCC_COMPATIBILITY=1)
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
include(ConfigTargets)
include(OgreAddTargets)

configure_gamekit(${OGREKIT_SOURCE_DIR}/Dependencies/Source)

set(GAMEKIT_GRAPHIC_BACKEND 1 CACHE STRING 
	"Select the graphic backend for GameKit. Possible values:
		 1 - Ogre 1.9a
		 2 - Ogre 1.8rc
		 3 - Ogre 1.9-mercurial
	")

# Named constants for the available backends
set(GAMEKIT_GRAPHIC_BACKEND_19 		  		1)
set(GAMEKIT_GRAPHIC_BACKEND_18 				2)
set(GAMEKIT_GRAPHIC_BACKEND_19_mercurial 	3)


if (GAMEKIT_GRAPHIC_BACKEND EQUAL GAMEKIT_GRAPHIC_BACKEND_19)
	set(GAMEKIT_GRAPHIC_BACKEND_DIR Ogre-1.9a)
elseif (GAMEKIT_GRAPHIC_BACKEND EQUAL GAMEKIT_GRAPHIC_BACKEND_18)	
	set(GAMEKIT_GRAPHIC_BACKEND_DIR Ogre-1.8rc)
	add_definitions(-DBUILD_OGRE18)
elseif (GAMEKIT_GRAPHIC_BACKEND EQUAL GAMEKIT_GRAPHIC_BACKEND_19_mercurial)
	set(GAMEKIT_GRAPHIC_BACKEND_DIR Ogre-1.9-mercurial)
	set(OGREKIT_CREATE_OGRE_DEPENDENCY_DIR 1)
else()
	message(FATAL_ERROR "Invalid graphics backend or not supported yet.")
endif()


message(STATUS "-----------------------------------------------------------------------------")
message(STATUS "-- Graphic backend is " ${GAMEKIT_GRAPHIC_BACKEND_DIR})
message(STATUS "-----------------------------------------------------------------------------")

configure_ogrekit(${OGREKIT_SOURCE_DIR} ${OGREKIT_SOURCE_DIR}/${GAMEKIT_GRAPHIC_BACKEND_DIR} ${GAMEKIT_GRAPHIC_BACKEND})

set(OGREKIT_BULLET_LIBS BulletDynamics BulletCollision LinearMath)	
set(OGREKIT_BULLET_INCLUDE ${OGREKIT_SOURCE_DIR}/bullet/src)
if (OGREKIT_COMPILE_SOFTBODY)
	list(APPEND OGREKIT_BULLET_LIBS BulletSoftBody)
endif()

if (NOT APPLE AND OGREKIT_COMPILE_WXWIDGETS)
	include(wxSetup)
	configure_wx_build(${OGREKIT_SOURCE_DIR})
	setup_wx()
endif()


#if (APPLE)
#    if (${XCODE_VERSION} STRGREATER "4.0.0")
#        set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "4.2")
#    endif()
#endif()


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
                    ${OGREKIT_SOURCE_DIR}/GUI
                    ${OGREKIT_SOURCE_DIR}/Engine/External/Ogre/supp
                    ${OGREKIT_BULLET_INCLUDE}
                    ${OGREKIT_DEP_INCLUDE}
                    )

set(OGREKIT_LIB OgreKitCore ${OGREKIT_OGRE_LIBS})

# ---------------------------------------------------------
# Configure Dependencies
subdirs(bullet)

if (OGREKIT_BUILD_ANDROID)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOGREKIT_BUILD_ANDROID")
endif ()

if (OGREKIT_BUILD_NACL)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOGREKIT_BUILD_NACL -DNACL")
endif ()

subdirs(Dependencies)

# ---------------------------------------------------------
# The actual GameKit/OgreKit builds can only proceed after
# the dependencies have been built. So: after cmake/make of
# the deps, re-run cmake/make for the rest!
if(NOT OGREKIT_ONLY_DEPENDENCIES)
	if (OGREKIT_CREATE_OGRE_DEPENDENCY_DIR)
		set(CMAKE_FIND_ROOT_PATH "${OGRE_DEPENDENCIES_DIR}" ${CMAKE_FIND_ROOT_PATH})
		set(FREETYPE_HOME "${OGRE_DEPENDENCIES_DIR}")
		set(FREEIMAGE_HOME "${OGRE_DEPENDENCIES_DIR}")
	endif ()
	
	if(NOT OGREKIT_SKIP_OGRE)
		if (OGREKIT_CREATE_OGRE_DEPENDENCY_DIR)
			# Enter Ogre dir as SOURCE_DIR
			set(OGREKIT_TEMP_CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
			set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/${GAMEKIT_GRAPHIC_BACKEND_DIR})
			
			if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
			  set(SAVE_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
			  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
			endif()
		endif()

		if(OGREKIT_BUILD_ANDROID)
			# Ogre samples search some libs in another subdir...
			file(COPY ${OGREKIT_BINARY_DIR}/systemlibs DESTINATION ${OGREKIT_BINARY_DIR}/${GAMEKIT_GRAPHIC_BACKEND_DIR})
		endif()
		
		# Configure Ogre build
		add_subdirectory(${GAMEKIT_GRAPHIC_BACKEND_DIR})

		if (OGREKIT_CREATE_OGRE_DEPENDENCY_DIR)
			if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
			  set(CMAKE_CXX_FLAGS "${SAVE_CMAKE_CXX_FLAGS}")
			endif()

			# Restore SOURCE_DIR
			set(CMAKE_SOURCE_DIR ${OGREKIT_TEMP_CMAKE_SOURCE_DIR})
		endif()
	endif()
	
	if (OGREKIT_CREATE_OGRE_DEPENDENCY_DIR)
		# point the Engine to the build-config
		set(OGREKIT_OGRE_INCLUDE "${OGREKIT_BINARY_DIR}/${GAMEKIT_GRAPHIC_BACKEND_DIR}/include;${OGREKIT_OGRE_INCLUDE}")
		set(OGREKIT_INCLUDE "${OGREKIT_BINARY_DIR}/${GAMEKIT_GRAPHIC_BACKEND_DIR}/include;${OGREKIT_INCLUDE}")
	endif()
	
	# GameKit Engine ------------------------------------------------------------------------
	subdirs(Engine)

	# Further GameKit stuff (Samples, Tools and Projects) -----------------------------------
	if (NOT OGREKIT_USE_BPARSE OR OGREKIT_USE_FILETOOLS)
		subdirs(${OGREKIT_SOURCE_DIR}/Tools/FileTools)
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



endif()
