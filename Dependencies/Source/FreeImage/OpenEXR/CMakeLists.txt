

SET(fi_half_SRCS Half/half.cpp)
FILE(GLOB fi_half_HDRS "Half/*.h")

FILE(GLOB fi_Iex_SRCS "Iex/*.cpp")
FILE(GLOB fi_Iex_HDRS "Iex/*.h")

FILE(GLOB fi_IlmImf_SRCS "IlmImf/*.cpp")
FILE(GLOB fi_IlmImf_HDRS "IlmImf/*.h")



SET(fi_IlmThread_SRCS
        IlmThread/IlmThread.cpp
        IlmThread/IlmThreadMutex.cpp
        IlmThread/IlmThreadPool.cpp
        IlmThread/IlmThreadSemaphore.cpp
        )

IF (WIN32)
    SET(fi_IlmThread_SRCS_PLAT
            IlmThread/IlmThreadMutexWin32.cpp
            IlmThread/IlmThreadSemaphoreWin32.cpp
            IlmThread/IlmThreadWin32.cpp
            )
ELSE ()            
    SET(fi_IlmThread_SRCS_PLAT
            IlmThread/IlmThreadMutexPosix.cpp
            IlmThread/IlmThreadSemaphorePosix.cpp
            IlmThread/IlmThreadSemaphorePosixCompat.cpp
            )
ENDIF ()


FILE(GLOB fi_IlmThread_HDRS "IlmThread/*.h")

FILE(GLOB fi_Imath_SRCS "Imath/*.cpp")
FILE(GLOB fi_Imath_HDRS "Imath/*.h")


INCLUDE_DIRECTORIES(${FI_INCLUDE} )

ADD_DEFINITIONS(-DFREEIMAGE_LIB)

ADD_LIBRARY(OpenEXR ${fi_half_SRCS} 
                    ${fi_half_HDRS}
                    ${fi_Iex_SRCS}
                    ${fi_Iex_HDRS}
                    ${fi_IlmImf_SRCS}
                    ${fi_IlmImf_HDRS}
                    ${fi_Imath_SRCS}
                    ${fi_Imath_HDRS}
                    ${fi_IlmThread_SRCS}
                    ${fi_IlmThread_HDRS}
                    ${fi_IlmThread_SRCS_PLAT}
                    )
