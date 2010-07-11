SET(SOURCE
    src/accessors.cc
    src/allocation.cc
    src/api.cc
    src/assembler.cc
    src/ast.cc
    src/bootstrapper.cc
    src/builtins.cc
    src/checks.cc
    src/circular-queue.cc
    src/code-stubs.cc
    src/codegen.cc
    src/compilation-cache.cc
    src/compiler.cc
    src/contexts.cc
    src/conversions.cc
    src/counters.cc
    src/cpu-profiler.cc
    src/data-flow.cc
    src/dateparser.cc
    src/debug-agent.cc
    src/debug.cc
    src/disassembler.cc
    src/diy-fp.cc
    src/dtoa.cc
    src/execution.cc
    src/factory.cc
    src/flags.cc
    src/flow-graph.cc
    src/frame-element.cc
    src/frames.cc
    src/full-codegen.cc
    src/func-name-inferrer.cc
    src/global-handles.cc
    src/fast-dtoa.cc
    src/fixed-dtoa.cc
    src/handles.cc
    src/hashmap.cc
    src/heap-profiler.cc
    src/heap.cc
    src/ic.cc
    src/interpreter-irregexp.cc
    src/jsregexp.cc
    src/jump-target.cc
    src/liveedit.cc
    src/log-utils.cc
    src/log.cc
    src/mark-compact.cc
    src/messages.cc
    src/objects.cc
    src/oprofile-agent.cc
    src/parser.cc
    src/profile-generator.cc
    src/property.cc
    src/regexp-macro-assembler-irregexp.cc
    src/regexp-macro-assembler.cc
    src/regexp-stack.cc
    src/register-allocator.cc
    src/rewriter.cc
    src/runtime.cc
    src/scanner.cc
    src/scopeinfo.cc
    src/scopes.cc
    src/serialize.cc
    src/snapshot-common.cc
    src/spaces.cc
    src/string-stream.cc
    src/stub-cache.cc
    src/token.cc
    src/top.cc
    src/type-info.cc
    src/unicode.cc
    src/utils.cc
    src/v8-counters.cc
    src/v8.cc
    src/v8threads.cc
    src/variables.cc
    src/version.cc
    src/virtual-frame.cc
    src/vm-state.cc
    src/zone.cc
)


set(LIBRARY
    src/runtime.js
    src/v8natives.js
    src/array.js
    src/string.js
    src/uri.js
    src/math.js
    src/messages.js
    src/apinatives.js
    src/date.js
    src/regexp.js
    src/json.js
    src/liveedit-debugger.js
    src/mirror-debugger.js
    src/debug-debugger.js
)

set(ARCH_ARM
    src/fast-codegen.cc
    src/jump-target-light.cc
    src/virtual-frame-light.cc
    src/arm/builtins-arm.cc
    src/arm/codegen-arm.cc
    src/arm/constants-arm.cc
    src/arm/cpu-arm.cc
    src/arm/debug-arm.cc
    src/arm/disasm-arm.cc
    src/arm/fast-codegen-arm.cc
    src/arm/frames-arm.cc
    src/arm/full-codegen-arm.cc
    src/arm/ic-arm.cc
    src/arm/jump-target-arm.cc
    src/arm/macro-assembler-arm.cc
    src/arm/regexp-macro-assembler-arm.cc
    src/arm/register-allocator-arm.cc
    src/arm/stub-cache-arm.cc
    src/arm/virtual-frame-arm.cc
    src/arm/assembler-arm.cc
)


set(ARCH_MIPS
    src/fast-codegen.cc
    src/mips/assembler-mips.cc
    src/mips/builtins-mips.cc
    src/mips/codegen-mips.cc
    src/mips/constants-mips.cc
    src/mips/cpu-mips.cc
    src/mips/debug-mips.cc
    src/mips/disasm-mips.cc
    src/mips/fast-codegen-mips.cc
    src/mips/full-codegen-mips.cc
    src/mips/frames-mips.cc
    src/mips/ic-mips.cc
    src/mips/jump-target-mips.cc
    src/mips/macro-assembler-mips.cc
    src/mips/register-allocator-mips.cc
    src/mips/stub-cache-mips.cc
    src/mips/virtual-frame-mips.cc
)
set(ARCH_IA32
    src/jump-target-heavy.cc
    src/virtual-frame-heavy.cc
    src/ia32/assembler-ia32.cc
    src/ia32/builtins-ia32.cc
    src/ia32/codegen-ia32.cc
    src/ia32/cpu-ia32.cc
    src/ia32/debug-ia32.cc
    src/ia32/disasm-ia32.cc
    src/ia32/fast-codegen-ia32.cc
    src/ia32/frames-ia32.cc
    src/ia32/full-codegen-ia32.cc
    src/ia32/ic-ia32.cc
    src/ia32/jump-target-ia32.cc
    src/ia32/macro-assembler-ia32.cc
    src/ia32/regexp-macro-assembler-ia32.cc
    src/ia32/register-allocator-ia32.cc
    src/ia32/stub-cache-ia32.cc
    src/ia32/virtual-frame-ia32.cc
)


