
# You shouldn't have to modify anything below this line 
########################################################

ADD_DEFINITIONS( -D_IRR_STATIC_LIB_ )

LINK_DIRECTORIES(
"C:/MinGW/lib"
)

INCLUDE_DIRECTORIES(
${IRRLICHT_SOURCE_DIR}/include ${BULLET_PHYSICS_SOURCE_DIR}/Extras/readblend ${BULLET_PHYSICS_SOURCE_DIR}/src
)

LINK_LIBRARIES(
readblend  BulletSoftBody BulletDynamics BulletCollision LinearMath Irrlicht ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY}
)



ADD_EXECUTABLE(GameKit
BulletBlendReader.cpp
BulletBlendReader.h
main.cpp
)

