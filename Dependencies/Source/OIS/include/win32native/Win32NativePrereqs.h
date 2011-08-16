/*
The zlib/libpng License

Copyright (c) 2005-2007 Phillip Castaneda (pjcast -- www.wreckedgames.com)

This software is provided 'as-is', without any express or implied warranty. In no event will
the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial 
applications, and to alter it and redistribute it freely, subject to the following
restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that 
		you wrote the original software. If you use this software in a product, 
		an acknowledgment in the product documentation would be appreciated but is 
		not required.

    2. Altered source versions must be plainly marked as such, and must not be 
		misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/

/*  Win32 native support added for the Ogre GameKit port by Charlie C.
	http://gamekit.googlecode.com/
*/
#ifndef _WIN32_NATIVE_PREREQS_H_
#define _WIN32_NATIVE_PREREQS_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct WNMouseEvent
{
	int type;			// 0,1,2,3 (motion, wheel, bup, bdn)
	int button;
	int z;
	HWND hWnd;
};


struct WNKeyEvent
{
	int type;			// 0,1 (press, release)
	int scancode;
	unsigned int charcode;
	int mod;
};

#define WNClamp(x, mi, ma) ((x) <  (mi) ?  (mi)  : (x) > (ma) ? (ma) : (x))
#define WNAddBit(x, b) ((x) |=  (1 << (b)))
#define WNClrBit(x, b) ((x) &= ~(1 << (b)))


#define VK_KEY_0 '0'
#define VK_KEY_1 '1'
#define VK_KEY_2 '2'
#define VK_KEY_3 '3'
#define VK_KEY_4 '4'
#define VK_KEY_5 '5'
#define VK_KEY_6 '6'
#define VK_KEY_7 '7'
#define VK_KEY_8 '8'
#define VK_KEY_9 '9'
#define VK_KEY_A 'A'
#define VK_KEY_B 'B'
#define VK_KEY_C 'C'
#define VK_KEY_D 'D'
#define VK_KEY_E 'E'
#define VK_KEY_F 'F'
#define VK_KEY_G 'G'
#define VK_KEY_H 'H'
#define VK_KEY_I 'I'
#define VK_KEY_J 'J'
#define VK_KEY_K 'K'
#define VK_KEY_L 'L'
#define VK_KEY_M 'M'
#define VK_KEY_N 'N'
#define VK_KEY_O 'O'
#define VK_KEY_P 'P'
#define VK_KEY_Q 'Q'
#define VK_KEY_R 'R'
#define VK_KEY_S 'S'
#define VK_KEY_T 'T'
#define VK_KEY_U 'U'
#define VK_KEY_V 'V'
#define VK_KEY_W 'W'
#define VK_KEY_X 'X'
#define VK_KEY_Y 'Y'
#define VK_KEY_Z 'Z'

#ifndef WM_MOUSEWHEEL
# define WM_MOUSEWHEEL 0x020A
#endif

#endif//_WIN32_NATIVE_PREREQS_H_
