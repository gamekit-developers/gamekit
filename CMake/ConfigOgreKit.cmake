

macro (configure_ogrekit ROOT OGREPATH)

	set(OGREKIT_INSTALL_PREFIX ${ROOT}/Bin)

	set(OGREKIT_ZLIB_TARGET	ZLib)
	set(OGREKIT_ZZIP_TARGET ZZipLib)
	set(OGREKIT_FREEIMAGE_TARGET FreeImage)
	set(OGREKIT_FREETYPE_TARGET freetype)
	set(OGREKIT_RECAST_TARGET Recast)
	set(OGREKIT_DETOUR_TARGET Detour)
	set(OGREKIT_OPENSTEER_TARGET OpenSteer)

	set(OGRE_BINARY_DIR ${OGREPATH}/Bin)
	set(OGRE_TEMPLATES_DIR ${ROOT}/CMake/Templates)
	set(OGRELITE_SOURCE_DIR ${OGREPATH})

	include(OgreConfigTargets)
	include(DependenciesOgreKit)
	include(MacroLogFeature)

	if (APPLE)
		set(OGREKIT_PLATFORM ${OGREPATH}/OgreMain/include/OSX )
	  else (APPLE)
	    if (UNIX)
		set(OGREKIT_PLATFORM ${OGREPATH}/OgreMain/include/GLX )
	    else (UNIX)
	      if (WIN32)
		set(OGREKIT_PLATFORM ${OGREPATH}/OgreMain/include/WIN32 )
	      endif (WIN32)
	    endif (UNIX)
	endif (APPLE)
	
    option(OGREKIT_BUILD_RUNTIME    "Build Samples/Runtime"     ON)
    option(OGREKIT_BUILD_LOGICDEMO  "Build Samples/LogicDemo"   ON)


	set(OGREKIT_DEP_DIR ${ROOT}/Dependencies/Source)
	set(OGREKIT_FREEIMAGE_INCLUDE ${OGREKIT_DEP_DIR}/FreeImage)
	set(OGREKIT_FREETYPE_INCLUDE ${OGREKIT_DEP_DIR}/FreeType/include)
	set(OGREKIT_ZLIB_INCLUDE ${OGREKIT_DEP_DIR}/FreeImage/ZLib)
	set(OGREKIT_ZZIP_INCLUDE ${OGREKIT_DEP_DIR}/ZZipLib)
	set(OGREKIT_OIS_INCLUDE ${OGREKIT_DEP_DIR}/OIS/include)
	set(OGREKIT_OGRE_INCLUDE ${OGREPATH}/OgreMain/include ${OGREPATH}/Settings ${OGREKIT_PLATFORM})

	set(OGREKIT_LUA_INCLUDE ${OGREKIT_DEP_DIR}/Lua/lua)
	set(OGREKIT_OGGVORBIS_INCLUDE ${OGREKIT_DEP_DIR}/Codecs/include)
	
	set(OGREKIT_RECAST_INCLUDE ${OGREKIT_DEP_DIR}/Recast/Include)
	set(OGREKIT_DETOUR_INCLUDE ${OGREKIT_DEP_DIR}/Detour/Include)
	set(OGREKIT_OPENSTEER_INCLUDE ${OGREKIT_DEP_DIR}/OpenSteer/include)

	set(OGREKIT_MINGW_DIRECT3D TRUE)
	if (CMAKE_COMPILER_IS_GNUCXX)
		# Some Issues with unresolved symbols
		set(OGREKIT_MINGW_DIRECT3D FALSE)
	endif()

    if (OPENAL_FOUND)
		option(OGREKIT_OPENAL_SOUND "Enable building of the OpenAL subsystem" ON)
	    set(OGREKIT_OPENAL_INCLUDE ${OPENAL_INCLUDE_DIR})
	    set(OGREKIT_OPENAL_LIBRARY ${OPENAL_LIBRARY})
	else()
		option(OGREKIT_OPENAL_SOUND "Enable building of the OpenAL subsystem" OFF)
	endif()
        

	if (WIN32)
		if (NOT DirectX_FOUND OR NOT OGREKIT_MINGW_DIRECT3D)
			option(OGREKIT_OIS_WIN32_NATIVE "Enable building of the OIS Win32 backend" ON)
		else ()
			option(OGREKIT_OIS_WIN32_NATIVE "Enable building of the OIS Win32 backend" OFF)
		endif()
		
	endif()


	if (OPENGL_FOUND)
		option(OGREKIT_BUILD_GLRS "Enable the OpenGL render system" ON)
	endif()
	
	if (OPENGL_FOUND AND OGREKIT_BUILD_GLRS)
		message(STATUS "Configuring OpenGL")
		set(OGRE_BUILD_RENDERSYSTEM_GL TRUE)
		set(OGREKIT_GLRS_LIBS RenderSystem_GL)
		set(OGREKIT_GLRS_ROOT ${OGREPATH}/RenderSystems/GL)
		set(OGREKIT_GLESRS_INCLUDE ${OGREPATH}/RenderSystems/GLES/include)
		set(OGREKIT_GLRS_INCLUDE ${OGREPATH}/RenderSystems/GL/include)
	endif()

	if (WIN32 AND OGREKIT_MINGW_DIRECT3D)

		if (DirectX_FOUND)
			option(OGREKIT_BUILD_D3D9RS	 "Enable the Direct3D 9 render system" ON)
			option(OGREKIT_BUILD_D3D10RS "Enable the Direct3D 10 render system" OFF)
			option(OGREKIT_BUILD_D3D11RS "Enable the Direct3D 11 render system" OFF)
		endif()

		if (DirectX_FOUND AND OGREKIT_BUILD_D3D9RS)
		
			message(STATUS "Configuring Direct3D 9")

			set(OGRE_BUILD_RENDERSYSTEM_D3D9 TRUE)
			set(OGREKIT_D3D9_LIBS RenderSystem_Direct3D9)
			set(OGREKIT_D3D9_ROOT ${OGREPATH}/RenderSystems/Direct3D9)
			set(OGREKIT_DX9RS_INCLUDE ${OGREPATH}/RenderSystems/Direct3D9/include)
		endif()

		if (DirectX_FOUND AND OGREKIT_BUILD_D3D10RS)
			message(STATUS "Configuring Direct3D 10")
			set(OGRE_BUILD_RENDERSYSTEM_D3D10 TRUE)
			set(OGREKIT_D3D10_LIBS RenderSystem_Direct3D10)
			set(OGREKIT_D3D10_ROOT ${OGREPATH}/RenderSystems/Direct3D10)
			set(OGREKIT_DX10RS_INCLUDE ${OGREPATH}/RenderSystems/Direct3D10/include)
		endif()

		if (DirectX_FOUND AND OGREKIT_BUILD_D3D11RS)
			message(STATUS "Configuring Direct3D 11")
			set(OGRE_BUILD_RENDERSYSTEM_D3D11 TRUE)
			set(OGREKIT_D3D11_LIBS RenderSystem_Direct3D11)
			set(OGREKIT_D3D11_ROOT ${OGREPATH}/RenderSystems/Direct3D11)
			set(OGREKIT_DX11RS_INCLUDE ${OGREPATH}/RenderSystems/Direct3D11/include)
		endif()


	endif()
	
	if (Cg_FOUND)
		option(OGREKIT_BUILD_CG	 "Enable the CG plugin" ON)
		
		if (OGREKIT_BUILD_CG)
			message(STATUS "Configuring CG")
			set(OGRE_BUILD_PLUGIN_CG TRUE)
			set(OGREKIT_CG_LIBS Plugin_CgProgramManager)
			set(OGREKIT_CG_ROOT ${OGREPATH}/PlugIns/CgProgramManager)
			set(OGREKIT_CG_INCLUDE ${OGREPATH}/PlugIns/CgProgramManager/include)
		endif()

	endif()

	set(OGREKIT_OGRE_LIBS 
		OgreMain 
		${OGREKIT_FREEIMAGE_TARGET} 
		${OGREKIT_FREETYPE_TARGET} 
		${OGREKIT_ZLIB_TARGET} 
		${OGREKIT_ZZIP_TARGET}
		${OGREKIT_GLRS_LIBS}
		${OGREKIT_D3D9_LIBS}
		${OGREKIT_D3D10_LIBS}
		${OGREKIT_D3D11_LIBS}
		${OGREKIT_CG_LIBS}
		${OGREKIT_RECAST_TARGET}
		${OGREKIT_DETOUR_TARGET}
		${OGREKIT_OPENSTEER_TARGET}
		${OGREKIT_OPENAL_LIBRARY}
		)

