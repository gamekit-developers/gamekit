# Adding options only to speed up build/dev time
if (OGREKIT_BUILD_RUNTIME)
    subdirs(Runtime)
endif()

if (OGREKIT_BUILD_LOGICDEMO)
    subdirs(LogicDemo)
endif()

if (OGREKIT_BUILD_V8_TEST)
    subdirs(V8Test)
endif()

#if (OGREKIT_BUILD_CHROMEPLUGIN)
#    subdirs(ChromePlugin)
#endif()