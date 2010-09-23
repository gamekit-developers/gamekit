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
#ifndef _gkConstraintManager_h_
#define _gkConstraintManager_h_

#include "gkCommon.h"
#include "gkMathUtils.h"


class gkConstraintManager
{
public:
	typedef utArray<gkConstraint*>        Constraints;
	typedef utArrayIterator<Constraints>  ConstraintIterator;
	typedef utHashTable<utPointerHashKey, Constraints*> ConstraintObjectMap;


public:

	gkConstraintManager();
	~gkConstraintManager();

	void clear(void);

	void notifyObjectDestroyed(gkGameObject* gobj);
	void notifyObjectCloned(gkGameObject* oobj, gkGameObject* nobj);
	void notifyInstanceCreated(gkGameObject* gobj);
	void notifyInstanceDestroyed(gkGameObject* gobj);


	void addConstraint(gkGameObject* obj, gkConstraint* cons);
	void removeConstraint(gkGameObject* obj, gkConstraint* cons);


	Constraints& getConstraints(gkGameObject* obj);
	bool hasConstraints(gkGameObject* obj);



	void update(gkScalar delta);


private:



	void removeUpdate(gkConstraint* cons);


	Constraints            m_updateConstraints;
	ConstraintObjectMap    m_objectMapConstraints;
};


#endif//_gkConstraintManager_h_
