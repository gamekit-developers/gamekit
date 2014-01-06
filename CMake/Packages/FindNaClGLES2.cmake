#-------------------------------------------------------------------
# This file is part of the CMake build system for Gamekit
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.gamekit.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# - Try to find prebuild win32 OpenGLES and EGL (http://code.google.com/p/angleproject/)
# Once done this will define
#  
#  OPENGLES2_FOUND        - system has OpenGLES
#  OPENGLES2_INCLUDE_DIR  - the GL include directory
#  OPENGLES2_LIBRARIES    - Link these to use OpenGLES
#
#  EGL_FOUND        - system has EGL
#  EGL_INCLUDE_DIR  - the EGL include directory
#  EGL_LIBRARIES    - Link these to use EGL

#getenv_path(NACL_SDK_ROOT)

#message(STATUS "env nacl:"${ENV_NACL_SDK_ROOT})

#IF (MSVC)  
	#SET(OPENGLES2_INCLUDE_DIR ${ENV_NACL_SDK_ROOT}/include/gles2) #Find GLES2/gl2.h			
	SET(OPENGLES2_INCLUDE_DIR 
		${OGRE_SOURCE_DIR}/RenderSystems/GLES2/include
		#${OGRE_SOURCE_DIR}/RenderSystem/GLES2/include
		#${ENV_NACL_SDK_ROOT}/include/ppapi/lib/gl
	) #Find GLES2/gl2.h			
	SET(EGL_INCLUDE_DIR ${OGRE_SOURCE_DIR}/RenderSystems/GLES2/include) #Find EGL/egl.h	
	SET(OPENGLES2_gl_LIBRARY ${NACL_LIB_PATH}/libppapi_gles2.a)
	#SET(OPENGLES2_gl_LIBRARY ${ENV_NACL_SDK_ROOT}/lib/glibc_x86_32/Release/libppapi_gles2.a) #Find libGLESv2
	#SET(EGL_egl_LIBRARY)
	#SET(EGL_egl_LIBRARY ${OGREKIT_DEP_WIN_DIR}/gles2/lib/libEGL.lib) #Find libEGL
	#SET(OPENGLES2_BIN_DIR ${OGREKIT_DEP_WIN_DIR}/gles2/bin)
	#message(STATUS "Add to your PATH: " ${OPENGLES2_INCLUDE_DIR})
#ENDIF (MSVC)

set(OGRE_SET_DISABLE_GLES3_SUPPORT 0)
SET( OPENGLES2_FOUND "YES" )
IF(OPENGLES2_gl_LIBRARY) # AND EGL_egl_LIBRARY)

    #SET( OPENGLES2_LIBRARIES ${OPENGLES2_gl_LIBRARY} ${OPENGLES2_LIBRARIES})
    #SET( EGL_LIBRARIES ${EGL_egl_LIBRARY} ${EGL_LIBRARIES})
    SET( OPENGLES2_FOUND "YES" )

ENDIF()#OPENGLES2_gl_LIBRARY AND EGL_egl_LIBRARY)

MARK_AS_ADVANCED(
  OPENGLES2_INCLUDE_DIR
  OPENGLES2_gl_LIBRARY
  EGL_INCLUDE_DIR
  EGL_egl_LIBRARY
)
