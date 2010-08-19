# Adding options only to speed up build/dev time
if (OGREKIT_BUILD_RUNTIME)
    subdirs(Runtime)
endif()

if (OGREKIT_BUILD_LOGICDEMO)
    subdirs(LogicDemo)
endif()


if (OGREKIT_BUILD_LUARUNTIME)
    subdirs(LuaRuntime)
    #subdirs(LuaPackage)
endif()


# Code cleanup tests.
subdirs(PlayPen)
