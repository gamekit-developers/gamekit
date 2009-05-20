
# You shouldn't have to modify anything below this line 
########################################################

ADD_DEFINITIONS( -D_IRR_STATIC_LIB_ )

IF (WIN32)
 SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DYNAMICBASE")
ENDIF (WIN32)


LINK_DIRECTORIES(
"C:/MinGW/lib"
)

INCLUDE_DIRECTORIES(
${IRRLICHT_SOURCE_DIR}/include ${BULLET_PHYSICS_SOURCE_DIR}/Extras/readblend ${BULLET_PHYSICS_SOURCE_DIR}/src
)

IF(APPLE)
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
ELSE (APPLE)
SET(EXTRA_LIBS )
ENDIF (APPLE)

LINK_LIBRARIES(
readblend  BulletSoftBody BulletDynamics BulletCollision LinearMath Irrlicht ${EXTRA_LIBS} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY}
)



ADD_EXECUTABLE(GameKit
BulletBlendReader.cpp
BulletBlendReader.h
main.cpp
)

