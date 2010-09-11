
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
	
	option(OGREKIT_USE_LUA               "Use Lua script bindings" ON)
	option(OGREKIT_COMPILE_OGRE_SCRIPTS  "Automatically convert Blender TX to Ogre (.material, .font, .overlay... etc)" ON)

	if (OGREKIT_USE_LUA)
		add_definitions(-DOGREKIT_USE_LUA)	
	endif()
	
	if (OGREKIT_COMPILE_OGRE_SCRIPTS)
		add_definitions(-DOGREKIT_COMPILE_OGRE_SCRIPTS)	
	endif()

	set(OGREKIT_ZLIB_TARGET	ZLib)
	set(OGREKIT_ZZIP_TARGET ZZipLib)
	set(OGREKIT_FREEIMAGE_TARGET FreeImage)
	set(OGREKIT_FREETYPE_TARGET freetype)
	set(OGREKIT_RECAST_TARGET Recast)
	set(OGREKIT_DETOUR_TARGET Detour)
	set(OGREKIT_OPENSTEER_TARGET OpenSteer)
	set(OGREKIT_V8_TARGET V8)
	set(OGREKIT_LUA_TARGET Lua)
	

	set(OGRE_BINARY_DIR ${OGREPATH}/Bin)
	set(OGRE_TEMPLATES_DIR ${ROOT}/CMake/Templates)
	set(OGRELITE_SOURCE_DIR ${OGREPATH})

	include(OgreConfigTargets)
	include(DependenciesOgreKit)
	include(MacroLogFeature)

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
	
	option(SAMPLES_RUNTIME        "Build Samples/Runtime"     ON)
	option(SAMPLES_LOGICDEMO      "Build Samples/LogicDemo"   ON)
	option(SAMPLES_VEHICLEDEMO    "Build Samples/VehicleDemo" ON)
	option(SAMPLES_CPPDEMO        "Build Samples/CppDemo"     ON)
	
	
	
	if (OGREKIT_USE_LUA)
		option(SAMPLES_LUARUNTIME "Build Samples/LuaRuntime" ON)
	else()
		set(SAMPLES_LUARUNTIME FALSE CACHE BOOL "Forcing remove Samples/LuaRuntime" FORCE)
	endif()


	if (APPLE)
		option(OGREKIT_BUILD_IPHONE	"Build GameKit on IPhone SDK"	OFF)
	endif()

	if (OGREKIT_BUILD_IPHONE)
		set(OGRE_BUILD_PLATFORM_IPHONE TRUE)
		add_definitions("-DOGREKIT_BUILD_IPHONE")		
	endif()

	option(OGREKIT_COMPLIE_SWIG "Enable compile time SWIG generation."  OFF)
	option(OGREKIT_HEADER_GENERATOR "Build Blender DNA to C++ generator."   OFF)
	
	#copy from ogre3d build
	# Set up iPhone overrides.
	if (OGRE_BUILD_PLATFORM_IPHONE)
	  include_directories("${OGREPATH}/OgreMain/include/iPhone")
	
	  # Set build variables
	  set(CMAKE_OSX_SYSROOT iphoneos3.2)
	  set(CMAKE_OSX_DEPLOYMENT_TARGET "")
	  set(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework CoreGraphics -framework QuartzCore -framework UIKit")
	  set(XCODE_ATTRIBUTE_SDKROOT iphoneos3.2)
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
	  
	  if(VERSION STRLESS "2.8.1")
		message(STATUS "Copy iphone sdk files to " ${CMAKE_BINARY_DIR})
		file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SDK/iPhone/edit_linker_paths.sed DESTINATION ${CMAKE_BINARY_DIR})
		file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SDK/iPhone/fix_linker_paths.sh DESTINATION ${CMAKE_BINARY_DIR})
		
	  endif()
	  
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

	option(OGREKIT_UPDATE_DOCS "Update Lua API documentation(Requires doxygen)." OFF)


	if (OGREKIT_COMPLIE_SWIG)
		
		include(RunSwig)
		include(TemplateCompiler)

	endif()


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
	

	set(OGREKIT_DEP_INCLUDE
		${OGREKIT_FREEIMAGE_INCLUDE}
		${OGREKIT_FREETYPE_INCLUDE}
		${OGREKIT_ZLIB_INCLUDE}
		${OGREKIT_ZZIP_INCLUDE}
		${OGREKIT_OIS_INCLUDE}
		${OGREKIT_LUA_INCLUDE}
		${OGREKIT_OGGVORBIS_INCLUDE}
		${OGREKIT_RECAST_INCLUDE}
		${OGREKIT_DETOUR_INCLUDE}
		${OGREKIT_OPENSTEER_INCLUDE}
	)




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
	
	if (OPENGLES_FOUND)
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
		${OGREKIT_ZLIB_TARGET} 
		${OGREKIT_ZZIP_TARGET}
		${OGREKIT_GLRS_LIBS}
		${OGREKIT_D3D9_LIBS}
		${OGREKIT_D3D10_LIBS}
		${OGREKIT_D3D11_LIBS}
		${OGREKIT_RECAST_TARGET}
		${OGREKIT_DETOUR_TARGET}
		${OGREKIT_OPENSTEER_TARGET}
		${OGREKIT_OPENAL_LIBRARY}
		)
		
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
	endif()

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

	if (OGRE_BUILD_RENDERSYSTEM_GLES)
		
		add_definitions(-DOGREKIT_GLESRS)

		include_directories(
			${OGREKIT_GLESRS_ROOT}/include
			${OGREKIT_GLESRS_ROOT}/include/EAGL			
		)
	
		link_libraries(
			${OGREKIT_GLESRS_LIBS} 
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
	
	if (OGREKIT_BUILD_D3D10RS AND DirectX_D3D10_FOUND)
		add_definitions(-DOGREKIT_D3D10RS)
		
		include_directories(
			${OGREKIT_D3D10_ROOT}/include
		)
		
		link_libraries(
			${OGREKIT_D3D10_LIBS} 
			${DirectX_D3D10_LIBRARIES}
		)
			
	endif()

	if (OGREKIT_BUILD_D3D11RS AND DirectX_D3D11_FOUND)
		add_definitions(-DOGREKIT_D3D11RS)
		
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

