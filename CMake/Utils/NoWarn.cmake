
if (CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-w)
else()
	add_definitions(/W0)
endif()
