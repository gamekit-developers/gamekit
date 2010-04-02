##
## Author: George Petasis
##

# - try to find GTK (and glib) and GTKGLArea
#  GTK2_INCLUDE_DIR   - Directories to include to use GTK
#  GTK2_LIBRARIES     - Files to link against to use GTK
#  GTK2_FOUND         - GTK was found
#
# - Other advanced variables:
#  GTK2_gtk_INCLUDE_PATH
#  GTK2_gdk_INCLUDE_PATH
#  GTK2_glibconfig_INCLUDE_PATH
#  GTK2_glib_INCLUDE_PATH
#  GTK2_cairo_INCLUDE_PATH
#  GTK2_pango_INCLUDE_PATH
#  GTK2_atk_INCLUDE_PATH
#  
#  GTK2_gtk_LIBRARY
#  GTK2_glib_LIBRARY
#  GTK2_gdk_LIBRARY
#  GTK2_gmodule_LIBRARY
#  GTK2_gobject_LIBRARY
#  GTK2_gthread_LIBRARY
#  GTK2_gio_LIBRARY
#  GTK2_cairo_LIBRARY
#  GTK2_pango_LIBRARY
#  GTK2_atk_LIBRARY

# - Usage:
#  The behaviour of the module can be altered through variables, defined
#  before the module is loaded. These (optional) variables are:
#
#  GTK2_ADDITIONAL_PATHS: A list of paths to search for GTK+
#  GTK2_LIBRARY_NAMES:    A list of names to be used when searching for
#                         libraries/include directories.
#                         If not initialised, defaults to:
#                         "-x11-2.0 -win32-2.0 -2.0 20 -win32 -x11
#                          -x11-1.0 -win32-1.0 -1.0 10"
#  GTK2_SEARCH_LIBRARIES: A list of additional libraries to be searched.
#                         If not initialised, defaults to:
#                         "gdk gmodule gobject gthread gio cairo pango atk"
#  GTK2_LOGGING_ENABLED:  If true, include/library paths will be printed.
#                         Defaults to "undefined".
#  

##############################################################################
## Configuration section
##############################################################################

SET ( GTK2_PATHS /usr /opt/gnome /usr/openwin /usr/local )

## GTK2_ADDITIONAL_PATHS: Additional paths to look for GTK+ 2.x
SET ( GTK2_ADDITIONAL_PATHS ${GTK2_ADDITIONAL_PATHS}
                           "$ENV{ProgramFiles}/gtk+-bundle-2.12.11" )

## GTK2_SEARCH_LIBRARIES_DEFAULT: This variable holds the names of libraries
## that will always be searched by this module.
SET ( GTK2_SEARCH_LIBRARIES_DEFAULT gtk glib )

## GTK2_LIBRARY_NAMES: A set of names under which libraries will be searched.
SET ( GTK2_LIBRARY_NAMES ${GTK2_LIBRARY_NAMES}
      -x11-2.0 -win32-2.0 -2.0 20 -win32 -x11
      -x11-1.0 -win32-1.0 -1.0 10 )

## GTK2_SEARCH_LIBRARIES: This variable holds the names of libraries that can
## be optionally searched by the module. If not specified by the caller, it is
## set to a large number of libraries.
IF ( NOT GTK2_SEARCH_LIBRARIES )
  SET ( GTK2_SEARCH_LIBRARIES gdk gmodule gobject gthread
                              gio cairo pango atk )
ENDIF ( NOT GTK2_SEARCH_LIBRARIES )

SET ( GTK2_INCLUDE_FILES_INFO 
  gtk        gtk/gtk.h
  gdk        gdkconfig.h
  glibconfig glibconfig.h
  glib       glib.h
  cairo      cairo.h
  pango      pango/pango.h
  atk        atk/atkobject.h
)

##############################################################################
## End of Configuration section
##############################################################################

## Convert GTK2_ADDITIONAL_PATHS to
## GTK2_ADDITIONAL_INCLUDE_PATHS & GTK2_ADDITIONAL_LIBRARY_PATHS
FOREACH ( gtk2_path ${GTK2_ADDITIONAL_PATHS} )
  FILE ( TO_CMAKE_PATH ${gtk2_path} gtk2_path_cmake )
  SET ( GTK2_ADDITIONAL_INCLUDE_PATHS ${GTK2_ADDITIONAL_INCLUDE_PATHS}
    ${gtk2_path_cmake}/include
    ${gtk2_path_cmake}/share/include
    ${gtk2_path_cmake}/include/gtk-2.0
    ${gtk2_path_cmake}/lib/gtk-2.0/include
    ${gtk2_path_cmake}/lib64/gtk-2.0/include
    ${gtk2_path_cmake}/lib/glib-2.0/include
    ${gtk2_path_cmake}/lib64/glib-2.0/include
  )
  SET ( GTK2_ADDITIONAL_LIBRARY_PATHS ${GTK2_ADDITIONAL_LIBRARY_PATHS}
    ${gtk2_path_cmake}/lib ${gtk2_path_cmake}/lib64 )
ENDFOREACH ( gtk2_path )
MESSAGE ( STATUS ${GTK2_ADDITIONAL_LIBRARY_PATHS} )

