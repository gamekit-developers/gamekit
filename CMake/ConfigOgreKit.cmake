
macro (configure_ogrekit ROOT OGREPATH)

	set(GNUSTEP_SYSTEM_ROOT $ENV{GNUSTEP_SYSTEM_ROOT})
	
	if(APPLE OR GNUSTEP_SYSTEM_ROOT)
		if (WIN32 AND NOT CMAKE_COMPILER_IS_GNUCXX)
			set(OGREKIT_USE_COCOA FALSE CACHE BOOL "Forcing remove Use Cocoa" FORCE)
		else()
			option(OGREKIT_USE_COCOA	"Use Cocoa"	ON)
		endif()
	endif()
	
	if(OGREKIT_USE_COCOA)
		add_definitions(-DOGREKIT_USE_COCOA)
		if(GNUSTEP_SYSTEM_ROOT)
			include_directories(${GNUSTEP_SYSTEM_ROOT}/Library/Headers)
			link_directories(${GNUSTEP_SYSTEM_ROOT}/Library/Libraries)
			link_libraries("gnustep-base")
			link_libraries("objc")
		endif()
	endif()

	set(OGREKIT_INSTALL_PREFIX ${ROOT}/Bin)
	
	option(OGREKIT_USE_LUA					"Use Lua script bindings" OFF)
	option(OGREKIT_COMPILE_SWIG				"Enable compile time SWIG generation."  OFF)
	option(OGREKIT_COMPILE_OGRE_SCRIPTS		"Automatically convert Blender TX to Ogre (.material, .font, .overlay... etc)" OFF)
	option(OGREKIT_COMPILE_WXWIDGETS		"Enable / Disable wxWidgets builds" OFF)
	option(OGREKIT_DEBUG_ASSERT				"Enable / Disable debug asserts." ON)
	option(OGREKIT_HEADER_GENERATOR			"Build Blender DNA to C++ generator."   OFF)
	option(OGREKIT_UPDATE_CPP_DOCS			"Update C++ API documentation(Requires doxygen)." OFF)
	option(OGREKIT_UPDATE_LUA_DOCS			"Update Lua API documentation(Requires doxygen)." OFF)
	option(OGREKIT_UPDATE_FBT_DOCS			"Update FBT API documentation(Requires doxygen)." OFF)
	option(OGREKIT_DISABLE_ZIP				"Disable external .zip resource loading" ON)
	option(OGREKIT_USE_STATIC_FREEIMAGE		"Compile and link statically FreeImage and all its plugins" ON)	
	option(OGREKIT_ENABLE_UNITTESTS			"Enable / Disable UnitTests" OFF)
	option(OGREKIT_USE_FILETOOLS			"Compile FBT file format utilities" ON)
	option(OGREKIT_COMPILE_TINYXML			"Enable / Disable TinyXml builds" OFF)
	option(OGREKIT_COMPILE_LIBROCKET		"Enable / Disalbe libRocket builds" OFF)
	option(OGREKIT_GENERATE_BUILTIN_RES		"Generate build-in resources" OFF)
	option(OGREKIT_COMPILE_TCL				"Compile TemplateGenerator" OFF)
	option(OGREKIT_COMPILE_RECAST			"Enable / Disable Recast build" OFF)
	option(OGREKIT_COMPILE_OPENSTEER		"Enable / Disable OpenSterr build" OFF)
	option(OGREKIT_USE_NNODE				"Use Logic Node(It's Nodal Logic, not Blender LogicBrick)" OFF)
	option(OGREKIT_COMPILE_OGRE_COMPONENTS	"Eanble compile additional Ogre components (RTShader, Terrain, Paging, ... etc)" OFF)
	option(OGREKIT_COMPILE_OPTS				"Enable / Disable Opts builds" OFF)
	
	set(OGREKIT_ZZIP_TARGET ZZipLib)
	set(OGREKIT_FREETYPE_TARGET freetype)
	set(OGREKIT_RECAST_TARGET Recast)
	set(OGREKIT_DETOUR_TARGET Detour)
	set(OGREKIT_OPENSTEER_TARGET OpenSteer)
	set(OGREKIT_LUA_TARGET Lua)
	set(OGREKIT_OGGVORBIS_TARGET OggVorbis)
	set(OGREKIT_OIS_TARGET OIS)
	set(OGREKIT_TINYXML_TARGET TinyXml)
	set(OGREKIT_OPTS_TARGET opts)
	
	set(OGRE_BINARY_DIR ${OGREPATH}/Bin)
	set(OGRE_TEMPLATES_DIR ${ROOT}/CMake/Templates)
	set(OGRELITE_SOURCE_DIR ${OGREPATH})
    set(OGREKIT_DEP_DIR ${ROOT}/Dependencies/Source)
	set(OGREKIT_SAMPLES_DIR ${ROOT}/Samples)
	
	include(OgreConfigTargets)
	include(DependenciesOgreKit)
	include(MacroLogFeature)


	if (NOT OGREKIT_USE_STATIC_FREEIMAGE)
	
		if(ZLIB_FOUND)
			set(OGREKIT_ZLIB_TARGET	${ZLIB_LIBRARY})
			set(OGREKIT_FREEIMAGE_INCLUDE	${ZLIB_INCLUDE_DIR})
		else(ZLIB_FOUND)
			message("Zlib not found.")
			message("Package is mandatory, please install it or enable static FreeImage compilation.")
		endif(ZLIB_FOUND)
		
		
		if(FreeImage_FOUND)
			set(OGREKIT_FREEIMAGE_TARGET	${FreeImage_LIBRARY})	
			set(OGREKIT_FREEIMAGE_INCLUDE	${FreeImage_INCLUDE_DIR})
		else(FreeImage_FOUND)
			message("FreeImage not found")
			message("Package is mandatory, please install it or enable static FreeImage compilation.")
		endif(FreeImage_FOUND)
		
	else(NOT OGREKIT_USE_STATIC_FREEIMAGE)
	
		set(OGREKIT_ZLIB_TARGET	ZLib)
		set(OGREKIT_FREEIMAGE_TARGET FreeImage)
		set(OGREKIT_ZLIB_INCLUDE ${OGREKIT_DEP_DIR}/FreeImage/ZLib)
		set(OGREKIT_FREEIMAGE_INCLUDE ${OGREKIT_DEP_DIR}/FreeImage)        
		
	endif(NOT OGREKIT_USE_STATIC_FREEIMAGE)


	if (APPLE)
		if (OGRE_BUILD_PLATFORM_IPHONE)
			option(OGREKIT_USE_COCOA "Use Cocoa" ON)
			set(OGREKIT_PLATFORM ${OGREPATH}/OgreMain/include/IPhone )
		else()
			set(OGREKIT_PLATFORM ${OGREPATH}/OgreMain/include/OSX )
		endif()
	  else (APPLE)
		if (UNIX)
		set(OGREKIT_PLATFORM ${OGREPATH}/OgreMain/include/GLX )
		else (UNIX)
		  if (WIN32)
		set(OGREKIT_PLATFORM ${OGREPATH}/OgreMain/include/WIN32 )
		  endif (WIN32)
		endif (UNIX)
	endif (APPLE)
	
	option(SAMPLES_RUNTIME        "Build Samples/Runtime"       OFF)
	option(SAMPLES_LOGICDEMO      "Build Samples/LogicDemo"     OFF)
	option(SAMPLES_VEHICLEDEMO    "Build Samples/VehicleDemo"   OFF)
	option(SAMPLES_CPPDEMO        "Build Samples/CppDemo"       OFF)
	option(SAMPLES_NODE_EDITOR    "Build Samples/NodeEditor"    OFF)
	option(SAMPLES_EMBEDDEMO      "Build Samples/EmbedDemo"     OFF)
	option(SAMPLES_INSPECTOR      "Build Samples/FileInspector" OFF)
	option(SAMPLES_GUIDEMO        "Build Samples/GuiDemo"       OFF)
	option(SAMPLES_LUA_EDITOR     "Build Samples/LuaEditor"     OFF)
	option(SAMPLES_LUARUNTIME     "Build Samples/LuaRuntime"    OFF)
	option(SAMPLES_ANIMKIT_GL     "Build Samples/AnimKitGL"     ON)
	
	if (SAMPLES_LOGICDEMO OR SAMPLES_VEHICLEDEMO)		
		set(OGREKIT_USE_NNODE TRUE CACHE BOOL "Forcing Logic Node" FORCE)		
	endif()
	
	if (SAMPLES_LOGICDEMO)
		set(OGREKIT_COMPILE_RECAST TRUE CACHE BOOL "Forcing Recast" FORCE)
		set(OGREKIT_COMPILE_OPENSTEER TRUE CACHE BOOL "Forcing OpenSteer" FORCE)
	endif()
	
	if (SAMPLES_LUARUNTIME OR SAMPLES_LUA_EDITOR)
		set(OGREKIT_USE_LUA TRUE CACHE BOOL "Forcing Lua" FORCE)
	endif()
	
	if (SAMPLES_LUARUNTIME)
		set(OGREKIT_COMPILE_OPTS TRUE CACHE BOOL "Forcing Opts" FORCE)
	endif()
	
	if (NOT OGREKIT_USE_LUA)
		set(OGREKIT_COMPILE_SWIG FALSE CACHE BOOL "Disabling Swig" FORCE)
	endif()

	if (SAMPLES_NODE_EDITOR OR SAMPLES_EMBEDDEMO OR SAMPLES_LUA_EDITOR OR SAMPLES_INSPECTOR)
		set(OGREKIT_COMPILE_WXWIDGETS TRUE CACHE BOOL "Forcing wxWidgets" FORCE)	
	endif()
	
	if (SAMPLES_LUA_EDITOR)
		set(OGREKIT_COMPILE_TINYXML TRUE CACHE BOOL "Forcing TinyXml" FORCE)
	endif()

	if (SAMPLES_INSPECTOR)	
		set(OGREKIT_USE_FILETOOLS   TRUE CACHE BOOL "Forcing File Utils" FORCE)
	endif()

	if (WIN32 AND (SAMPLES_EMBEDDEMO OR SAMPLES_LUA_EDITOR))
		set(OGREKIT_OIS_WIN32_NATIVE TRUE CACHE BOOL "Forcing OISWin32Native"  FORCE)
	endif()
	
	if (SAMPLES_GUIDEMO)
		set(OGREKIT_COMPILE_LIBROCKET TRUE CACHE BOOL "Forcing LibRocket"  FORCE)
	endif()
	
	if (OGREKIT_COMPILE_SWIG OR OGREKIT_GENERATE_BUILTIN_RES)
		set(OGREKIT_COMPILE_TCL TRUE CACHE BOOL "Forcing TCL"  FORCE)
	endif()

	if (APPLE)
		option(OGREKIT_BUILD_IPHONE	"Build GameKit on IPhone SDK"	OFF)
	endif()

	if (OGREKIT_BUILD_IPHONE)
		set(OGRE_BUILD_PLATFORM_IPHONE TRUE)
	endif()
	
	if (OGREKIT_COMPILE_OGRE_COMPONENTS)
		option(OGRE_BUILD_COMPONENT_PAGING "Build Ogre Paging Compoment" ON)
		option(OGRE_BUILD_COMPONENT_TERRAIN "Build Ogre Terrain Compoment" ON)
		option(OGRE_BUILD_COMPONENT_RTSHADERSYSTEM "Build Ogre RTShaderSystem Compoment" OFF)
		option(OGRE_BUILD_COMPONENT_PROPERTY "Build Ogre Property Compoment(Required boost)" OFF)
	endif()

	#copy from ogre3d build
	# Set up iPhone overrides.
	if (OGRE_BUILD_PLATFORM_IPHONE)
		include_directories("${OGREPATH}/OgreMain/include/iPhone")
	
		# Set build variables
		set(CMAKE_OSX_SYSROOT iphoneos)
		set(CMAKE_OSX_DEPLOYMENT_TARGET "")
		set(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework CoreGraphics -framework QuartzCore -framework UIKit")
		set(XCODE_ATTRIBUTE_SDKROOT iphoneos)
		set(OGRE_BUILD_RENDERSYSTEM_GLES TRUE CACHE BOOL "Forcing OpenGL ES RenderSystem for iPhone" FORCE)
		set(OGRE_STATIC TRUE CACHE BOOL "Forcing static build for iPhone" FORCE)
		set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.\${PRODUCT_NAME:rfc1034identifier}")
		set(OGRE_CONFIG_ENABLE_VIEWPORT_ORIENTATIONMODE TRUE CACHE BOOL "Forcing viewport orientation support for iPhone" FORCE)
	
		# CMake 2.8.1 added the ability to specify per-target architectures.
		# As a side effect, it creates corrupt Xcode projects if you try do it for the whole project.
		if(VERSION STRLESS "2.8.1")
			set(CMAKE_OSX_ARCHITECTURES $(ARCHS_STANDARD_32_BIT))
		else()
			set(CMAKE_OSX_ARCHITECTURES "armv6;armv7;")
		endif()
	
		add_definitions(-fno-regmove)
		remove_definitions(-msse)
	
		if (NOT OGRE_CONFIG_ENABLE_VIEWPORT_ORIENTATIONMODE)
			set(OGRE_SET_DISABLE_VIEWPORT_ORIENTATIONMODE 1)
		endif()
	
	elseif (APPLE)
	
		# Set 10.4 as the base SDK by default
		set(XCODE_ATTRIBUTE_SDKROOT macosx10.4)
	
		if (NOT CMAKE_OSX_ARCHITECTURES)
			set(CMAKE_OSX_ARCHITECTURES "i386")
		endif()
	  
		# 10.6 sets x86_64 as the default architecture.
		# Because Carbon isn't supported on 64-bit and we still need it, force the architectures to ppc and i386
		if(CMAKE_OSX_ARCHITECTURES MATCHES "x86_64" OR CMAKE_OSX_ARCHITECTURES MATCHES "ppc64")
			string(REPLACE "x86_64" "" CMAKE_OSX_ARCHITECTURES ${CMAKE_OSX_ARCHITECTURES})
			string(REPLACE "ppc64" "" CMAKE_OSX_ARCHITECTURES ${CMAKE_OSX_ARCHITECTURES})
		endif()
	
		# Make sure that the OpenGL render system is selected for non-iPhone Apple builds
		set(OGRE_BUILD_RENDERSYSTEM_GL TRUE)
		set(OGRE_BUILD_RENDERSYSTEM_GLES FALSE)
	
	endif ()


	if (OGREKIT_COMPILE_SWIG)
		include(RunSwig)
	endif()


	if (OGREKIT_COMPILE_TCL)	
		include(TemplateCompiler)
	endif()
	
	set(OGREKIT_FREETYPE_INCLUDE ${OGREKIT_DEP_DIR}/FreeType/include)
	set(OGREKIT_ZZIP_INCLUDE ${OGREKIT_DEP_DIR}/ZZipLib)
	set(OGREKIT_OIS_INCLUDE ${OGREKIT_DEP_DIR}/OIS/include)
	set(OGREKIT_OGRE_INCLUDE ${OGREPATH}/OgreMain/include ${OGREKIT_BINARY_DIR}/Settings ${OGREKIT_PLATFORM})
	set(OGREKIT_LUA_INCLUDE ${OGREKIT_DEP_DIR}/Lua/lua)
	set(OGREKIT_OGGVORBIS_INCLUDE ${OGREKIT_DEP_DIR}/Codecs/include)
	
	set(OGREKIT_RECAST_INCLUDE ${OGREKIT_DEP_DIR}/Recast/Include)
	set(OGREKIT_DETOUR_INCLUDE ${OGREKIT_DEP_DIR}/Detour/Include)
	set(OGREKIT_OPENSTEER_INCLUDE ${OGREKIT_DEP_DIR}/OpenSteer/include)
	set(OGREKIT_TINYXML_INCLUDE ${OGREKIT_DEP_DIR}/TinyXml)
	set(OGREKIT_OPTS_INCLUDE ${OGREKIT_DEP_DIR}/opts)

	set(OGREKIT_DEP_INCLUDE
#		${OGREKIT_FREEIMAGE_INCLUDE} Conflicts with OpenSteer includes and needed by Ogre, not OgreKit
		${OGREKIT_FREETYPE_INCLUDE}
		${OGREKIT_ZLIB_INCLUDE}
		${OGREKIT_OIS_INCLUDE}		
		${GAMEKIT_SERIALIZE_BULLET}
		${GAMEKIT_SERIALIZE_BLENDER}
		${GAMEKIT_UTILS_PATH}
		${GAMEKIT_ANIMKIT_PATH}
	)

	
	set(OGREKIT_LIBROCKET_INCLUDE ${OGREKIT_DEP_DIR}/libRocket/Include)
	set(OGREKIT_LIBROCKET_LIBS RocketCore RocketControls RocketDebugger)

	if (WIN32)
		# Use static library. No SDK needed at build time.
		# Must have OpenAL32.dll installed on the system 
		# In order to use OpenAL sound.
		set(OPENAL_FOUND TRUE)
	endif()


	if (OPENAL_FOUND)
		option(OGREKIT_OPENAL_SOUND "Enable building of the OpenAL subsystem" ON)
		
		if (WIN32)
			add_definitions(-DAL_STATIC_LIB -DALC_STATIC_LIB)
			set(OGREKIT_OPENAL_INCLUDE ${OGREKIT_DEP_DIR}/OpenAL/)
			set(OGREKIT_OPENAL_LIBRARY OpenAL)
		else()
			set(OGREKIT_OPENAL_INCLUDE ${OPENAL_INCLUDE_DIR})
			set(OGREKIT_OPENAL_LIBRARY ${OPENAL_LIBRARY})
		endif()
	else()
		option(OGREKIT_OPENAL_SOUND "Enable building of the OpenAL subsystem" OFF)
	endif()

	set(OGREKIT_MINGW_DIRECT3D TRUE)
	if (CMAKE_COMPILER_IS_GNUCXX)
		# Some Issues with unresolved symbols
		set(OGREKIT_MINGW_DIRECT3D FALSE)
	endif()


	if (WIN32)
		if (NOT DirectX_FOUND OR NOT OGREKIT_MINGW_DIRECT3D)
			# Default use OIS without dinput 

			option(OGREKIT_OIS_WIN32_NATIVE "Enable building of the OIS Win32 backend" ON)
		else ()
			# Use standard OIS build.

			option(OGREKIT_OIS_WIN32_NATIVE "Enable building of the OIS Win32 backend" OFF)
		endif()
	endif()

	
	if (OPENGL_FOUND)
		option(OGREKIT_BUILD_GLRS "Enable the OpenGL render system" ON)
	endif()
	
	if (OPENGLES_FOUND AND OGREKIT_BUILD_IPHONE)
		option(OGREKIT_BUILD_GLESRS "Enable the OpenGLES system" ON)
	endif()

	if (OPENGL_FOUND AND OGREKIT_BUILD_GLRS)
		set(OGRE_BUILD_RENDERSYSTEM_GL  TRUE)
		set(OGREKIT_GLRS_LIBS           RenderSystem_GL)
		set(OGREKIT_GLRS_ROOT           ${OGREPATH}/RenderSystems/GL)
		set(OGREKIT_GLESRS_INCLUDE      ${OGREPATH}/RenderSystems/GLES/include)
		set(OGREKIT_GLRS_INCLUDE        ${OGREPATH}/RenderSystems/GL/include)
	endif()

	if (OPENGLES_FOUND AND OGREKIT_BUILD_GLESRS)		
		set(OGRE_BUILD_RENDERSYSTEM_GLES TRUE)
		set(OGREKIT_GLESRS_LIBS          RenderSystem_GLES)
		set(OGREKIT_GLESRS_ROOT          ${OGREPATH}/RenderSystems/GLES)
		set(OGREKIT_GLESRS_INCLUDE       ${OGREPATH}/RenderSystems/GLES/include)
		set(OGREKIT_GLRS_INCLUDE         ${OGREPATH}/RenderSystems/GL/include)
	endif()


	if (WIN32 AND OGREKIT_MINGW_DIRECT3D)

		if (DirectX_FOUND)
			option(OGREKIT_BUILD_D3D9RS	 "Enable the Direct3D 9 render system" ON)
			option(OGREKIT_BUILD_D3D10RS "Enable the Direct3D 10 render system" OFF)
			option(OGREKIT_BUILD_D3D11RS "Enable the Direct3D 11 render system" OFF)
		endif()

		if (DirectX_FOUND AND OGREKIT_BUILD_D3D9RS)
			set(OGRE_BUILD_RENDERSYSTEM_D3D9   TRUE)
			set(OGREKIT_D3D9_LIBS              RenderSystem_Direct3D9)
			set(OGREKIT_D3D9_ROOT              ${OGREPATH}/RenderSystems/Direct3D9)
			set(OGREKIT_DX9RS_INCLUDE          ${OGREPATH}/RenderSystems/Direct3D9/include)
		endif()

		if (DirectX_FOUND AND OGREKIT_BUILD_D3D10RS)
			set(OGRE_BUILD_RENDERSYSTEM_D3D10 TRUE)
			set(OGREKIT_D3D10_LIBS            RenderSystem_Direct3D10)
			set(OGREKIT_D3D10_ROOT            ${OGREPATH}/RenderSystems/Direct3D10)
			set(OGREKIT_DX10RS_INCLUDE        ${OGREPATH}/RenderSystems/Direct3D10/include)
		endif()


		if (DirectX_FOUND AND OGREKIT_BUILD_D3D11RS)
			set(OGRE_BUILD_RENDERSYSTEM_D3D11  TRUE)
			set(OGREKIT_D3D11_LIBS             RenderSystem_Direct3D11)
			set(OGREKIT_D3D11_ROOT             ${OGREPATH}/RenderSystems/Direct3D11)
			set(OGREKIT_DX11RS_INCLUDE         ${OGREPATH}/RenderSystems/Direct3D11/include)
		endif()


	endif()
	
	if (0)
		# disable until support is added  
		option(OGREKIT_BUILD_CG	 "Enable the CG plugin" ON)

		if (OGREKIT_BUILD_CG)
			set(OGRE_BUILD_PLUGIN_CG       TRUE)
			set(OGREKIT_CG_LIBS            Plugin_CgProgramManager)
			set(OGREKIT_CG_ROOT            ${OGREPATH}/PlugIns/CgProgramManager)
			set(OGREKIT_CG_INCLUDE         ${OGREPATH}/PlugIns/CgProgramManager/include)
		endif()

	endif()

	set(OGREKIT_OGRE_LIBS 
		OgreMain 
		${OGREKIT_FREEIMAGE_TARGET} 
		${OGREKIT_FREETYPE_TARGET} 
		${OGREKIT_GLRS_LIBS}
		${OGREKIT_D3D9_LIBS}
		${OGREKIT_D3D10_LIBS}
		${OGREKIT_D3D11_LIBS}		
		${GAMEKIT_SERIALIZE_BLENDER_TARGET}
		${GAMEKIT_SERIALIZE_BULLET_TARGET}
		${GAMEKIT_UTILS_TARGET}
		${OGREKIT_OIS_TARGET}
		${OGREKIT_ZLIB_TARGET}
		${GAMEKIT_ANIMKIT_TARGET}
		)

	if (OGREKIT_OPENAL_SOUND)
		list(APPEND OGREKIT_OGRE_LIBS	${OGREKIT_OPENAL_LIBRARY} ${OGREKIT_OGGVORBIS_TARGET})
		list(APPEND OGREKIT_DEP_INCLUDE ${OGREKIT_OPENAL_INCLUDE} ${OGREKIT_OGGVORBIS_INCLUDE})
	endif()
		
	if (OGREKIT_USE_LUA)
		list(APPEND OGREKIT_OGRE_LIBS	${OGREKIT_LUA_TARGET})
		list(APPEND OGREKIT_DEP_INCLUDE ${OGREKIT_LUA_INCLUDE})
	endif()

	if (NOT OGREKIT_DISABLE_ZIP)
		list(APPEND OGREKIT_OGRE_LIBS	${OGREKIT_ZZIP_TARGET})
		list(APPEND OGREKIT_DEP_INCLUDE ${OGREKIT_ZZIP_INCLUDE})
	endif()

	if (OGREKIT_COMPILE_RECAST)
		list(APPEND OGREKIT_OGRE_LIBS	${OGREKIT_RECAST_TARGET}  ${OGREKIT_DETOUR_TARGET})
		list(APPEND OGREKIT_DEP_INCLUDE ${OGREKIT_RECAST_INCLUDE} ${OGREKIT_DETOUR_INCLUDE})
	endif()
	
	if (OGREKIT_COMPILE_OPENSTEER)
		list(APPEND OGREKIT_OGRE_LIBS	${OGREKIT_OPENSTEER_TARGET})
		list(APPEND OGREKIT_DEP_INCLUDE ${OGREKIT_OPENSTEER_INCLUDE})
	endif()
		
	#Check Build Settings
	if (APPLE)
		if (OGREKIT_BUILD_IPHONE)
			set(OGRE_BUILD_RENDERSYSTEM_GL CACHE BOOL "Forcing remove OpenGL RenderSystem for iPhone" FORCE)
			set(OGRE_BUILD_RENDERSYSTEM_GLES TRUE CACHE BOOL "Forcing use OpenGLES RenderSystem for iPhone" FORCE)
						
			set(OGREKIT_BUILD_CG   CACHE BOOL "Forcing remove CG for iPhone"   FORCE)
			set(OGREKIT_USE_COCOA  TRUE CACHE BOOL "Forcing use COCOA for iPhone" FORCE)
			
			if (OGREKIT_BUILD_GLRS)
				message(SEND_ERROR "Turn OFF OGREKIT_BUILD_GLRS Option for iPhone")
			endif()
			if (NOT OGREKIT_BUILD_GLESRS)
				message(SEND_ERROR "Turn ON OGREKIT_BUILD_GLESRS Option for iPhone")
			endif()
			
			if(VERSION STRLESS "2.8.2")
				message(ERROR "You should update the CMake 2.8.2 higher for iPhone build.")
	  		endif()
	  
		else()
			set(OGRE_BUILD_RENDERSYSTEM_GL TRUE CACHE BOOL "Forcing use OpenGL RenderSystem for OS X" FORCE)
			set(OGRE_BUILD_RENDERSYSTEM_GLES CACHE BOOL "Forcing remove OpenGLES RenderSystem for OS X" FORCE)
			
			if (NOT OGREKIT_BUILD_GLRS)
				message(SEND_ERROR "Turn ON OGREKIT_BUILD_GLRS Option for OS X")
			endif()
			if (OGREKIT_BUILD_GLESRS)
				message(SEND_ERROR "Turn OFF OGREKIT_BUILD_GLESRS Option for OS X")
			endif()
		endif()
	endif(APPLE)
 
	configure_file(${CMAKE_SOURCE_DIR}/CMake/Templates/OgreKitSettings.in ${CMAKE_BINARY_DIR}/Engine/gkSettings.h)
	include_directories(${CMAKE_BINARY_DIR}/Engine)

endmacro(configure_ogrekit)


macro(configure_rendersystem)

	if (OGRE_BUILD_RENDERSYSTEM_GL)
		
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

	if (OGRE_BUILD_RENDERSYSTEM_GLES)
		
		include_directories(
			${OGREKIT_GLESRS_ROOT}/include
			${OGREKIT_GLESRS_ROOT}/include/EAGL
		)
	
		link_libraries(
			${OGREKIT_GLESRS_LIBS} 
		)
		
	endif()

	if (OGREKIT_BUILD_D3D9RS)

		include_directories(
			${OGREKIT_D3D9_ROOT}/include
		)
		
		link_libraries(
			${DirectX_D3D9_LIBRARY}
			${OGREKIT_D3D9_LIBS} 
		)
			
	endif()
	
	if (OGREKIT_BUILD_D3D10RS AND DirectX_D3D10_FOUND)
		
		include_directories(
			${OGREKIT_D3D10_ROOT}/include
		)
		
		link_libraries(
			${OGREKIT_D3D10_LIBS} 
			${DirectX_D3D10_LIBRARIES}
		)
			
	endif()

	if (OGREKIT_BUILD_D3D11RS AND DirectX_D3D11_FOUND)
		
		include_directories(
			${OGREKIT_D3D11_ROOT}/include
		)
		
		link_libraries(
			${OGREKIT_D3D11_LIBS} 
			${DirectX_D3D10_LIBRARIES}
		)

	endif()
	
	if (0)
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

