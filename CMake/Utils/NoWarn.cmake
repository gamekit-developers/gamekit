
if (CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang") )
	add_definitions(-w)
else()
	add_definitions(/W0)
endif()