## Convert GTK2_PATHS to
## GTK2_INCLUDE_PATHS & GTK2_LIBRARY_PATHS
FOREACH ( gtk2_path ${GTK2_PATHS} )
  FILE ( TO_CMAKE_PATH ${gtk2_path} gtk2_path_cmake )
  SET ( GTK2_INCLUDE_PATHS ${GTK2_INCLUDE_PATHS}
    ${gtk2_path_cmake}/include
    ${gtk2_path_cmake}/share/include
    ${gtk2_path_cmake}/include/gtk-2.0
    ${gtk2_path_cmake}/lib/gtk-2.0/include
    ${gtk2_path_cmake}/lib64/gtk-2.0/include
    ${gtk2_path_cmake}/lib/glib-2.0/include
    ${gtk2_path_cmake}/lib64/glib-2.0/include
  )
  SET ( GTK2_LIBRARY_PATHS ${GTK2_LIBRARY_PATHS}
    ${gtk2_path_cmake}/lib ${gtk2_path_cmake}/lib64 )
ENDFOREACH ( gtk2_path )

##
## Function: GTK2_GET_NAMES
##
FUNCTION ( GTK2_GET_NAMES library variable )
  FOREACH ( name ${GTK2_LIBRARY_NAMES} )
    SET ( ${variable} ${${variable}} ${library}${name} )
  ENDFOREACH ( name )
  SET ( ${variable}  ${${variable}} ${library} )
  SET ( ${variable}  ${${variable}} PARENT_SCOPE )
ENDFUNCTION ( GTK2_GET_NAMES )

LIST ( LENGTH GTK2_INCLUDE_FILES_INFO GTK2_INCLUDE_FILES_INFO_LEN )
MATH ( EXPR GTK2_INCLUDE_FILES_INFO_LEN
         "${GTK2_INCLUDE_FILES_INFO_LEN}-1" )
FOREACH ( gtk2_i RANGE 0 ${GTK2_INCLUDE_FILES_INFO_LEN} 2 )
  LIST ( GET GTK2_INCLUDE_FILES_INFO ${gtk2_i} gtk2_library )
  MATH ( EXPR gtk2_i ${gtk2_i}+1 )
  LIST ( GET GTK2_INCLUDE_FILES_INFO ${gtk2_i} gtk2_header )
  GTK2_GET_NAMES ( ${gtk2_library} GTK2_${gtk2_library}_PATH_SUFFIXES )
  FIND_PATH ( GTK2_${gtk2_library}_INCLUDE_PATH
    PATH_SUFFIXES ${GTK2_${gtk2_library}_PATH_SUFFIXES}
    NAMES ${gtk2_header}
    PATHS ${GTK2_INCLUDE_PATHS}
          ${GTK2_ADDITIONAL_INCLUDE_PATHS}
  )
  IF ( GTK2_LOGGING_ENABLED )
    MESSAGE ( STATUS "   + ${gtk2_header}: \t${GTK2_${gtk2_library}_INCLUDE_PATH}" )
  ENDIF ( GTK2_LOGGING_ENABLED )
  MARK_AS_ADVANCED ( GTK2_${gtk2_library}_INCLUDE_PATH )
  IF ( GTK2_${gtk2_library}_INCLUDE_PATH )
    SET (GTK2_INCLUDE_DIR ${GTK2_INCLUDE_DIR} ${GTK2_${gtk2_library}_INCLUDE_PATH} )
  ENDIF ( GTK2_${gtk2_library}_INCLUDE_PATH )
ENDFOREACH ( gtk2_i )

##
## Locate GTK+ 2.0 libraries...
##

FOREACH ( gtk2_library ${GTK2_SEARCH_LIBRARIES_DEFAULT} ${GTK2_SEARCH_LIBRARIES} )
  GTK2_GET_NAMES ( ${gtk2_library} GTK2_${gtk2_library}_LIBRARY_NAMES )
  FIND_LIBRARY ( GTK2_${gtk2_library}_LIBRARY
    NAMES ${GTK2_${gtk2_library}_LIBRARY_NAMES}
    PATHS ${GTK2_LIBRARY_PATHS}
          ${GTK2_ADDITIONAL_LIBRARY_PATHS}
  )
  IF ( GTK2_LOGGING_ENABLED )
    MESSAGE ( STATUS "   + ${gtk2_library}: \t${GTK2_${gtk2_library}_LIBRARY}" )
  ENDIF ( GTK2_LOGGING_ENABLED )
  MARK_AS_ADVANCED ( GTK2_${gtk2_library}_LIBRARY )
ENDFOREACH ( gtk2_library )

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS ( GTK2 DEFAULT_MSG
  GTK2_gtk_LIBRARY      GTK2_glib_LIBRARY
  GTK2_gtk_INCLUDE_PATH GTK2_glib_INCLUDE_PATH
)

IF ( GTK2_FOUND )
  FOREACH ( gtk2_library ${GTK2_SEARCH_LIBRARIES_DEFAULT} )
    SET ( GTK2_LIBRARIES ${GTK2_LIBRARIES}
        ${GTK2_${gtk2_library}_LIBRARY} )
  ENDFOREACH ( gtk2_library )
ENDIF ( GTK2_FOUND )
