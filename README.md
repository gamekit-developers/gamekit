gamekit
=======

The goal of gamekit is to create a basic game engine that allows fast prototyping build around open source software free for commercial use.

* Using Ogre or Irrlicht for graphics, Bullet for physics, OpenAL for sound
* OgreKit is most actively developed and in svn/trunk, the suspended Irrlicht version is in svn/branches/IrrKit.
* Engine is written in C++ and the game logic can be done in C++, Lua scripting or logic bricks
* Reads all data from Blender .blend files, with future FBX import planned
* Free from viral licenses: only using components using MIT/BSD/Zlib style licenses
* CMake cross-platform build system support that works out-of-the-box, see http://cmake.org
* Gamekit supports Windows and Mac OSX, Linux, Android and iPhone.

Building
========

Use cmake on Windows, Linux and MacOSX should build without further configuration,
all dependencies are included in the source tree.

It is easiest to use ''cmake-gui'' to configure all settings and create the build files.
Alternatively you can use the commandline cmake tool,
for example for MacOSX creating GNU Makefiles using CMake:

```
mkdir gmake
cd gmake
cmake ..
make
```
Here are a examples to generate other build / project files, just run cmake without arguments to see other options:

```
cmake .. -G Xcode
cmake .. -G "Visual Studio 12"
cmake .. -G "Ninja"
cmake .. -G "CodeBlocks - Unix Makefiles"
```

Further information
===================

See:
* http://gamekit.org 
* http://gamekit.googlecode.com
* https://github.com/gamekit-developers/gamekit/wiki
