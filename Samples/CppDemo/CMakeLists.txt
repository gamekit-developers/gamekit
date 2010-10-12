# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

set(CONTENT
	Maps.blend
	Momo.blend
	Rat.blend
)
set(TargetName AppCppDemo)


if (APPLE)
	include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
	SET(EXETYPE MACOSX_BUNDLE)
	SET_SOURCE_FILES_PROPERTIES( MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET_SOURCE_FILES_PROPERTIES(${CONTENT} PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET (NibFile MainMenu.nib)
endif()

set(SRC ${CONTENT} 
	gkApplication.h
	gkApplication.cpp
	gkCollisionCameraConstraint.h
	gkCollisionCameraConstraint.cpp
	gkGamePlayer.h
	gkGamePlayer.cpp
	gkGameLevel.h
	gkGameLevel.cpp
	gkGameNPC.h
	gkGameNPC.cpp
	Graphics/gkHUD.h
	Graphics/gkHUD.cpp
	Graphics/gkHUDElement.h
	Graphics/gkHUDElement.cpp
	Graphics/gkHUDManager.h
	Graphics/gkHUDManager.cpp
	)


include_directories(
	${OGREKIT_INCLUDE}
)

link_libraries(
	${OGREKIT_LIB}
)


set(HiddenCMakeLists ../CMakeLists.txt)
source_group(ParentCMakeLists FILES ${HiddenCMakeLists})


if (APPLE)

	add_executable(
		${TargetName} 
		MACOSX_BUNDLE
		MainMenu.nib
		${SRC} 
		${HiddenCMakeLists}
	)

else ()

	add_executable(${TargetName} ${SRC} ${HiddenCMakeLists})
	
	
	IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
		ADD_CUSTOM_COMMAND(
			TARGET ${TargetName} 
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/Maps.blend ${CMAKE_CURRENT_BINARY_DIR}/Maps.blend
			COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/Momo.blend ${CMAKE_CURRENT_BINARY_DIR}/Momo.blend
			COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/Rat.blend ${CMAKE_CURRENT_BINARY_DIR}/Rat.blend
		)
	ENDIF ()

endif()
