INCLUDE_DIRECTORIES( ${BULLET_PHYSICS_SOURCE_DIR}/src  
${BULLET_PHYSICS_SOURCE_DIR}/src
${CMAKE_SOURCE_DIR}/Serialize/CommonSerialize
)

ADD_LIBRARY(BlenderSerialize dna249.cpp dna249-64bit.cpp bBlenderFile.cpp  bBlenderFile.h bMain.cpp bMain.h )
