# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

subdirs(bAscii)

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
    bAscii/bAscii/
    bAscii/bAscii/bApi
)

link_libraries(
    bAscii
)

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
