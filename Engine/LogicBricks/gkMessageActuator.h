/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#ifndef GKMESSAGEACTUATOR_H
#define GKMESSAGEACTUATOR_H

#include "gkLogicActuator.h"

class gkMessageActuator : public gkLogicActuator
{
public:
	enum BodyType
	{
		BT_TEXT,
		BT_PROP,
	};

private:
	gkString m_to, m_subject, m_bodyText, m_bodyProp;
	int m_bodyType;

public:
	gkMessageActuator(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkMessageActuator() {}

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	void execute(void);

	GK_INLINE void setTo(gkString v)                  {m_to = v;}
	GK_INLINE void setSubject(const gkString& v)      {m_subject = v;}
	GK_INLINE void setBodyType(int v)                 {m_bodyType = v;}
	GK_INLINE void setBodyText(const gkString& v)     {m_bodyText = v;}
	GK_INLINE void setBodyProperty(const gkString& v) {m_bodyProp = v;}

	GK_INLINE const gkString& getTo(void)           const {return m_to;}
	GK_INLINE const gkString& getSubject(void)      const {return m_subject;}
	GK_INLINE int             getBodyType(void)     const {return m_bodyType;}
	GK_INLINE const gkString& getBodyText(void)     const {return m_bodyText;}
	GK_INLINE const gkString& getBodyProperty(void) const {return m_bodyProp;}
};

#endif // GKMESSAGEACTUATOR_H
