
# ---------------------------------------------------------
include(FindDoxygen)


file(GLOB_RECURSE SRC Example/*.cpp Example/*.h)

set(OGREKIT_DOC_IMAGES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Images)
	
set(OGREKIT_DOXYFILE_CPP_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.txt.in)
set(OGREKIT_DOXYFILE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.txt)
set(DOXYFILE_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/html)

if (DOXYGEN_FOUND)
	configure_file(${OGREKIT_DOXYFILE_CPP_IN} ${OGREKIT_DOXYFILE_CPP})

	set(CPCMD ${CMAKE_COMMAND} -E copy_directory ${OGREKIT_DOC_IMAGES_DIR} ${DOXYFILE_OUTDIR})
	set(RMCMD ${CMAKE_COMMAND} -E remove_directory ${DOXYFILE_OUTDIR}/.svn)

	add_custom_target(CppApiManual
		COMMAND ${DOXYGEN_EXECUTABLE} ${OGREKIT_DOXYFILE_CPP} && ${CPCMD} && ${RMCMD}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
		COMMENT "Building OgreKit C++ Documentation" 
		SOURCES ${SRC}
	)
else()
	message("doxygen not found")
endif()
