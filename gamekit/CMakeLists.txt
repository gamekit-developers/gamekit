
# You shouldn't have to modify anything below this line 
########################################################


IF (WIN32 AND MSVC)
 SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DYNAMICBASE")
ENDIF (WIN32 AND MSVC)


LINK_DIRECTORIES(
"C:/MinGW/lib"
)

INCLUDE_DIRECTORIES(
${IRRLICHT_SOURCE_DIR}/include ${BULLET_PHYSICS_SOURCE_DIR}/Extras/readblend ${BULLET_PHYSICS_SOURCE_DIR}/src
)

SET (EXTRA_LIBS )

IF(APPLE)
	SET(EXETYPE MACOSX_BUNDLE)   
	set_source_files_properties(MainMenu.nib PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

	#SET(GUI_TYPE MACOSX_BUNDLE)
   INCLUDE_DIRECTORIES ( /Developer/Headers/FlatCarbon )
   FIND_LIBRARY(CARBON_LIBRARY Carbon)
   FIND_LIBRARY(QUICKTIME_LIBRARY QuickTime )
   FIND_LIBRARY(APP_SERVICES_LIBRARY ApplicationServices )
   FIND_LIBRARY(Foundation_FRAMEWORKS  Foundation )
   FIND_LIBRARY(CoreFoundation_FRAMEWORKS CoreFoundation )
   FIND_LIBRARY(APPKIT_LIBRARY AppKit )  
	 MARK_AS_ADVANCED (CARBON_LIBRARY
                     QUICKTIME_LIBRARY
                     APP_SERVICES_LIBRARY)
   SET(EXTRA_LIBS ${APPKIT_LIBRARY} ${Foundation_FRAMEWORKS} ${CoreFoundation_FRAMEWORKS}  ${CARBON_LIBRARY} ${QUICKTIME_LIBRARY} ${APP_SERVICES_LIBRARY})

IF (X11_VIDEO_LIBRARY)
        SET(EXTRA_LIBS ${EXTRA_LIBS} ${X11_VIDEO_LIBRARY} )
ENDIF (X11_VIDEO_LIBRARY)



ELSE (APPLE)

IF (X11_VIDEO_LIBRARY)
	SET(EXTRA_LIBS ${EXTRA_LIBS} Xxf86vm )
ENDIF (X11_VIDEO_LIBRARY)

ENDIF (APPLE)

                if( X11_VIDEO_LIBRARY )
                        message( "found again" )
		else( X11_VIDEO_LIBRARY )
                        message( "not found" )
                endif( X11_VIDEO_LIBRARY )

#ENDIF (LINUX)


LINK_LIBRARIES(
	readblend  BulletSoftBody BulletDynamics BulletCollision LinearMath Irrlicht ${EXTRA_LIBS} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${X11_VIDEO_LIBRARY}
)

IF (APPLE)
	SET (NibFile MainMenu.nib)
	SET (RESOURCES $(NibFile))
	SET_SOURCE_FILES_PROPERTIES( PhysicsAnimationBakingDemo.blend   PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	ADD_EXECUTABLE(GameKit
	MACOSX_BUNDLE
	MainMenu.nib
	PhysicsAnimationBakingDemo.blend
	BulletBlendReader.cpp
	BulletBlendReader.h
	main.cpp)
ELSE (APPLE)
	ADD_EXECUTABLE(GameKit
	BulletBlendReader.cpp
	BulletBlendReader.h
	main.cpp)
ENDIF (APPLE)

