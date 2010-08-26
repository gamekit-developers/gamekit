# Adding options only to speed up build/dev time
if (SAMPLES_RUNTIME)
    subdirs(Runtime)
endif()

if (SAMPLES_LOGICDEMO)
    subdirs(LogicDemo)
endif()

if (SAMPLES_VEHICLEDEMO)
    subdirs(VehicleDemo)
endif()


if (SAMPLES_LUARUNTIME)
    subdirs(LuaRuntime)
endif()
