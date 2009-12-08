
GameKit is a prototyping environment that includes
http://gamekit.googlecode.com

+ Bullet Physics Library for physics
+ Ogre 1.7 or Irrlicht 1.6 for Graphics
+ bParse to read 32bit/64bit little-endian and big endian .blend files
+ some example .blend files included in the GameKit folder

On Apple, you can copy a user file called game.blend next to the GameKit.app bundle, to override the default .blend file.

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
