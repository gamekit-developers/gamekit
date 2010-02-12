/*
-------------------------------------------------------------------------------
	Lua 5.1 script tests.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would be
	 appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
	 misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "LuaEngine.h"
#include "LuaBindingUtils.h"
#include "gkKeyCodes.h"

#define ModuleName "KeyCodes"


void LuaKeyCodes_initialize(LuaState *L)
{
	int tval= LuaBind::beginBinding(L);
	LuaBind::table(L, ModuleName);
	LuaBind::set(L, "KC_AKEY", (int)KC_AKEY);
	LuaBind::set(L, "KC_BKEY", (int)KC_BKEY);
	LuaBind::set(L, "KC_CKEY", (int)KC_CKEY);
	LuaBind::set(L, "KC_DKEY", (int)KC_DKEY);
	LuaBind::set(L, "KC_EKEY", (int)KC_EKEY);
	LuaBind::set(L, "KC_FKEY", (int)KC_FKEY);
	LuaBind::set(L, "KC_GKEY", (int)KC_GKEY);
	LuaBind::set(L, "KC_HKEY", (int)KC_HKEY);
	LuaBind::set(L, "KC_IKEY", (int)KC_IKEY);
	LuaBind::set(L, "KC_JKEY", (int)KC_JKEY);
	LuaBind::set(L, "KC_KKEY", (int)KC_KKEY);
	LuaBind::set(L, "KC_LKEY", (int)KC_LKEY);
	LuaBind::set(L, "KC_MKEY", (int)KC_MKEY);
	LuaBind::set(L, "KC_NKEY", (int)KC_NKEY);
	LuaBind::set(L, "KC_OKEY", (int)KC_OKEY);
	LuaBind::set(L, "KC_PKEY", (int)KC_PKEY);
	LuaBind::set(L, "KC_QKEY", (int)KC_QKEY);
	LuaBind::set(L, "KC_RKEY", (int)KC_RKEY);
	LuaBind::set(L, "KC_SKEY", (int)KC_SKEY);
	LuaBind::set(L, "KC_TKEY", (int)KC_TKEY);
	LuaBind::set(L, "KC_UKEY", (int)KC_UKEY);
	LuaBind::set(L, "KC_VKEY", (int)KC_VKEY);
	LuaBind::set(L, "KC_WKEY", (int)KC_WKEY);
	LuaBind::set(L, "KC_XKEY", (int)KC_XKEY);
	LuaBind::set(L, "KC_YKEY", (int)KC_YKEY);
	LuaBind::set(L, "KC_ZKEY", (int)KC_ZKEY);
	LuaBind::set(L, "KC_ZEROKEY", (int)KC_ZEROKEY);
	LuaBind::set(L, "KC_ONEKEY", (int)KC_ONEKEY);
	LuaBind::set(L, "KC_TWOKEY", (int)KC_TWOKEY);
	LuaBind::set(L, "KC_THREEKEY", (int)KC_THREEKEY);
	LuaBind::set(L, "KC_FOURKEY", (int)KC_FOURKEY);
	LuaBind::set(L, "KC_FIVEKEY", (int)KC_FIVEKEY);
	LuaBind::set(L, "KC_SIXKEY", (int)KC_SIXKEY);
	LuaBind::set(L, "KC_SEVENKEY", (int)KC_SEVENKEY);
	LuaBind::set(L, "KC_EIGHTKEY", (int)KC_EIGHTKEY);
	LuaBind::set(L, "KC_NINEKEY", (int)KC_NINEKEY);
	LuaBind::set(L, "KC_CAPSLOCKKEY", (int)KC_CAPSLOCKKEY);
	LuaBind::set(L, "KC_LEFTCTRLKEY", (int)KC_LEFTCTRLKEY);
	LuaBind::set(L, "KC_LEFTALTKEY", (int)KC_LEFTALTKEY);
	LuaBind::set(L, "KC_RIGHTALTKEY", (int)KC_RIGHTALTKEY);
	LuaBind::set(L, "KC_RIGHTCTRLKEY", (int)KC_RIGHTCTRLKEY);
	LuaBind::set(L, "KC_RIGHTSHIFTKEY", (int)KC_RIGHTSHIFTKEY);
	LuaBind::set(L, "KC_LEFTSHIFTKEY", (int)KC_LEFTSHIFTKEY);
	LuaBind::set(L, "KC_ESCKEY", (int)KC_ESCKEY);
	LuaBind::set(L, "KC_TABKEY", (int)KC_TABKEY);
	LuaBind::set(L, "KC_RETKEY", (int)KC_RETKEY);
	LuaBind::set(L, "KC_SPACEKEY", (int)KC_SPACEKEY);
	LuaBind::set(L, "KC_LINEFEEDKEY", (int)KC_LINEFEEDKEY);
	LuaBind::set(L, "KC_BACKSPACEKEY", (int)KC_BACKSPACEKEY);
	LuaBind::set(L, "KC_DELKEY", (int)KC_DELKEY);
	LuaBind::set(L, "KC_SEMICOLONKEY", (int)KC_SEMICOLONKEY);
	LuaBind::set(L, "KC_PERIODKEY", (int)KC_PERIODKEY);
	LuaBind::set(L, "KC_COMMAKEY", (int)KC_COMMAKEY);
	LuaBind::set(L, "KC_QUOTEKEY", (int)KC_QUOTEKEY);
	LuaBind::set(L, "KC_ACCENTGRAVEKEY", (int)KC_ACCENTGRAVEKEY);
	LuaBind::set(L, "KC_MINUSKEY", (int)KC_MINUSKEY);
	LuaBind::set(L, "KC_SLASHKEY", (int)KC_SLASHKEY);
	LuaBind::set(L, "KC_BACKSLASHKEY", (int)KC_BACKSLASHKEY);
	LuaBind::set(L, "KC_EQUALKEY", (int)KC_EQUALKEY);
	LuaBind::set(L, "KC_LEFTBRACKETKEY", (int)KC_LEFTBRACKETKEY);
	LuaBind::set(L, "KC_RIGHTBRACKETKEY", (int)KC_RIGHTBRACKETKEY);
	LuaBind::set(L, "KC_LEFTARROWKEY", (int)KC_LEFTARROWKEY);
	LuaBind::set(L, "KC_DOWNARROWKEY", (int)KC_DOWNARROWKEY);
	LuaBind::set(L, "KC_RIGHTARROWKEY", (int)KC_RIGHTARROWKEY);
	LuaBind::set(L, "KC_UPARROWKEY", (int)KC_UPARROWKEY);
	LuaBind::set(L, "KC_PAD0", (int)KC_PAD0);
	LuaBind::set(L, "KC_PAD1", (int)KC_PAD1);
	LuaBind::set(L, "KC_PAD2", (int)KC_PAD2);
	LuaBind::set(L, "KC_PAD3", (int)KC_PAD3);
	LuaBind::set(L, "KC_PAD4", (int)KC_PAD4);
	LuaBind::set(L, "KC_PAD5", (int)KC_PAD5);
	LuaBind::set(L, "KC_PAD6", (int)KC_PAD6);
	LuaBind::set(L, "KC_PAD7", (int)KC_PAD7);
	LuaBind::set(L, "KC_PAD8", (int)KC_PAD8);
	LuaBind::set(L, "KC_PAD9", (int)KC_PAD9);
	LuaBind::set(L, "KC_PADEQUALS", (int)KC_PADEQUALS);
	LuaBind::set(L, "KC_PADPERIOD", (int)KC_PADPERIOD);
	LuaBind::set(L, "KC_PADSLASHKEY", (int)KC_PADSLASHKEY);
	LuaBind::set(L, "KC_PADASTERKEY", (int)KC_PADASTERKEY);
	LuaBind::set(L, "KC_PADMINUS", (int)KC_PADMINUS);
	LuaBind::set(L, "KC_PADENTER", (int)KC_PADENTER);
	LuaBind::set(L, "KC_PADPLUSKEY", (int)KC_PADPLUSKEY);
	LuaBind::set(L, "KC_F1KEY", (int)KC_F1KEY);
	LuaBind::set(L, "KC_F2KEY", (int)KC_F2KEY);
	LuaBind::set(L, "KC_F3KEY", (int)KC_F3KEY);
	LuaBind::set(L, "KC_F4KEY", (int)KC_F4KEY);
	LuaBind::set(L, "KC_F5KEY", (int)KC_F5KEY);
	LuaBind::set(L, "KC_F6KEY", (int)KC_F6KEY);
	LuaBind::set(L, "KC_F7KEY", (int)KC_F7KEY);
	LuaBind::set(L, "KC_F8KEY", (int)KC_F8KEY);
	LuaBind::set(L, "KC_F9KEY", (int)KC_F9KEY);
	LuaBind::set(L, "KC_F10KEY", (int)KC_F10KEY);
	LuaBind::set(L, "KC_F11KEY", (int)KC_F11KEY);
	LuaBind::set(L, "KC_F12KEY", (int)KC_F12KEY);
	LuaBind::set(L, "KC_PAUSEKEY", (int)KC_PAUSEKEY);
	LuaBind::set(L, "KC_INSERTKEY", (int)KC_INSERTKEY);
	LuaBind::set(L, "KC_HOMEKEY", (int)KC_HOMEKEY);
	LuaBind::set(L, "KC_PAGEUPKEY", (int)KC_PAGEUPKEY);
	LuaBind::set(L, "KC_PAGEDOWNKEY", (int)KC_PAGEDOWNKEY);
	LuaBind::set(L, "KC_ENDKEY", (int)KC_ENDKEY);
	LuaBind::set(L, "KC_UNKNOWNKEY", (int)KC_UNKNOWNKEY);
	LuaBind::set(L, "KC_COMMANDKEY", (int)KC_COMMANDKEY);
	LuaBind::set(L, "KC_GRLESSKEY", (int)KC_GRLESSKEY);

	LuaBind::table(L);
	LuaBind::endBinding(L, tval);
}
