/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#ifndef _gkEntityObject_h_
#define _gkEntityObject_h_

#include "gkGameObject.h"
#include "gkSerialize.h"


class gkEntity : public gkGameObject
{
public:
	gkEntity(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkEntity();

	GK_INLINE Ogre::Entity* getEntity(void) { return m_entity; }

	GK_INLINE gkEntityProperties&  getEntityProperties(void) {return *m_entityProps;}
	
	GK_INLINE gkMesh* getMesh(void) {return m_entityProps->m_mesh; }
	
	GK_INLINE gkSkeleton* getSkeleton(void) {return m_skeleton;}
	void          setSkeleton(gkSkeleton* skel);

	void _resetPose(void);


	// Remove only the entity but keep the rest.
	void _destroyAsStaticGeometry(void);


protected:


	virtual gkBoundingBox getAabb() const;

	gkGameObject* clone(const gkString& name);

	gkEntityProperties*     m_entityProps;
	Ogre::Entity*           m_entity;
	gkSkeleton*             m_skeleton;

	virtual void createInstanceImpl();
	virtual void destroyInstanceImpl();
};


#endif//_gkEntityObject_h_
