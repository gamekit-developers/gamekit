INCLUDE_DIRECTORIES(
${BULLET_PHYSICS_SOURCE_DIR}/src  
../BulletFileLoader
../BlenderSerialize
)

ADD_LIBRARY(BlenderSerialize dna_252_32.cpp dna_252_64.cpp bBlenderFile.cpp  bBlenderFile.h bMain.cpp bMain.h )
