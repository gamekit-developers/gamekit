# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)


set(FREETYPE_SRC 
	src/autofit/autofit.c
	src/bdf/bdf.c
	src/cff/cff.c
	src/base/ftbase.c
	src/base/ftbbox.c
	src/base/ftbdf.c
	src/base/ftbitmap.c
	src/cache/ftcache.c
	src/base/ftgasp.c
	src/base/ftglyph.c
	src/base/ftgxval.c
	src/gzip/ftgzip.c
	src/base/ftinit.c
	src/lzw/ftlzw.c
	src/base/ftmm.c
	src/base/ftotval.c
	src/base/ftpfr.c
	src/base/ftstroke.c
	src/base/ftsynth.c
	src/base/ftsystem.c
	src/base/fttype1.c
	src/base/ftwinfnt.c
	src/base/ftxf86.c
	src/pcf/pcf.c
	src/pfr/pfr.c
	src/psaux/psaux.c
	src/pshinter/pshinter.c
	src/psnames/psmodule.c
	src/raster/raster.c
	src/sfnt/sfnt.c
	src/smooth/smooth.c
	src/truetype/truetype.c
	src/type1/type1.c
	src/cid/type1cid.c
	src/type42/type42.c
	src/winfonts/winfnt.c
)

if (WIN32)
	set(FREETYPE_PLATFORM
		win32/ftdebug.c
		)
endif (WIN32)

## TODO TEST OTHER PLATFORMS

if (APPLE AND OGREKIT_BUILD_IPHONE)
	add_definitions(-DDARWIN_NO_CARBON)
endif()


include_directories(include)
add_definitions(-DFT2_BUILD_LIBRARY)
add_library(${OGREKIT_FREETYPE_TARGET} ${FREETYPE_SRC} ${FREETYPE_PLATFORM})

config_ogrekit_target(${OGREKIT_FREETYPE_TARGET} TRUE)

if (OGREKIT_CREATE_OGRE_DEPENDENCY_DIR)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${OGRE_PREBUILT_DEPENDENCY_INCLUDE_DIR} PATTERN "include/*")
	GET_TARGET_PROPERTY(FILEPATH ${OGREKIT_FREETYPE_TARGET} LOCATION)
	ADD_CUSTOM_COMMAND(TARGET ${OGREKIT_FREETYPE_TARGET} POST_BUILD
		COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${FILEPATH} ${OGRE_PREBUILT_DEPENDENCY_LIB_DIR}
	)
endif()
