# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)



if (APPLE)
	include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
	SET(EXETYPE MACOSX_BUNDLE)
	SET_SOURCE_FILES_PROPERTIES( MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET_SOURCE_FILES_PROPERTIES(momo_ogre.blend PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET (NibFile MainMenu.nib)
endif()

set(SRC 
	momo_ogre.blend
	Main.cpp
)



include_directories(
	${OGREKIT_INCLUDE}
	../../Dependencies/Source/tclap/include
)

link_libraries(
	${OGREKIT_LIB}
)


set(HiddenCMakeLists ../CMakeLists.txt)
source_group(ParentCMakeLists FILES ${HiddenCMakeLists})

if (WIN32)
	set(PLAT_RESOURCE Win32/app.rc)
	set(RESOURCE )	
	set_source_files_properties(${RESOURCE} PROPERTIES HEADER_FILE_ONLY 1)
	source_group(Content\\Resources FILES ${PLAT_RESOURCE} ${RESOURCE})

	list(APPEND SRC ${PLAT_RESOURCE})
endif()




if (APPLE)

	add_executable(
		AppOgreKit 
		MACOSX_BUNDLE
		MainMenu.nib
		${SRC} 
		${HiddenCMakeLists}
	)

else ()

	add_executable(AppOgreKit ${SRC} ${HiddenCMakeLists})
	
	IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
		ADD_CUSTOM_COMMAND(
			TARGET AppOgreKit 
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/momo_ogre.blend ${CMAKE_CURRENT_BINARY_DIR}/momo_ogre.blend
		)
	ENDIF ()

endif()
