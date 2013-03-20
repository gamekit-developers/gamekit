/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 harkon.kr

    Contributor(s): none yet.
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

#include "OgreKit.h"

#include "RocketEventListener.h"
#include <Rocket/Core/Context.h>
#include <Rocket/Debugger.h>
#include "RocketRenderListener.h"


RocketEventListener::RocketEventListener(gkWindow* window, Rocket::Core::Context* context)
	:	m_context(context),
		m_window(window)
{
	GK_ASSERT(m_window && m_context);
	m_window->addListener(this);
}

RocketEventListener::~RocketEventListener()
{
	if (m_window)
		m_window->removeListener(this);
}


void RocketEventListener::mouseMoved(const gkMouse& mouse)
{
	int key_modifier_state = getKeyModifierState(m_window->getKeyModifier());

	m_context->ProcessMouseMove(mouse.position.x, mouse.position.y,
		key_modifier_state);

	if (mouse.wheelDelta != 0)
		m_context->ProcessMouseWheel(-mouse.wheelDelta, key_modifier_state);
}

void RocketEventListener::mousePressed(const gkMouse& mouse)
{
	for (int i = 0; i < 3; i++)
		if (mouse.isButtonDown(i))
			m_context->ProcessMouseButtonDown(i, getKeyModifierState(m_window->getKeyModifier()));
}

void RocketEventListener::mouseReleased(const gkMouse& mouse)
{
	for (int i = 0; i < 3; i++)
		if (!mouse.isButtonDown(i))
			m_context->ProcessMouseButtonUp(i, getKeyModifierState(m_window->getKeyModifier()));
}


void RocketEventListener::keyPressed(const gkKeyboard &key, const gkScanCode &sc)
{
	Rocket::Core::Input::KeyIdentifier key_identifier = (Rocket::Core::Input::KeyIdentifier)getKeyCode(sc);

	// Toggle the debugger on a shift-~ press.
	if (key_identifier == Rocket::Core::Input::KI_OEM_3 &&
		(getKeyModifierState(key.key_mod) & Rocket::Core::Input::KM_SHIFT))
	{
		Rocket::Debugger::SetVisible(!Rocket::Debugger::IsVisible());
		return;
	}

	if (key_identifier != Rocket::Core::Input::KI_UNKNOWN)
		m_context->ProcessKeyDown(key_identifier, getKeyModifierState(key.key_mod));

	// Send through the ASCII value as text input if it is printable.
	if (key.text >= 32)
		m_context->ProcessTextInput((Rocket::Core::word)key.text);
	else if (key_identifier == Rocket::Core::Input::KI_RETURN)
		m_context->ProcessTextInput((Rocket::Core::word)'\n');
}

void RocketEventListener::keyReleased(const gkKeyboard &key, const gkScanCode &sc)
{
	Rocket::Core::Input::KeyIdentifier key_identifier = (Rocket::Core::Input::KeyIdentifier)getKeyCode(sc);

	if (key_identifier != Rocket::Core::Input::KI_UNKNOWN)
		m_context->ProcessKeyUp(key_identifier, getKeyModifierState(key.key_mod));

}


