#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------
include(MacroLogFeature)


#######################################################################
# Core dependencies
#######################################################################

# Find X11
if (UNIX AND NOT OGREKIT_BUILD_MOBILE) 
	## TEST THIS
	find_package(X11)
	macro_log_feature(X11_FOUND "X11" "X Window system" "http://www.x.org" TRUE "" "")
	macro_log_feature(X11_Xt_FOUND "Xt" "X Toolkit" "http://www.x.org" TRUE "" "")
	find_library(XAW_LIBRARY NAMES Xaw Xaw7 PATHS ${DEP_LIB_SEARCH_DIR} ${X11_LIB_SEARCH_PATH})
	macro_log_feature(XAW_LIBRARY "Xaw" "X11 Athena widget set" "http://www.x.org" TRUE "" "")
	mark_as_advanced(XAW_LIBRARY)
endif ()


if (OGREKIT_BUILD_MOBILE)
	# Find OpenGLES
	find_package(OpenGLES)
	macro_log_feature(OPENGLES_FOUND "OpenGLES" "Support for the OpenGLES render system" "" FALSE "" "")

	# Find OpenGLES2
	find_package(OpenGLES2)
	macro_log_feature(OPENGLES2_FOUND "OpenGLES2" "Support for the OpenGLES2 render system" "" FALSE "" "")
else()
	# Find OpenGL
	find_package(OpenGL)
	macro_log_feature(OPENGL_FOUND "OpenGL" "Support for the OpenGL render system" "http://www.opengl.org/" FALSE "" "")
endif()


# Find DirectX
if (WIN32)
    # Find OpenGLES2
	find_package(OpenGLES2)
	macro_log_feature(OPENGLES2_FOUND "OpenGLES2" "Support for the OpenGLES2 render system" "" FALSE "" "")
    
	find_package(DirectX)
	macro_log_feature(DirectX_FOUND "DirectX" "Support for the DirectX render system" "http://msdn.microsoft.com/en-us/directx/" FALSE "" "")
endif()


# Find Cg (Disabled Not used right now!)
# find_package(Cg)
# macro_log_feature(Cg_FOUND "cg" "C for graphics shader language" "http://developer.nvidia.com/object/cg_toolkit.html" FALSE "" "")

if(NOT WIN32 AND NOT OGREKIT_BUILD_ANDROID)
	# Use static loader On win32 platforms 

	# Find OpenAL
	include(FindOpenAL)
	macro_log_feature(OPENAL_FOUND "OpenAL" "Support for the OpenAL sound system" "http://connect.creativelabs.com/openal/default.aspx" FALSE "" "")
endif()

if(NOT OGREKIT_USE_STATIC_FREEIMAGE)
	find_package(FreeImage)
	macro_log_feature(FreeImage_FOUND "FreeImage" "Support for the FreeImage library" "http://freeimage.sourceforge.net/" FALSE "" "")

	find_package(ZLIB)
	macro_log_feature(ZLIB_FOUND "ZLib" "Support for the ZLib library" "http://www.zlib.net/" FALSE "" "")

endif()

#######################################################################
# Apple-specific
#######################################################################
if (APPLE AND NOT OGREKIT_BUILD_IPHONE)
	find_package(Carbon)
	macro_log_feature(Carbon_FOUND "Carbon" "Carbon" "http://developer.apple.com/mac" TRUE "" "")

	find_package(Cocoa)
	macro_log_feature(Cocoa_FOUND "Cocoa" "Cocoa" "http://developer.apple.com/mac" TRUE "" "")

	find_package(IOKit)
	macro_log_feature(IOKit_FOUND "IOKit" "IOKit HID framework needed by the samples" "http://developer.apple.com/mac" FALSE "" "")
endif()

# Display results, terminate if anything required is missing
MACRO_DISPLAY_FEATURE_LOG()

if (NOT OGREKIT_BUILD_MOBILE)

	# Add library and include paths from the dependencies
	include_directories(
		${OPENGL_INCLUDE_DIRS}
		${Cg_INCLUDE_DIRS}
		${X11_INCLUDE_DIR}
		${DirectX_INCLUDE_DIRS}
		${Carbon_INCLUDE_DIRS}
		${Cocoa_INCLUDE_DIRS}
	)
	
	link_directories(
		${OPENGL_LIBRARY_DIRS}
		${Cg_LIBRARY_DIRS}
		${X11_LIBRARY_DIRS}
		${DirectX_LIBRARY_DIRS}
	)

endif()
