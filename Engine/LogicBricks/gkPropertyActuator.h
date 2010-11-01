/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#ifndef _gkPropertyActuator_h_
#define _gkPropertyActuator_h_

#include "gkLogicActuator.h"
#include "gkVariable.h"



class gkPropertyActuator : public gkLogicActuator
{
public:

	enum Type
	{
		PA_ASSIGN,
		PA_ADD,
		PA_COPY,
		PA_TOGGLE,
	};

private:
	int         m_type;
	gkString    m_prop, m_value, m_othOb;
	bool        m_init;
	gkVariable*  m_cur, *m_oth;
	gkVariable  m_propVal;


public:

	gkPropertyActuator(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkPropertyActuator();

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	GK_INLINE void      setType(int v)                  {m_type = v;}
	GK_INLINE void      setProperty(const gkString& v)  {m_prop = v;}
	GK_INLINE void      setValue(const gkString& v)     {m_value = v;}
	GK_INLINE void      setObject(const gkString& v)    {m_othOb = v;}

	GK_INLINE int       getType(void)                   const {return m_type;}
	GK_INLINE gkString  getProperty(void)               const {return m_prop;}
	GK_INLINE gkString  getValue(void)                  const {return m_value;}
	GK_INLINE gkString  getObject(void)                 const {return m_othOb;}


	void execute(void);
};


#endif//_gkPropertyActuator_h_
