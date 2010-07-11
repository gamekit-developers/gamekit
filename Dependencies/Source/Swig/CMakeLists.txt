
file(GLOB SWIG_SRC
    CParse/*.c
    DOH/*.c
    Preprocessor/*.c
    Swig/*.c
    Modules/*.cxx
    )
file(GLOB SWIG_HDR
    CParse/*.h
    DOH/*.h
    Preprocessor/*.h
    Swig/*.h
    Modules/*.h
    )


file(GLOB SWIG_LIB
    Lib/V8/*.swg
    Lib/V8/*.i
    Lib/*.swg
)


if (MSVC)
    add_definitions(/wd4273)
endif()

source_group(V8\\Src FILES Modules/V8.cxx)
source_group(V8\\Lib FILES ${SWIG_LIB})

include_directories(CParse DOH Preprocessor Swig Modules Include)
add_executable(Swig ${SWIG_SRC} ${SWIG_HDR} ${SWIG_LIB})