endmacro(configure_ogrekit)


macro(configure_rendersystem)

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
	    
	endif()

	if (OGREKIT_BUILD_D3D9RS)

		add_definitions(-DOGREKIT_D3D9RS)
	    
		include_directories(
			${OGREKIT_D3D9_ROOT}/include
		)
	    
		link_libraries(
			${DirectX_D3D9_LIBRARY}
			${OGREKIT_D3D9_LIBS} 
		)
	        
	endif()
	
	if (OGREKIT_BUILD_D3D10RS)
		add_definitions(-DOGREKIT_D3D10RS)
	    
		include_directories(
			${OGREKIT_D3D10_ROOT}/include
		)
	    
		link_libraries(
			${OGREKIT_D3D10_LIBS} 
			${DirectX_D3D10_LIBRARY}
		)
	        
	endif()

	if (OGREKIT_BUILD_D3D11RS)
		add_definitions(-DOGREKIT_D3D11RS)
	    
		include_directories(
			${OGREKIT_D3D11_ROOT}/include
		)
	    
		link_libraries(
			${OGREKIT_D3D11_LIBS} 
			${DirectX_D3D11_LIBRARY}
		)

	endif()
	
	if (OGREKIT_BUILD_CG)
		add_definitions(-DOGREKIT_CG)
	    
		include_directories(
			${OGREKIT_CG_ROOT}/include
		)

		link_libraries(
			${OGREKIT_CG_LIBS} 
			${Cg_LIBRARY_REL}
		)

	endif()
endmacro(configure_rendersystem)

