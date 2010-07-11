# -----------------------------------------------------------------------------
#
#  Compiletime generators for SWIG
#   http://www.swig.org/
#
# -----------------------------------------------------------------------------
set(SWIG_LIB_ROOT   "${OGREKIT_SOURCE_DIR}/Dependencies/Source/Swig")
set(SWIG_EXECUTABLE Swig)
# -----------------------------------------------------------------------------
#
# Get a list of absolute paths
#
# -----------------------------------------------------------------------------
macro(SWIG_SOURCE_ABSOLUTE_SRC OUT)
    foreach (it ${ARGN})
        get_filename_component(N ${it} ABSOLUTE)
        set(${OUT} ${${OUT}} ${N})
    endforeach(it)
endmacro(SWIG_SOURCE_ABSOLUTE_SRC)
# -----------------------------------------------------------------------------
#
# Generic call 
#
# For each ${INPUT_FILE}
#   Call ${SWIG} -o ${OUT_FILE} ${ARGUMENT_LIST} ${INPUT_FILE}
# 
#
# -----------------------------------------------------------------------------
macro(SWIG_SOURCE_LANGUAGE LANG GENDIR ARGS OUT DEPS)

    swig_source_absolute_src(ABS_DEPS ${${DEPS}})
    set(SWIG_REAL_LIB
        ${SWIG_LIB_ROOT}/Lib
    )
    file(GLOB_RECURSE LIB_FILES ${SWIG_REAL_LIB}/*.swg ${SWIG_REAL_LIB}/*.i)
    get_target_property(SWIG ${SWIG_EXECUTABLE} LOCATION)

    foreach (ARG ${ARGN})
    
        get_filename_component(IFILE ${ARG} ABSOLUTE)
        string(REPLACE ".i" "${LANG}" OFILE ${IFILE})

        string(REPLACE ".cpp" "" BENAME ${LANG})

        get_filename_component(INAME ${IFILE} NAME)
        get_filename_component(ONAME ${OFILE} NAME)

        set(ROFILE ${OFILE})
        if (NOT ${GENDIR} STREQUAL "")
            get_filename_component(OPATH ${OFILE} PATH)
            set(ROFILE "${OPATH}/${GENDIR}/${ONAME}")
        endif()
        add_custom_command(
            OUTPUT  ${ROFILE}
            COMMAND ${SWIG} -o ${ROFILE} ${${ARGS}} ${IFILE}
            DEPENDS ${SWIG} ${IFILE} ${ABS_DEPS} ${LIB_FILES}
            WORKING_DIRECTORY ${SWIG_LIB_ROOT}
            COMMENT "${SWIG_EXECUTABLE} ${ONAME} -${BENAME} ${INAME}"
        )
        list(APPEND ${OUT} ${ROFILE})
    endforeach(ARG)


endmacro(SWIG_SOURCE_LANGUAGE)
# -----------------------------------------------------------------------------
#
# V8 JavaScript 
#   http://code.google.com/p/v8/
#
#   -gcpad nr, Fakes the ammount of external memory as seen by V8.
#              The higher the number the more the V8 GC will collect
#              all unused memory. 
#
# -----------------------------------------------------------------------------
macro(SWIG_SOURCE_V8 OUT DEPS)
    set(ARGS "-V8")
    list(APPEND ARGS "-c++")
    list(APPEND ARGS "-gcpad")
    list(APPEND ARGS "1024")
    swig_source_language("V8.cpp" "Generated" ARGS ${OUT} ${DEPS} ${ARGN})
endmacro(SWIG_SOURCE_V8)
# -----------------------------------------------------------------------------
#
# Lua
#   http://www.lua.org/
#
# -----------------------------------------------------------------------------
macro(SWIG_SOURCE_LUA OUT DEPS)
    set(ARGS "-lua")
    list(APPEND ARGS "-c++")
    swig_source_language("Lua.cpp" "Generated" ARGS ${OUT} ${DEPS} ${ARGN})
endmacro(SWIG_SOURCE_LUA)
# -----------------------------------------------------------------------------
#
# XML Tests
#
# -----------------------------------------------------------------------------
macro(SWIG_SOURCE_XML OUT DEPS)
    set(ARGS "-xml")
    list(APPEND ARGS "-c++")
    swig_source_language(".xml" "Generated" ARGS ${OUT} ${DEPS} ${ARGN})
endmacro(SWIG_SOURCE_XML)
# -----------------------------------------------------------------------------
# 
# External XML Tests
#
# For each ${INPUT_FILE}
#   Call ${SWIG_XML} ${OUT_FILE} ${INPUT_FILE}
# 
#
# -----------------------------------------------------------------------------
macro(SWIGXML_SOURCE LANG GENDIR ARGS OUT DEPS)
    swig_source_absolute_src(ABS_DEPS ${${DEPS}})
    get_target_property(SWIGXML SwigXML LOCATION)

    swig_source_xml(XML_OUT ${DEPS} ${ARGN})
    foreach (ARG ${XML_OUT})
        
    
        get_filename_component(IFILE ${ARG} ABSOLUTE)
        string(REPLACE ".xml" "${LANG}" OFILE ${IFILE})

        get_filename_component(INAME ${IFILE} NAME)
        get_filename_component(ONAME ${OFILE} NAME)


        add_custom_command(
            OUTPUT  ${OFILE}
            COMMAND ${SWIGXML} ${OFILE} ${IFILE}
            DEPENDS ${SWIGXML} ${IFILE} ${ABS_DEPS} ${LIB_FILES}
            WORKING_DIRECTORY ${SWIG_LIB_ROOT}
            COMMENT "SwigXML ==> ${ONAME}"
        )
        list(APPEND ${OUT} ${OFILE})
        list(APPEND ${OUT} ${ARG})
    endforeach(ARG)
endmacro(SWIGXML_SOURCE)
# -----------------------------------------------------------------------------
#
# External XML Tests
#
# -----------------------------------------------------------------------------
macro(SWIGXML_SOURCE_XML OUT DEPS)
    swigxml_source(".cpp" "Generated" ARGS ${OUT} ${DEPS} ${ARGN})
endmacro(SWIGXML_SOURCE_XML)
