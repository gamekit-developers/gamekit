include(FindPkgMacros)
findpkg_begin(XUL)

getenv_path(XUL_HOME)
getenv_path(XUL_PATH)
getenv_path(XUL_ROOT)



set(XUL_PREFIX_PATH ${XUL_HOME} ${ENV_XUL_HOME}
                    ${XUL_PATH} ${ENV_XUL_PATH}
                    ${XUL_ROOT} ${ENV_XUL_ROOT}
                    )
create_search_paths(XUL)

clear_if_changed(
    XUL_PREFIX_PATH
    XUL_XPCOM_LIBRARY
    XUL_XPCOM_LIBRARY    
    XUL_INCLUDE_DIR
)

set(XUL_INCLUDE_NAMES npapi.h)


find_path(XUL_INCLUDE_DIR       NAMES ${XUL_INCLUDE_NAMES}  HINTS ${XUL_INC_SEARCH_PATH})
find_library(XUL_XUL_LIBRARY    NAMES xul                   HINTS ${XUL_LIB_SEARCH_PATH})
find_library(XUL_XPCOM_LIBRARY  NAMES xpcom                 HINTS ${XUL_LIB_SEARCH_PATH})


make_library_set(XUL_LIBRARY)
findpkg_finish(XUL)
