INCLUDE_DIRECTORIES( 
	${BULLET_PHYSICS_SOURCE_DIR}/src  
	${CMAKE_SOURCE_DIR}/Serialize/CommonSerialize
)

ADD_LIBRARY(BulletSerialize BulletDNA.cpp btBulletFile.cpp btBulletFile.h )
