setup_wx()

if (WIN32)
    add_executable(FontGen WIN32 FontGen.cpp)
else()
    add_executable(FontGen FontGen.cpp)
endif()
