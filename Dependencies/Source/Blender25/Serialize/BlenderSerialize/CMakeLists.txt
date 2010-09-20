INCLUDE_DIRECTORIES(
${BULLET_PHYSICS_SOURCE_DIR}/src  
../BulletFileLoader
../BlenderSerialize
)

ADD_LIBRARY(BlenderSerialize dna_32.cpp dna_64.cpp bBlenderFile.cpp  bBlenderFile.h bMain.cpp bMain.h )

config_target_if_iphone_build(BlenderSerialize TRUE)
