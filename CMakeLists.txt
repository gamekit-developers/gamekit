cmake_minimum_required(VERSION 2.4)

PROJECT(GAMEKIT)
SET(GAMEKIT 2.75)

IF (NOT CMAKE_BUILD_TYPE)
# SET(CMAKE_BUILD_TYPE "Debug")
 SET(CMAKE_BUILD_TYPE "Release")
ENDIF (NOT CMAKE_BUILD_TYPE) 

 
  
IF(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -D_IRR_STATIC_LIB_ )
#ADD_DEFINITIONS( -D_IRR_STATIC_LIB_ )


#for now, just check for this X11 library on every platform, ignore on Mac and Win
#Linux and perhaps FreeBSD?
IF ( UNIX AND NOT APPLE )
find_library( X11_VIDEO_LIBRARY Xxf86vm
                        /usr/lib64
                        /usr/lib
                        /usr/local/lib64
                /usr/local/lib
                        /sw/lib
                        /opt/local/lib
                        DOC "Xxf86vm"
                )

                if( X11_VIDEO_LIBRARY )
                        message( "--- Looking for Xxf86vm - found" )
                        message( "--- Libs: " ${X11_VIDEO_LIBRARY} )
			SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -D_IRR_LINUX_X11_VIDMODE_ )
#			add_definitions( -D_IRR_LINUX_X11_VIDMODE_ )              
		  else( X11_VIDEO_LIBRARY )
                        message( "--- Looking for Xxf86vm - not found" )
                endif( X11_VIDEO_LIBRARY )

                set( X11_LIBRARIES ${X11_LIBRARIES} ${X11_VIDEO_LIBRARY} )

ENDIF (UNIX AND NOT APPLE)

   
SUBDIRS(bullet)
SUBDIRS(irrlicht-1.6)
SUBDIRS(gamekit)
