@echo off

set MAINFOLDER=%CD%
set CYGWIN=nodosfilewarning

    
if "%1" == "" (
	echo "Enter build target directory."
	@exit /b 1
)

if "%NACL_SDK_ROOT%" == "" (
	echo "NACL_SDK_ROOT Path is empty."
	@exit /b 1
)

if "%LIBPATH%" == "" (
	echo "Warning: vc++ environment is missing."
)

if not exist %1 (
	mkdir %1
)

if not exist "%1\x86" (
	mkdir "%1\x86"
)

if not exist "%1\x64" (
	mkdir "%1\x64"
)

echo Source direcotry:  %MAINFOLDER%  Binery directory: %1

cd "%1\x86"

if "%2" == "debug" (	
	cmake %MAINFOLDER% -G "NMake Makefiles" -DGAMEKIT_GRAPHIC_BACKEND=1 -DCMAKE_TOOLCHAIN_FILE="%MAINFOLDER%/CMake/toolchain/nacl.toolchain.cmake" -DOGRE_BUILD_PLATFORM_NACL=1 -DOGREKIT_BUILD_NACL=1 -DOGREKIT_MINIMAL_FREEIMAGE_CODEC=1 -DLIBRARY_OUTPUT_PATH= -DCMAKE_BUILD_TYPE="Debug" --debug-trycompile
	cmake %MAINFOLDER% -G "NMake Makefiles" -DGAMEKIT_GRAPHIC_BACKEND=1 -DCMAKE_TOOLCHAIN_FILE="%MAINFOLDER%/CMake/toolchain/nacl.toolchain.cmake" -DOGRE_BUILD_PLATFORM_NACL=1 -DOGREKIT_BUILD_NACL=1 -DOGREKIT_MINIMAL_FREEIMAGE_CODEC=1 -DLIBRARY_OUTPUT_PATH= -DCMAKE_BUILD_TYPE="Debug" --debug-trycompile
) else (                                                                                                                                     
set command=%MAINFOLDER% -G "NMake Makefiles" -DGAMEKIT_GRAPHIC_BACKEND=1 -DCMAKE_TOOLCHAIN_FILE="%MAINFOLDER%/CMake/toolchain/nacl.toolchain.cmake" -DOGRE_BUILD_PLATFORM_NACL=1 -DOGRE_CONFIG_ALLOCATOR=1 -DOGREKIT_BUILD_NACL=1 -DOGREKIT_MINIMAL_FREEIMAGE_CODEC=1 -DLIBRARY_OUTPUT_PATH=  -DCMAKE_BUILD_TYPE="Release" -DOGRE_UNITY_BUILD=1 -DOGREKIT_UNITY_BUILD=1 
	rem cmake %MAINFOLDER% -G "NMake Makefiles" -DGAMEKIT_GRAPHIC_BACKEND=1 -DCMAKE_TOOLCHAIN_FILE="%MAINFOLDER%/CMake/toolchain/nacl.toolchain.cmake" -DOGRE_BUILD_PLATFORM_NACL=1 -DOGREKIT_BUILD_NACL=1 -DOGREKIT_MINIMAL_FREEIMAGE_CODEC=1 -DLIBRARY_OUTPUT_PATH=  -DCMAKE_BUILD_TYPE="Release"
cmake %command% -DOGREKIT_BUILD_ARCH_X64=0
cmake %command% -DOGREKIT_BUILD_ARCH_X64=0
)

cd "..\x64"
cmake %command% -DOGREKIT_BUILD_ARCH_X64=1
cmake %command% -DOGREKIT_BUILD_ARCH_X64=1
rem cmake %MAINFOLDER% -G "NMake Makefiles" -DGAMEKIT_GRAPHIC_BACKEND=1 -DCMAKE_TOOLCHAIN_FILE="%MAINFOLDER%/CMake/toolchain/nacl.toolchain.cmake" -DOGRE_BUILD_PLATFORM_NACL=1 -DOGRE_CONFIG_ALLOCATOR=1 -DOGREKIT_BUILD_NACL=1 -DOGREKIT_MINIMAL_FREEIMAGE_CODEC=1 -DLIBRARY_OUTPUT_PATH=  -DCMAKE_BUILD_TYPE="Release" -DOGRE_UNITY_BUILD=1 -DOGREKIT_UNITY_BUILD=1 -DOGREKIT_BUILD_ARCH_X64=1

cd %MAINFOLDER%

