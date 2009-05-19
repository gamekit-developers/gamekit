INCLUDE_DIRECTORIES(
${BULLET_PHYSICS_SOURCE_DIR}/Extras/readblend
)

ADD_LIBRARY(readblend
readblend.c
readblend.h
blendtype.h
abs-file.h
)
