/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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
#ifndef _gkButtonNode_h_
#define _gkButtonNode_h_


#include "gkLogicNode.h"
#include "gkInput.h"

class gkButtonNode : public gkLogicNode
{
public:

	enum
	{
		UPDATED,
		IS_DOWN,
		NOT_IS_DOWN,
		PRESS,
		RELEASE
	};

	DECLARE_SOCKET_TYPE(UPDATED, bool);
	DECLARE_SOCKET_TYPE(IS_DOWN, bool);
	DECLARE_SOCKET_TYPE(NOT_IS_DOWN, bool);
	DECLARE_SOCKET_TYPE(PRESS, bool);
	DECLARE_SOCKET_TYPE(RELEASE, bool);

	gkButtonNode(gkLogicTree* parent, size_t id);

	virtual ~gkButtonNode() {}

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);

	GK_INLINE void setDelay(int d)
	{
		m_delay = d;
	}

	virtual bool isButtonDown() const = 0;

private:

	int m_delay;
	int m_counter;

	bool m_pressed;
};

class gkKeyNode : public gkButtonNode
{
public:

	gkKeyNode(gkLogicTree* parent, size_t id)
		: gkButtonNode(parent, id), m_key(KC_NONE)
	{
	}

	~gkKeyNode()
	{
	}

	GK_INLINE void setKey(gkScanCode key)
	{
		m_key = key;
	}

	bool isButtonDown() const;

private:

	gkScanCode m_key;
};

class gkMouseButtonNode : public gkButtonNode
{
public:

	gkMouseButtonNode(gkLogicTree* parent, size_t id)
		: gkButtonNode(parent, id), m_button(gkMouse::Left)
	{
	}

	~gkMouseButtonNode()
	{
	}

	GK_INLINE void setButton(gkMouse::Buttons button)
	{
		m_button = button;
	}

	bool isButtonDown() const;

private:

	gkMouse::Buttons m_button;
};



#endif//_gkButtonNode_h_
