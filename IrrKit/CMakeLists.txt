
# You shouldn't have to modify anything below this line 
########################################################


IF (WIN32 AND MSVC)
 SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DYNAMICBASE")
ENDIF (WIN32 AND MSVC)

LINK_DIRECTORIES(
"C:/MinGW/lib"
)

INCLUDE_DIRECTORIES(
${IRRLICHT_SOURCE_DIR}/include ${BULLET_PHYSICS_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/Serialize/CommonSerialize
		${CMAKE_SOURCE_DIR}/Serialize/BlenderSerialize
		${CMAKE_SOURCE_DIR}/Serialize/BulletSerialize
)

SET (EXTRA_LIBS )

IF(APPLE)
	SET(EXETYPE MACOSX_BUNDLE)   
	SET_SOURCE_FILES_PROPERTIES( MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
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



SET(IrrKit_SRCS
	clubsilo_packed.blend
	PhysicsAnimationBakingDemo.blend
	BulletBlendReaderNew.cpp
	BulletBlendReaderNew.h
	BulletIrrlichtUtils.cpp
	BulletIrrlichtUtils.h
	IrrBlendNew.cpp
	IrrBlendNew.h
	main.cpp
)
	

LINK_LIBRARIES(
	 BlenderSerialize BulletSerialize bParse BulletSoftBody BulletDynamics BulletCollision LinearMath Irrlicht ${EXTRA_LIBS} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${X11_VIDEO_LIBRARY} 
)



IF (APPLE)
	SET (NibFile MainMenu.nib)
	SET (RESOURCS $(NibFile))
	SET_SOURCE_FILES_PROPERTIES( PhysicsAnimationBakingDemo.blend   PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET_SOURCE_FILES_PROPERTIES( clubsilo_packed.blend   PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	ADD_EXECUTABLE(IrrKit
	MACOSX_BUNDLE
	MainMenu.nib
	${IrrKit_SRCS}
	)
ELSE (APPLE)
	ADD_EXECUTABLE(IrrKit
	${IrrKit_SRCS}
	${bParse_SRCS}

	)
ENDIF (APPLE)

IF(WIN32)
				ADD_CUSTOM_COMMAND(
					TARGET IrrKit
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${GAMEKIT_SOURCE_DIR}/IrrKit/clubsilo_packed.blend" ${CMAKE_CURRENT_BINARY_DIR}
				)
ENDIF(WIN32)