set(ARCH_X64
    src/fast-codegen.cc
    src/jump-target-heavy.cc
    src/virtual-frame-heavy.cc
    src/x64/assembler-x64.cc
    src/x64/builtins-x64.cc
    src/x64/codegen-x64.cc
    src/x64/cpu-x64.cc
    src/x64/debug-x64.cc
    src/x64/disasm-x64.cc
    src/x64/fast-codegen-x64.cc
    src/x64/frames-x64.cc
    src/x64/full-codegen-x64.cc
    src/x64/ic-x64.cc
    src/x64/jump-target-x64.cc
    src/x64/macro-assembler-x64.cc
    src/x64/regexp-macro-assembler-x64.cc
    src/x64/register-allocator-x64.cc
    src/x64/stub-cache-x64.cc
    src/x64/virtual-frame-x64.cc
)

set(GEN 
    #generated/libraries-empty.cc
    generated/libraries.cc
)

set(DTOA
    src/dtoa-config.c
)

set(SNAPSHOT
    src/snapshot-empty.cc
)

source_group(generated      FILES ${GEN})
source_group(library        FILES ${LIBRARY})
# -------------------------------------------------------------------
set(ALL_SOURCE
    ${SOURCE}
    ${LIBRARY}
    ${GEN}
    ${DTOA}
    ${SNAPSHOT}
)



# -------------------------------------------------------------------
# Setup 
# -------------------------------------------------------------------
if (V8_MODE STREQUAL "debug")
    add_definitions(-DV8_ENABLE_CHECKS -DENABLE_DISASSEMBLER -D_DEBUG -DDEBUG)

    list(APPEND ALL_SOURCE src/objects-debug.cc src/prettyprinter.cc src/regexp-macro-assembler-tracer.cc)
endif()
if (V8_VMSTATE)
    add_definitions(-DENABLE_VMSTATE_TRACKING)
endif()
if (V8_PROTECTHEAP)
    add_definitions(-DENABLE_VMSTATE_TRACKING -DENABLE_HEAP_PROTECTION)
endif()
if (V8_PROFILINGSUPPORT)
    add_definitions(-DENABLE_VMSTATE_TRACKING -DENABLE_LOGGING_AND_PROFILING)
endif()
if (V8_DEBUGGERSUPPORT)
    add_definitions(-DENABLE_DEBUGGER_SUPPORT)
endif()
if (V8_REGEXP STREQUAL "interpreted")
    add_definitions(-DV8_INTERPRETED_REGEXP)
endif()
if (V8_LIBRARY STREQUAL "shared")
    add_definitions(-DBUILDING_V8_SHARED)
    set(LIB_BUILD_TYPE SHARED)
endif()


if (V8_ARCH STREQUAL "ia32")
    add_definitions(-DV8_TARGET_ARCH_IA32 -D_USE_32BIT_TIME_T)
    source_group(ia32 FILES ${ARCH_IA32})
    list(APPEND ALL_SOURCE ${ARCH_IA32})
endif()


if (V8_ARCH STREQUAL "arm")
    add_definitions(-DV8_TARGET_ARCH_ARM)
    source_group(arm FILES ${ARCH_ARM})
    list(APPEND ALL_SOURCE ${ARCH_ARM})
    
    if (V8_UNALIGNEDACCESSES)
        set(-DCAN_USE_UNALIGNED_ACCESSES=1)
    else()
        set(-DCAN_USE_UNALIGNED_ACCESSES=0)
    endif()
    
    if (WIN32)
        list(APPEND ALL_SOURCE src/arm/simulator-arm.cc)
    endif()
    
endif()

if (V8_ARCH STREQUAL "mips")
    add_definitions(-DV8_TARGET_ARCH_MIPS)
    source_group(arm FILES ${ARCH_MIPS})
    list(APPEND ALL_SOURCE ${ARCH_MIPS})
endif()


if (V8_ARCH STREQUAL "x64")
    add_definitions(-DV8_TARGET_ARCH_X64)
    source_group(x64 FILES ${ARCH_X64})
    list(APPEND ALL_SOURCE ${ARCH_X64})
endif()

if (V8_OS STREQUAL "freebsd")
    list(APPEND ALL_SOURCE src/platform-freebsd.cc src/platform-posix.cc)
endif()

if (V8_OS STREQUAL "openbsd")
    list(APPEND ALL_SOURCE src/platform-openbsd.cc src/platform-posix.cc)
endif()

if (V8_OS STREQUAL "linux")
    list(APPEND ALL_SOURCE src/platform-linux.cc src/platform-posix.cc)
endif()

if (V8_OS STREQUAL "macos")
    list(APPEND ALL_SOURCE src/platform-macos.cc src/platform-posix.cc)
endif()
if (V8_OS STREQUAL "solaris")
    list(APPEND ALL_SOURCE src/platform-solaris.cc src/platform-posix.cc)
endif()


if (V8_OS STREQUAL "win32")
    list(APPEND ALL_SOURCE src/platform-win32.cc)
endif()

if (V8_OS STREQUAL "nullos")
    list(APPEND ALL_SOURCE src/platform-nullos.cc)
endif()
    


if (WIN32)
    add_definitions(-DWIN32)
    link_libraries(winmm ws2_32)
endif()

if (MSVC)
    add_definitions(
        /WX 
        # V8
        /wd4355 /wd4800 /IGNORE:4221 
        # DTOA
        /wd4018 /wd4244
        # ARM
        /wd4146
        /GR-
        )
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-fno-rtti -fno-exceptions)
endif()

include_directories(src)
add_library(V8 ${LIB_BUILD_TYPE} ${ALL_SOURCE})
