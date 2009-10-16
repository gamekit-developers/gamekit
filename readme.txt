
GameKit is a prototyping environment that includes 

+ Bullet Physics Library for physics
+ Irrlicht 1.6 for Graphics
+ readblend to read 32bit little-endian .blend files
+ some example .blend files included in the GameKit folder

It has been tested on Windows, Mac OS X and Linux.

Some notes:

Use Cmake (http://cmake.org) to create a build system, for example:

cmake . -G "Visual Studio 9 2008"
cmake . -G Xcode
cmake . -G "Unix Makefiles"


By default, OpenGL is enabled. 

To switch to Direct3D8 or Direct3D9 edit the following files:

gamekit\irrlicht-1.5\include\IrrCompileConfig.h around line 89

//#define _IRR_COMPILE_WITH_DIRECT3D_8_
#define _IRR_COMPILE_WITH_DIRECT3D_9_


gamekit\GameKit\main.cpp around line 644:
//driverType = video::EDT_OPENGL;
//driverType = video::EDT_DIRECT3D8;
	driverType = video::EDT_DIRECT3D9;
