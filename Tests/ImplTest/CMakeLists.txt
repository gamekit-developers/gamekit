# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)



set(SRC Main.cpp)
include_directories(
    ${OGREKIT_OGRE_INCLUDE}
    ${OGREKIT_SOURCE_DIR}/Core/Engine
)

link_libraries(
    OgreKitCore 
    ${OGREKIT_OGRE_LIBS}
)

if (WIN32)
	if (CMAKE_COMPILER_IS_GNUCXX)
		# FreeImage LibRawLite dependency 
		link_libraries(ws2_32)
	endif (CMAKE_COMPILER_IS_GNUCXX)
endif(WIN32)

add_executable(TestApp ${SRC})
install(TARGETS TestApp RUNTIME DESTINATION ${OGREKIT_INSTALL_PREFIX})

set(HomeFiles 
	BigEndian.blend
	UVLayers.blend
	VertexColors.blend
	clubsilo_packed.blend
	momo_ogreSmallAnim.blend
	MultiParentTest.blend
	MultiMat225.blend
	MultiMat249.blend
	monkit2.blend
)

install(FILES ${HomeFiles} DESTINATION ${OGREKIT_INSTALL_PREFIX})
