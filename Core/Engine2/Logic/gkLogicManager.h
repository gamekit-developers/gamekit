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
#ifndef _gkLogicManager_h_
#define _gkLogicManager_h_

#include "gkLogicCommon.h"
#include "gkMathUtils.h"
#include "OgreIteratorWrappers.h"
#include "OgreCommon.h"

#include "OgreSingleton.h"



class gkGameObject;



class gkLogicManager : public Ogre::Singleton<gkLogicManager>
{
public:
    typedef std::map<size_t, gkLogicTree*> NodeTree;
    typedef Ogre::MapIterator<NodeTree> NodeTreeIterator;
    typedef std::vector<gkLogicTree*> TreeList;

public:
    gkLogicManager();
    ~gkLogicManager();

    gkLogicTree* create();
    gkLogicTree* create(const gkString &name);

    gkLogicTree* get(size_t id);
    gkLogicTree* get(const gkString &name);
    TreeList get(gkGameObject* ob);

    NodeTreeIterator getIterator();

    void destroy(gkLogicTree* tree);
    void destroy(size_t handle);

    static gkLogicManager& getSingleton();
    static gkLogicManager* getSingletonPtr();

    void update(gkScalar tick);

    void clear();
private:

    TreeList    m_locals;
    size_t      m_uniqueHandle;
    NodeTree    m_trees;
};


#endif//_gkLogicManager_h_