int RocketEventListener::getKeyCode(int kc)
{
#define CASE(c, ret) case (c): return ret
	using namespace Rocket::Core::Input;
	switch (kc)
	{
		CASE(KC_ACCENTGRAVEKEY   ,    KI_OEM_3);
		CASE(KC_ZEROKEY          ,    KI_1);
		CASE(KC_ONEKEY           ,    KI_2);
		CASE(KC_TWOKEY           ,    KI_3);
		CASE(KC_THREEKEY         ,    KI_4);
		CASE(KC_FOURKEY          ,    KI_5);
		CASE(KC_FIVEKEY          ,    KI_6);
		CASE(KC_SIXKEY           ,    KI_7);
		CASE(KC_SEVENKEY         ,    KI_8);
		CASE(KC_EIGHTKEY         ,    KI_9);
		CASE(KC_NINEKEY          ,    KI_0);
		CASE(KC_PAD0             ,    KI_NUMPAD0);
		CASE(KC_PAD1             ,    KI_NUMPAD1);
		CASE(KC_PAD2             ,    KI_NUMPAD2);
		CASE(KC_PAD3             ,    KI_NUMPAD3);
		CASE(KC_PAD4             ,    KI_NUMPAD4);
		CASE(KC_PAD5             ,    KI_NUMPAD5);
		CASE(KC_PAD6             ,    KI_NUMPAD6);
		CASE(KC_PAD7             ,    KI_NUMPAD7);
		CASE(KC_PAD8             ,    KI_NUMPAD8);
		CASE(KC_PAD9             ,    KI_NUMPAD9);
		CASE(KC_PADSLASHKEY      ,    KI_DIVIDE);
		CASE(KC_PADENTER         ,    KI_NUMPADENTER);
		CASE(KC_PADEQUALS        ,    KI_OEM_NEC_EQUAL);
		CASE(KC_PADPERIOD        ,    KI_DECIMAL);
		CASE(KC_PADASTERKEY      ,    KI_MULTIPLY);
		CASE(KC_PADPLUSKEY       ,    KI_ADD);
		CASE(KC_AKEY             ,    KI_A);
		CASE(KC_BKEY             ,    KI_B);
		CASE(KC_CKEY             ,    KI_C);
		CASE(KC_DKEY             ,    KI_D);
		CASE(KC_EKEY             ,    KI_E);
		CASE(KC_FKEY             ,    KI_F);
		CASE(KC_GKEY             ,    KI_G);
		CASE(KC_HKEY             ,    KI_H);
		CASE(KC_IKEY             ,    KI_I);
		CASE(KC_JKEY             ,    KI_J);
		CASE(KC_KKEY             ,    KI_K);
		CASE(KC_LKEY             ,    KI_L);
		CASE(KC_MKEY             ,    KI_M);
		CASE(KC_NKEY             ,    KI_N);
		CASE(KC_OKEY             ,    KI_O);
		CASE(KC_PKEY             ,    KI_P);
		CASE(KC_QKEY             ,    KI_Q);
		CASE(KC_RKEY             ,    KI_R);
		CASE(KC_SKEY             ,    KI_S);
		CASE(KC_TKEY             ,    KI_T);
		CASE(KC_UKEY             ,    KI_U);
		CASE(KC_VKEY             ,    KI_V);
		CASE(KC_WKEY             ,    KI_W);
		CASE(KC_XKEY             ,    KI_X);
		CASE(KC_YKEY             ,    KI_Y);
		CASE(KC_ZKEY             ,    KI_Z);
		CASE(KC_CAPSLOCKKEY      ,    KI_CAPITAL);
		CASE(KC_LEFTCTRLKEY      ,    KI_LCONTROL);
		CASE(KC_LEFTALTKEY       ,    KI_LMENU);
		CASE(KC_RIGHTALTKEY      ,    KI_RMENU);
		CASE(KC_RIGHTCTRLKEY     ,    KI_RCONTROL);
		CASE(KC_RIGHTSHIFTKEY    ,    KI_RSHIFT);
		CASE(KC_LEFTSHIFTKEY     ,    KI_LSHIFT);
		CASE(KC_ESCKEY           ,    KI_ESCAPE);
		CASE(KC_TABKEY           ,    KI_TAB);
		CASE(KC_RETKEY           ,    KI_RETURN);
		CASE(KC_SPACEKEY         ,    KI_SPACE);
		CASE(KC_BACKSPACEKEY     ,    KI_BACK);
		CASE(KC_DELKEY           ,    KI_DELETE);
		CASE(KC_SEMICOLONKEY     ,    KI_OEM_1);
		CASE(KC_PERIODKEY        ,    KI_OEM_PERIOD);
		CASE(KC_COMMAKEY         ,    KI_OEM_COMMA);
		CASE(KC_QUOTEKEY         ,    KI_OEM_7);
		CASE(KC_MINUSKEY         ,    KI_OEM_MINUS);
		CASE(KC_SLASHKEY         ,    KI_OEM_2);
		CASE(KC_BACKSLASHKEY     ,    KI_OEM_5);
		CASE(KC_EQUALKEY         ,    KI_OEM_PLUS);
		CASE(KC_LEFTARROWKEY     ,    KI_LEFT);
		CASE(KC_DOWNARROWKEY     ,    KI_DOWN);
		CASE(KC_RIGHTARROWKEY    ,    KI_RIGHT);
		CASE(KC_UPARROWKEY       ,    KI_UP);
		CASE(KC_F1KEY            ,    KI_F1);
		CASE(KC_F2KEY            ,    KI_F2);
		CASE(KC_F3KEY            ,    KI_F3);
		CASE(KC_F4KEY            ,    KI_F4);
		CASE(KC_F5KEY            ,    KI_F5);
		CASE(KC_F6KEY            ,    KI_F6);
		CASE(KC_F7KEY            ,    KI_F7);
		CASE(KC_F8KEY            ,    KI_F8);
		CASE(KC_F9KEY            ,    KI_F9);
		CASE(KC_F10KEY           ,    KI_F10);
		CASE(KC_F11KEY           ,    KI_F11);
		CASE(KC_F12KEY           ,    KI_F12);
		CASE(KC_HOMEKEY          ,    KI_HOME);
		CASE(KC_PAGEUPKEY        ,    KI_PRIOR);
		CASE(KC_PAGEDOWNKEY      ,    KI_NEXT);
		CASE(KC_ENDKEY           ,    KI_END);
		CASE(KC_INSERTKEY        ,    KI_INSERT);
		CASE(KC_PAUSEKEY         ,    KI_PAUSE);
		default: break;
	}
	return KI_UNKNOWN;

#undef CASE
}


int RocketEventListener::getKeyModifierState(int mod)
{
	int rmod = 0;

	if (mod & gkKeyboard::KM_ALT)
		rmod |= Rocket::Core::Input::KM_ALT;
	if (mod & gkKeyboard::KM_CTRL)
		rmod |= Rocket::Core::Input::KM_CTRL;
	if (mod & gkKeyboard::KM_SHIFT)
		rmod |= Rocket::Core::Input::KM_SHIFT;
	
	return rmod;
}

