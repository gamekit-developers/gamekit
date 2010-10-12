# ----------------------------------------------------------------------------#
#                                                                             #
#  Build settings for V8                                                      #
#   http://code.google.com/p/v8/                                              #
#                                                                             #
# ----------------------------------------------------------------------------#
set(V8_MODE                 "release"   CACHE STRING    "mode (release, debug), default release")
set(V8_LIBRARY              "static"    CACHE STRING    "library (static, shared), default static")
set(V8_REGEXP               "native"    CACHE STRING    "regexp (native, interpreted), default native")
set(V8_DISASSEMBLER         OFF         CACHE BOOL      "disassembler (on, off), default off")
set(V8_UNALIGNEDACCESSES    OFF         CACHE BOOL      "unalignedaccesses (default, on, off), default off")
set(V8_PROTECTHEAP          OFF         CACHE BOOL      "protectheap (on, off), default off")
set(V8_DEBUGGERSUPPORT      ON          CACHE BOOL      "debuggersupport (on, off), default on")
set(V8_VMSTATE              OFF         CACHE BOOL      "vmstate (on, off), default off")
set(V8_PROFILINGSUPPORT     ON          CACHE BOOL      "profilingsupport (on, off), default on")
if (WIN32)
	set(V8_OS               "win32"     CACHE STRING    "os (freebsd, linux, macos, win32, android, openbsd, solaris)")
else()
	if (APPLE)
		set(V8_OS           "macos"     CACHE STRING    "os (freebsd, linux, macos, win32, android, openbsd, solaris)")
	else ()
		set(V8_OS           "linux"     CACHE STRING    "os (freebsd, linux, macos, win32, android, openbsd, solaris)")
	endif()
endif()

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	set(V8_ARCH "x64" CACHE STRING "arch (arm, ia32, x64, mips), default x64")
else()
	set(V8_ARCH "ia32" CACHE STRING "arch (arm, ia32, x64, mips), default ia32")
endif()


set(V8_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependencies/Source/V8/include)
set(V8_LIBRARY V8)