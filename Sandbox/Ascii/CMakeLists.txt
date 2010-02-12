# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)



if (APPLE)
include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
        SET(EXETYPE MACOSX_BUNDLE)
        SET_SOURCE_FILES_PROPERTIES( MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
 	SET_SOURCE_FILES_PROPERTIES( momo_ogreSmallAnim.bascii PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET (NibFile MainMenu.nib)
endif()

set(SRC 
	BigEndian.bascii
	CompileError.bascii
	InvalidArg.bascii
	InvalidArgCount.bascii
	InvalidKeyword.bascii
	InvalidStride.bascii
	InvalidSyntax.bascii
	MaxArg.bascii
	MultiMat249.bascii
	UVLayers.bascii
	VertexColors.bascii
	momo_ogreSmallAnim.bascii
	Main.cpp
	)
include_directories(
    ${OGREKIT_OGRE_INCLUDE}
    ${OGREKIT_SOURCE_DIR}/Tests/Ascii/bAscii/bAscii/
    ${OGREKIT_SOURCE_DIR}/Tests/Ascii/bAscii/bAscii/bApi
    ${OGREKIT_SOURCE_DIR}/Core/Engine
    ${OGREKIT_SOURCE_DIR}/Core/Engine/Loaders
    ${OGREKIT_SOURCE_DIR}/Core/Engine/Loaders/Internal
)

link_libraries(
    OgreKitCore 
    bAscii
    ${OGREKIT_OGRE_LIBS}
)

if (WIN32)
	if (CMAKE_COMPILER_IS_GNUCXX)
		# FreeImage LibRawLite dependency 
		link_libraries(ws2_32)
	endif (CMAKE_COMPILER_IS_GNUCXX)
endif(WIN32)

if (APPLE)
	add_executable(
	Ascii
	MACOSX_BUNDLE
	MainMenu.nib
	${SRC}
)
else ()
	add_executable(Ascii ${SRC})
endif()
