include(FindPkgMacros)
findpkg_begin(V8)

getenv_path(V8_HOME)

set(V8_PREFIX_PATH ${V8_HOME} ${ENV_V8_HOME})
create_search_paths(V8)

clear_if_changed(
    V8_PREFIX_PATH
    V8_LIBRARY
    V8_INCLUDE_DIR
    V8_SHELL
    V8_D8_SHELL
)
set(V8_INCLUDE_NAMES v8.h v8-debug.h v8-profiler.h)
set(V8_LIBRARY_NAMES v8)

find_program(V8_SHELL NAMES shell HINTS ${ENV_V8_HOME})
find_program(V8_D8_SHELL NAMES d8 HINTS ${ENV_V8_HOME})
find_path(V8_INCLUDE_DIR NAMES ${V8_INCLUDE_NAMES} HINTS ${V8_INC_SEARCH_PATH})
find_library(V8_LIBRARY NAMES ${V8_LIBRARY_NAMES} HINTS ${ENV_V8_HOME} ${V8_LIB_SEARCH_PATH})

make_library_set(V8_LIBRARY)
findpkg_finish(V8)
