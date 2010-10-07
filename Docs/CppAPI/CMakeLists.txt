
# ---------------------------------------------------------
include(FindDoxygen)


file(GLOB_RECURSE SRC Example/*.cpp Example/*.h)

set(OGREKIT_DOC_IMAGES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Images)
	
set(OGREKIT_DOXYFILE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/Docyfile-gen.txt)


if (DOXYGEN_FOUND)

	if(WIN32)
		set(CATCMD type ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.txt > ${OGREKIT_DOXYFILE_CPP})
		set(ECHOCMD echo INPUT= ${OGREKIT_SOURCE_DIR}/Engine/ >> ${OGREKIT_DOXYFILE_CPP})
	else()
		set(CATCMD cat ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.txt > ${OGREKIT_DOXYFILE_CPP})
		set(ECHOCMD echo INPUT= ${OGREKIT_SOURCE_DIR}/Engine/ >> ${OGREKIT_DOXYFILE_CPP})
	endif()

	set(CPCMD ${CMAKE_COMMAND} -E copy_directory ${OGREKIT_DOC_IMAGES_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/local/html)

	add_custom_target(CppApiManual
		COMMAND ${CATCMD} && ${ECHOCMD} && ${DOXYGEN_EXECUTABLE} ${OGREKIT_DOXYFILE_CPP} && ${CPCMD}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
		COMMENT "Building OgreKit C++ Documentation" 
		SOURCES ${SRC}
	)
else()
	message("doxygen not found")
endif()
