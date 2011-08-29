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


set(CPP_CONT_SRC
	Controllers/gkJoystickController.cpp
	Controllers/gkDefaultController.cpp
)

set(CPP_CONT_HDR
	gkGameController.h
	Controllers/gkJoystickController.h
	Controllers/gkDefaultController.h
)


set(CPP_MAIN_SRC
	gkApplication.cpp
	gkCollisionCameraConstraint.cpp
	gkGamePlayer.cpp
	gkGameLevel.cpp
	# gkGameNPC.cpp
)

set(CPP_MAIN_HDR
	gkApplication.h
	gkCollisionCameraConstraint.h
	gkGamePlayer.h
	gkGameLevel.h
	# gkGameNPC.h
)


set(SRC 
	${CONTENT} 
	${CPP_MAIN_SRC}  ${CPP_MAIN_HDR}
	${CPP_CONT_SRC}  ${CPP_CONT_HDR}
	${CPP_GRAPH_SRC} ${CPP_GRAPH_HDR}
	)



if(WIN32)

	source_group(Main\\Source FILES ${CPP_MAIN_SRC})
	source_group(Main\\Header FILES ${CPP_MAIN_HDR})

	source_group(Controllers\\Source FILES ${CPP_CONT_SRC})
	source_group(Controllers\\Header FILES ${CPP_CONT_HDR})

	source_group(Datafiles FILES ${CONTENT})
endif()


include_directories(
	${OGREKIT_INCLUDE}
	.
	
)

link_libraries(
	${OGREKIT_LIB}
)


if (APPLE)

	ogrekit_add_executable(
		${TargetName} 
		MACOSX_BUNDLE
		MainMenu.nib
		${SRC} 
	)

else ()

	ogrekit_add_executable(${TargetName} ${SRC})
	
	
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
