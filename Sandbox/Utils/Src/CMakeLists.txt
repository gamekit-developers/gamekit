
set(UT_HDR
	utCommon.h
	utStream.h
	utString.h
	utTypes.h
	)

set(UT_SRC utStream.cpp utString.cpp)

include_directories(.)

add_library(Utils	${UT_SRC} 
					${UT_HDR})
					
subdirs(XML)