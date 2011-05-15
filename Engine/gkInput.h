/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): xat.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
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
#ifndef _gkInput_h_
#define _gkInput_h_

#include "gkCommon.h"
#include "gkMathUtils.h"


typedef enum gkScanCode
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
} gkScanCode;



///Current state of an input device
enum gkInputState
{
	GK_NullState = 0,
	GK_Pressed,
	GK_Released,
};


class gkKeyboard
{
public:
	typedef int KeyState[KC_MAX];
public:

	gkKeyboard()
		:       key_count(0), text(0), key_mod(0)
	{
		memset(keys, GK_NullState, sizeof(KeyState));
	}

	GK_INLINE bool isKeyDown(gkScanCode key) const
	{
		if (key > KC_NONE && key < KC_MAX)
			return (keys[(int)key] == GK_Pressed);
		return false;
	}

	void clear(void)
	{
		key_count = 0;
		memset(keys, GK_NullState, sizeof(KeyState));
	}

	enum KEY_MODIFIER
	{
		KM_SHIFT = 1 << 0,
		KM_CTRL  = 1 << 1,
		KM_ALT   = 1 << 2
	};

	int          key_count;  // currently pressed count
	KeyState     keys;       // key buffer state
	unsigned int text;       // last input text character
	int          key_mod;    // key modifier state
};



class gkMouse
{
public:
	enum Buttons
	{
		Left = 0,
		Right,
		Middle,
	};

	typedef int ButtonState[3];


	gkVector2 getPosition(void) const {return position;}
	gkVector2 getRelative(void) const {return relative;}

	GK_INLINE bool isButtonDown(int button) const
	{
		if (button >= 0 && button < 3)
			return (buttons[button] == GK_Pressed);
		return false;
	}

	GK_INLINE bool mouseMoved(void) const { return moved; }

	void clear(void)
	{
		moved = false;
		wheelDelta = 0.f;
		relative.x = 0.f;
		relative.y = 0.f;
		position.x = 0.f;
		position.y = 0.f;
		buttons[0] = GK_NullState;
		buttons[1] = GK_NullState;
		buttons[2] = GK_NullState;
	}

public:

	gkMouse()
		:       position(0, 0),
		        relative(0, 0),
		        winsize(0, 0),
		        moved(false),
		        wheelDelta(0.0)
	{
		buttons[0] = GK_NullState;
		buttons[1] = GK_NullState;
		buttons[2] = GK_NullState;
	}

	gkVector2       position;   // absolite position
	gkVector2       relative;   // relative position
	gkVector2       winsize;    // main window size
	ButtonState     buttons;    // button pressed state
	bool            moved;      // mouse moved
	gkScalar        wheelDelta; // current wheel
};



class gkJoystick
{
public:

	typedef utArray<int> ButtonStates;
	typedef utArray<int> AxisStates;
	
	GK_INLINE UTsize getButtonsNumber() {return buttons.size();}
	GK_INLINE UTsize getAxesNumber()    {return axes.size();}

	GK_INLINE bool isButtonDown(int button) const
	{
		if (button >= 0 && button < (int)buttons.size())
			return (buttons[button] == GK_Pressed);
		return false;
	}
	
	GK_INLINE bool wasButtonPressed(int button) const
	{
		if (button >= 0 && button < (int)buttons.size())
			return (buttonsPressed[button] == GK_Pressed);
		return false;
	}

	GK_INLINE int getAxisValue(int axis) const
	{
		if (axis >= 0 && axis < (int)axes.size())
			return axes[axis];
		return 0;
	}

	GK_INLINE int getRelAxisValue(int axis) const
	{
		if (axis >= 0 && axis < (int)relAxes.size())
			return relAxes[axis];
		return 0;
	}
	
	void clear(void)
	{
		buttonCount = 0;
		UTsize i;
		for (i = 0; i < buttons.size(); ++i)
		{
			buttons[i] = GK_NullState;
			buttonsPressed[i] = GK_NullState;
		}

		for (i = 0; i < axes.size(); ++i)
			axes[i] = relAxes[i] = 0;

		accel = gkVector3(0.f, 0.f, 0.f);
	}

public:

	gkJoystick(unsigned int nbuttons, unsigned int naxis)
		:        buttonCount(0), accel(0.f, 0.f, 0.f)
	{
		buttons.resize(nbuttons, GK_NullState);
		axes.resize(naxis, 0);

		buttonsPressed.resize(nbuttons, GK_NullState);
		relAxes.resize(naxis, 0);
	}

	int             buttonCount; // currently pressed count
	ButtonStates    buttons;     // button pressed state
	AxisStates      axes;

	// for multi-touch
	ButtonStates	buttonsPressed;
	AxisStates      relAxes;

	// for accelerometer
	gkVector3		accel;
};

class gkTouch
{
public:
	gkTouch()
	{
	}
};

#endif//_gkInput_h_
