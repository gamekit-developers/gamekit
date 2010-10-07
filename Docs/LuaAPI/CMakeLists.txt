
# ---------------------------------------------------------
include(FindDoxygen)


file(GLOB_RECURSE SRC Source/*.inl Source/*.h)


set(OGREKIT_DOC_IMAGES 
	${CMAKE_CURRENT_SOURCE_DIR}/bg_t.gif
	${CMAKE_CURRENT_SOURCE_DIR}/top_bg.gif
	${CMAKE_CURRENT_SOURCE_DIR}/top__sm.gif
	)

if (DOXYGEN_FOUND)

	if(WIN32)
		set(CPCMD copy ${OGREKIT_DOC_IMAGES} ${CMAKE_CURRENT_SOURCE_DIR}/local/html/)
	else()
		set(CPCMD cp ${OGREKIT_DOC_IMAGES} ${CMAKE_CURRENT_SOURCE_DIR}/local/html/)
	endif()
	
	add_custom_target(LuaApiManual
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.txt && ${CPCMD}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
		COMMENT "Building OgreKit Lua Documentation" 
		SOURCES ${SRC}
	)
endif()
