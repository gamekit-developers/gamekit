###############################################################################
PROJECT(GEN)
FILE(GLOB cpp_SRC "*.cpp")
FILE(GLOB h_SRC "*.h")


SET(includes 
  .
	${CMAKE_SOURCE_DIR}/Serialize/CommonSerialize
	${CMAKE_SOURCE_DIR}/Serialize/BlenderSerialize
	${CMAKE_SOURCE_DIR}/Serialize/BulletSerialize
	${CMAKE_SOURCE_DIR}/bullet/src
)


LINK_LIBRARIES(
	 BulletSerialize BlenderSerialize bParse LinearMath
)

INCLUDE_DIRECTORIES(${includes})

SET(Main_LIBS bParse LinearMath)

ADD_EXECUTABLE(HeaderGenerator ${cpp_SRC} ${h_SRC})
