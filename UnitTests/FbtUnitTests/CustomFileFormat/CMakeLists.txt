include(MakeFBT)

include_directories(${FBT_SOURCE_DIR}/File)
link_libraries(
	${GTEST_LIB}
	fbtFile
)

set(CSTM_FORMAT_SRC
	customFile.cpp
	customFile.h
	Custom.h
)


add_fbt(cstmFile Custom.h)
#add_fbt_validator(cstmFile Custom.h)
add_library(cstmFileFormat ${CSTM_FORMAT_SRC} ${cstmFile})

