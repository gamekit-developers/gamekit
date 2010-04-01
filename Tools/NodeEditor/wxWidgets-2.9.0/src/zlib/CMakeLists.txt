
set(ZLIB_SRCS 
	adler32.c
	compress.c
	crc32.c
	deflate.c
	gzio.c
	infback.c
	inffast.c
	inflate.c
	inftrees.c
	trees.c
	uncompr.c
	zutil.c
)
set(ZLIB_HDRS zlib.h zconf.h)

include_directories(.)
add_library(wxZLib ${ZLIB_SRCS} ${ZLIB_HDRS})
