


set(UT_XML	utXML.h utXML.cpp)
file(GLOB UT_EXPAT_H *expat/*.h)
file(GLOB UT_EXPAT_S *expat/*.c)

source_group(expat FILES ${UT_EXPAT_H} ${UT_EXPAT_S})

add_definitions(-DHAVE_EXPAT_CONFIG_H -DXML_STATIC)
include_directories(. ../ expat)
add_library(UtilsXML ${UT_XML} ${UT_EXPAT_H} ${UT_EXPAT_S})