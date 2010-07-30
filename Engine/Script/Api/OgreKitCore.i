/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-------------------------------------------------------------------------------
-- Include file for Core 
*/
%{
#include "OgreKitCore.h"
%}
%rename (ScenePtr)      Pointer<Scene>;
%rename (GameObjectPtr) Pointer<GameObject>;
%rename (EntityPtr)     Pointer<Entity>;
%rename (CameraPtr)     Pointer<Camera>;
%rename (LightPtr)      Pointer<Light>;

%include "OgreKitCore.h"




typedef enum ScanCode
{
    KC_NONE = 0,
    KC_AKEY,
    KC_BKEY,
    KC_CKEY,
    KC_DKEY,
    KC_EKEY,
    KC_FKEY,
    KC_GKEY,
    KC_HKEY,
    KC_IKEY,
    KC_JKEY,
    KC_KKEY,
    KC_LKEY,
    KC_MKEY,
    KC_NKEY,
    KC_OKEY,
    KC_PKEY,
    KC_QKEY,
    KC_RKEY,
    KC_SKEY,
    KC_TKEY,
    KC_UKEY,
    KC_VKEY,
    KC_WKEY,
    KC_XKEY,
    KC_YKEY,
    KC_ZKEY,
    KC_ZEROKEY,
    KC_ONEKEY,
    KC_TWOKEY,
    KC_THREEKEY,
    KC_FOURKEY,
    KC_FIVEKEY,
    KC_SIXKEY,
    KC_SEVENKEY,
    KC_EIGHTKEY,
    KC_NINEKEY,
    KC_CAPSLOCKKEY,
    KC_LEFTCTRLKEY,
    KC_LEFTALTKEY,
    KC_RIGHTALTKEY,
    KC_RIGHTCTRLKEY,
    KC_RIGHTSHIFTKEY,
    KC_LEFTSHIFTKEY,
    KC_ESCKEY,
    KC_TABKEY,
    KC_RETKEY,
    KC_SPACEKEY,
    KC_LINEFEEDKEY,
    KC_BACKSPACEKEY,
    KC_DELKEY,
    KC_SEMICOLONKEY,
    KC_PERIODKEY,
    KC_COMMAKEY,
    KC_QUOTEKEY,
    KC_ACCENTGRAVEKEY,
    KC_MINUSKEY,
    KC_SLASHKEY,
    KC_BACKSLASHKEY,
    KC_EQUALKEY,
    KC_LEFTBRACKETKEY,
    KC_RIGHTBRACKETKEY,
    KC_LEFTARROWKEY,
    KC_DOWNARROWKEY,
    KC_RIGHTARROWKEY,
    KC_UPARROWKEY,
    KC_PAD0,
    KC_PAD1,
    KC_PAD2,
    KC_PAD3,
    KC_PAD4,
    KC_PAD5,
    KC_PAD6,
    KC_PAD7,
    KC_PAD8,
    KC_PAD9,
    KC_PADEQUALS,
    KC_PADPERIOD,
    KC_PADSLASHKEY,
    KC_PADASTERKEY,
    KC_PADMINUS,
    KC_PADENTER,
    KC_PADPLUSKEY,
    KC_F1KEY,
    KC_F2KEY,
    KC_F3KEY,
    KC_F4KEY,
    KC_F5KEY,
    KC_F6KEY,
    KC_F7KEY,
    KC_F8KEY,
    KC_F9KEY,
    KC_F10KEY,
    KC_F11KEY,
    KC_F12KEY,
    KC_PAUSEKEY,
    KC_INSERTKEY,
    KC_HOMEKEY,
    KC_PAGEUPKEY,
    KC_PAGEDOWNKEY,
    KC_ENDKEY,
    KC_UNKNOWNKEY,
    KC_COMMANDKEY,
    KC_GRLESSKEY,
    KC_MAX
};
