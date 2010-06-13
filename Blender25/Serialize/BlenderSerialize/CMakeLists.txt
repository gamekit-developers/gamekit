INCLUDE_DIRECTORIES( ${BULLET_PHYSICS_SOURCE_DIR}/src  
${BULLET_PHYSICS_SOURCE_DIR}/src
${BULLET_PHYSICS_SOURCE_DIR}/Extras/Serialize/BulletFileLoader
${BULLET_PHYSICS_SOURCE_DIR}/Extras/Serialize/BlenderSerialize
)

ADD_LIBRARY(BlenderSerialize dna_252_32.cpp dna_252_64.cpp bBlenderFile.cpp  bBlenderFile.h bMain.cpp bMain.h )
