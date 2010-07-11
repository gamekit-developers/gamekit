# ---------------------------------------------------------
if (APPLE)
    include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
    SET(EXETYPE MACOSX_BUNDLE)
    SET_SOURCE_FILES_PROPERTIES( MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
    SET_SOURCE_FILES_PROPERTIES( MomoAnimation.blend OgreKit.js PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
    SET (NibFile MainMenu.nib)
endif()


set(SRC MomoAnimation.blend OgreKit.js Main.cpp)

if (OGREKIT_COMPLIE_SWIG)
    set(Script_HEADER
        Module/OgreKitApplication.h
        Module/OgreKitTypemaps.i
     )

    set(Swig_INPUT
        Module/OgreKit.i
        )

    set(SWIG_BUILD_DEPS
        ${Script_HEADER}
    )

    swig_source_v8(Swig_OUTPUT_V8 SWIG_BUILD_DEPS ${Swig_INPUT})

    set(Script_ALLGEN
        ${Swig_INPUT}
        ${Swig_OUTPUT_V8}
    )

    set(Script_SOURCE
        ${Script_ALLGEN}
        Module/OgreKitApplication.cpp
    )
    
    list(APPEND SRC ${Script_SOURCE} ${Script_HEADER})

else()
    set(Script_HEADER
        Module/OgreKitApplication.h
     )
    set(Script_SOURCE
        Module/OgreKitApplication.cpp
        Module/Generated/OgreKitV8.cpp
    )
    list(APPEND SRC ${Script_SOURCE} ${Script_HEADER})

endif()



include_directories(
    ${OGREKIT_OGRE_INCLUDE}
    ${OGREKIT_BULLET_INCLUDE}
    ${OGREKIT_OPENSTEER_INCLUDE}
    ${OGREKIT_V8_INCLUDE}
    ../../Engine/
    ../../Engine/AI
    ../../Dependencies/Source/tclap/include
    Module
)

link_libraries(
    OgreKitCore
    ${OGREKIT_OGRE_LIBS}
    ${OGREKIT_BULLET_LIBS}
)


set(HiddenCMakeLists ../CMakeLists.txt)
source_group(ParentCMakeLists FILES ${HiddenCMakeLists})


if (APPLE)

	add_executable(
	    V8Test 
	    MACOSX_BUNDLE
	    MainMenu.nib
	    ${SRC} 
	    ${HiddenCMakeLists}
    )

else ()

	add_executable(V8Test ${SRC}  ${HiddenCMakeLists})

endif()
