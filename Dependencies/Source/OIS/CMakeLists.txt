# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)


set(OIS_SRC 
    src/OISEffect.cpp
    src/OISException.cpp
    src/OISForceFeedback.cpp
    src/OISInputManager.cpp
    src/OISJoyStick.cpp
    src/OISKeyboard.cpp
    src/OISObject.cpp
)

set(OIS_HDR
    include/OIS.h
    #include/OISConfig.h
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
    include/OISObject.h
    #include/OISPrereqs.h
)

if (WIN32)
	#set(DirectX_FOUND 0)

	if (DirectX_FOUND)
		set(OIS_PLAT 
			src/win32/Win32ForceFeedback.cpp
			src/win32/Win32InputManager.cpp
			src/win32/Win32JoyStick.cpp
			src/win32/Win32KeyBoard.cpp
			src/win32/Win32Mouse.cpp
			)
		link_libraries(${DirectX_DXGUID_LIBRARY} ${DirectX_DINPUT8_LIBRARY})
	else (DirectX_FOUND)
		add_definitions(-DOIS_WIN32_NATIVE)
		set(OIS_PLAT 
			src/win32native/Win32NativeInputManager.cpp
			src/win32native/Win32NativeKeyboard.cpp
			src/win32native/Win32NativeJoystick.cpp
			src/win32native/Win32NativeMouse.cpp
			)
    endif (DirectX_FOUND)

endif()


# Cannot test this
if (APPLE) 
    set(OIS_PLAT 
        src/mac/MacHIDManager.cpp
        src/mac/MacHelpers.cpp
        src/mac/MacInputManager.cpp
        src/mac/MacKeyboard.cpp
        src/mac/MacMouse.cpp
    )
endif()


if (UNIX) 
    set(OIS_PLAT 
        src/linux/EventHelpers.cpp
        src/linux/LinuxForceFeedback.cpp
        src/linux/LinuxInputManager.cpp
        src/linux/LinuxJoyStickEvents.cpp
        src/linux/LinuxKeyboard.cpp
        src/linux/LinuxMouse.cpp
    )
endif()

include_directories(${OGREKIT_OIS_INCLUDE} )
add_library(OIS ${OGREKIT_LIB_TYPE} ${OIS_SRC} ${OIS_HDR} ${OIS_PLAT})
