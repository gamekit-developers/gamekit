set(OIS_SRC 
	src/OISEffect.cpp
	src/OISException.cpp
	src/OISForceFeedback.cpp
	src/OISInputManager.cpp
	src/OISJoyStick.cpp
	src/OISKeyboard.cpp
	src/OISObject.cpp
)

if (OGREKIT_BUILD_IPHONE)
	set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/OISInputManager.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CMake/OISSettings.in ${CMAKE_CURRENT_BINARY_DIR}/OISSettings.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(OIS_HDR
	include/OIS.h
	include/OISConfig.h
	include/OISEffect.h
	include/OISEvents.h
	include/OISException.h
	include/OISFactoryCreator.h
	include/OISForceFeedback.h
	include/OISInputManager.h
	include/OISInterface.h
	include/OISJoyStick.h
	include/OISKeyboard.h
	include/OISMouse.h
	include/OISMultiTouch.h
	include/OISObject.h
	include/OISPrereqs.h
	include/OISLog.h
	${CMAKE_CURRENT_BINARY_DIR}/OISSettings.h
)


if (WIN32)

	set(OIS_BUILD_WIN32 FALSE)
	if (OGREKIT_OIS_WIN32_NATIVE)
		set(OIS_BUILD_WIN32 TRUE)
	else ()
		if (NOT DirectX_FOUND)
			set(OIS_BUILD_WIN32 TRUE)
		endif()
	endif()
	
	if (NOT OIS_BUILD_WIN32)
		set(OIS_PLAT 
			src/win32/Win32ForceFeedback.cpp
			src/win32/Win32InputManager.cpp
			src/win32/Win32JoyStick.cpp
			src/win32/Win32KeyBoard.cpp
			src/win32/Win32Mouse.cpp
			include/win32/Win32ForceFeedback.h
			include/win32/Win32InputManager.h
			include/win32/Win32JoyStick.h
			include/win32/Win32KeyBoard.h
			include/win32/Win32Mouse.h
			include/win32/Win32Prereqs.h
			)		
		link_libraries(${DirectX_LIBRARIES}) #link with directinput/xinput
	else ()
		set(OIS_PLAT 
			include/win32native/Win32NativeInputManager.h
			include/win32native/Win32NativeJoystick.h
			include/win32native/Win32NativeKeyboard.h
			include/win32native/Win32NativeMouse.h
			include/win32native/Win32NativePrereqs.h
			src/win32native/Win32NativeInputManager.cpp
			src/win32native/Win32NativeKeyboard.cpp
			src/win32native/Win32NativeJoystick.cpp
			src/win32native/Win32NativeMouse.cpp
			)
	endif ()

endif()


if (APPLE) 
	if (OGREKIT_BUILD_IPHONE)
		#add_definitions(-DOIS_IPHONE_PLATFORM)
		set(OIS_PLAT
			include/iphone/iPhoneAccelerometer.h
			include/iphone/iPhoneHelpers.h
			include/iphone/iPhoneInputManager.h
			include/iphone/iPhoneMultiTouch.h
			include/iphone/iPhonePrereqs.h

			src/iphone/iPhoneAccelerometer.mm
			src/iphone/iPhoneInputManager.mm
			src/iphone/iPhoneMultiTouch.mm
		)
	else()
		set(OIS_PLAT 
			include/mac/MacHIDManager.h
			include/mac/MacHelpers.h
			include/mac/MacInputManager.h
			include/mac/MacJoyStick.h
			include/mac/MacKeyboard.h
			include/mac/MacMouse.h
			include/mac/MacPrereqs.h

			src/mac/MacHIDManager.cpp
			src/mac/MacHelpers.cpp
			src/mac/MacInputManager.cpp
			src/mac/MacJoyStick.cpp
			src/mac/MacKeyboard.cpp
			src/mac/MacMouse.cpp
		)
	endif()
	
	if (IOKit_FOUND)
		link_libraries(${IOKit_LIBRARIES})
	endif()
elseif (UNIX)

	if (OGREKIT_BUILD_ANDROID)
		set(OIS_PLAT
			include/android/AndroidInputManager.h
			
			src/android/AndroidInputManager.cpp
		)
	elseif (OGREKIT_BUILD_NACL)		
		set(OIS_PLAT
			include/nacl/NaClInputManager.h
			
			src/nacl/NaClInputManager.cpp
		)      
	else () 		
		set(OIS_PLAT 
			include/linux/EventHelpers.h
			include/linux/LinuxForceFeedback.h
			include/linux/LinuxInputManager.h
			include/linux/LinuxJoyStickEvents.h
			include/linux/LinuxKeyboard.h
			include/linux/LinuxMouse.h
			include/linux/LinuxPrereqs.h

			src/linux/EventHelpers.cpp
			src/linux/LinuxForceFeedback.cpp
			src/linux/LinuxInputManager.cpp
			src/linux/LinuxJoyStickEvents.cpp
			src/linux/LinuxKeyboard.cpp
			src/linux/LinuxMouse.cpp
		)
	endif()
endif()

include_directories(${OGREKIT_OIS_INCLUDE} )
add_library(OIS ${OIS_SRC} ${OIS_HDR} ${OIS_PLAT})

config_ogrekit_target(OIS TRUE)

if (OGREKIT_CREATE_OGRE_DEPENDENCY_DIR)
	file(MAKE_DIRECTORY ${OGRE_PREBUILT_DEPENDENCY_INCLUDE_DIR}/OIS)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${OGRE_PREBUILT_DEPENDENCY_INCLUDE_DIR}/OIS PATTERN "include/*")
	GET_TARGET_PROPERTY(FILEPATH OIS LOCATION)
	ADD_CUSTOM_COMMAND(TARGET OIS POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${FILEPATH} ${OGRE_PREBUILT_DEPENDENCY_LIB_DIR})
endif()
