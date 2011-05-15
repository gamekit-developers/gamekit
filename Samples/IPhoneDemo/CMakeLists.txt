# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)



include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
SET(EXETYPE MACOSX_BUNDLE)
SET_SOURCE_FILES_PROPERTIES(MainMenu.nib    PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
SET_SOURCE_FILES_PROPERTIES(momo_ogre_i.blend PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
SET_SOURCE_FILES_PROPERTIES(multitouch_test.blend PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
SET_SOURCE_FILES_PROPERTIES(icon.png PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
SET_SOURCE_FILES_PROPERTIES(icon@2x.png PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
SET_SOURCE_FILES_PROPERTIES(icon-72.png PROPERTIES MACOSX_PACKAGE_LOCATION Resources )

SET(NibFile MainMenu.nib)


set(SRC 
	momo_ogre_i.blend
	multitouch_test.blend
	Main.mm
)

include_directories(
	${OGREKIT_INCLUDE}
)

link_libraries(
	${OGREKIT_LIB}
)


set(HiddenCMakeLists ../CMakeLists.txt)
source_group(ParentCMakeLists FILES ${HiddenCMakeLists})



add_executable(
	IPhoneDemo MACOSX_BUNDLE
	MainMenu.nib
	icon.png
	icon@2x.png
	icon-72.png
	${SRC} 
	${HiddenCMakeLists}
	)

set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.\${PRODUCT_NAME:rfc1034identifier}")
set_property(TARGET IPhoneDemo PROPERTY MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)

config_target_if_iphone_build(IPhoneDemo FALSE)
