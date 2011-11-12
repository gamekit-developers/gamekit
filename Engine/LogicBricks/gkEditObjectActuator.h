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
#ifndef _gkEditObjectActuator_h_
#define _gkEditObjectActuator_h_

#include "gkLogicActuator.h"



class gkEditObjectActuator : public gkLogicActuator, public gkInstancedManager::InstancedListener
{
public:

	enum Mode
	{
		EO_ADDOBJ,
		EO_ENDOBJ,
		EO_REPLACEMESH,
		EO_TRACKTO,
		EO_DYNAMICS,
	};

	enum Dyn
	{
		EOD_RESTORE,
		EOD_SUSPEND,
		EOD_ENABLE_BODY,
		EOD_DISABLE_BODY,
		EOD_SETMASS,
	};


private:

	gkVector3   m_linv, m_angv;
	bool        m_lvlocal, m_avlocal;
	int         m_life, m_mode, m_dynMode;
	gkString    m_obj;
	gkGameObject* m_lastCreatedObject;

	void addObject(void);
	void endObject(void);

    void trackToObject(void);

public:

	gkEditObjectActuator(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkEditObjectActuator();

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	GK_INLINE void  setMode(int v)                  {m_mode = v;}
	GK_INLINE void  setDynMode(int v)               {m_dynMode = v;}
	GK_INLINE void  setObject(const gkString& v)    {m_obj = v;}
	GK_INLINE void  setLinV(const gkVector3& v)     {m_linv = v;}
	GK_INLINE void  setLinVL(bool v)                {m_lvlocal = v;}
	GK_INLINE void  setAngV(const gkVector3& v)     {m_angv = v;}
	GK_INLINE void  setAngVL(bool v)                {m_avlocal = v;}
	GK_INLINE void  setLifeSpan(int v)              {m_life = v;}


	GK_INLINE int   getMode(void)                   const {return m_mode;}
	GK_INLINE int   getDynMode(void)                const {return m_dynMode;}
	GK_INLINE const gkString& getObject(void)       const {return m_obj;}
	GK_INLINE const gkVector3& getLinV(void)        const {return m_linv;}
	GK_INLINE bool  getLinVL(void)                  const {return m_lvlocal;}
	GK_INLINE const gkVector3& getAngV(void)        const {return m_angv;}
	GK_INLINE bool  getAngVL(void)                  const {return m_avlocal;}
	GK_INLINE int   getLifeSpan(void)               const {return m_life;}

	GK_INLINE gkGameObject* getLastCreatedObject(void) const {return m_lastCreatedObject;}

	virtual void notifyInstanceDestroyed(gkInstancedObject* inst) {
		if (m_lastCreatedObject==inst){
			m_lastCreatedObject=NULL;
		}
	}

	void execute(void);
};

#endif//_gkEditObjectActuator_h_
