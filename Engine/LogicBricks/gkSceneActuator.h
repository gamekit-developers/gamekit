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

#ifndef GKSCENEACTUATOR_H
#define GKSCENEACTUATOR_H

#include "gkLogicActuator.h"

class gkSceneActuator : public gkLogicActuator
{
public:
	enum Mode
	{
		SC_RESTART,
		SC_SET_SCENE,
		SC_SET_CAMERA,
		SC_ADD_FRONT,
		SC_ADD_BACK,
		SC_REMOVE,
		SC_SUSPEND,
		SC_RESUME,
	};

private:
	int m_mode;
	gkString m_sceneName;
	gkString m_camera;

public:
	gkSceneActuator(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkSceneActuator() {}


	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);
	void execute(void);

	GK_INLINE void            setMode(int v)               {m_mode = v;}
	GK_INLINE void            setScene(const gkString& v)  {m_sceneName = v;}
	GK_INLINE void            setCamera(const gkString& v) {m_camera = v;}

	GK_INLINE int             getMode(void)                const {return m_mode;}
	GK_INLINE const gkString& getScene(void)               const {return m_sceneName;}
	GK_INLINE const gkString& getCamera(void)              const {return m_camera;}
};

#endif // GKSCENEACTUATOR_H
