/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): silveira.nestor.
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
#ifndef _gkLogicTree_h_
#define _gkLogicTree_h_

#include "gkLogicCommon.h"
#include "gkLogicNode.h"
#include "gkResource.h"

class gkGameObject;
class gkNodeManager;

class gkLogicTree : public gkResource
{
public:
	typedef utList<gkLogicNode*>        NodeList;
	typedef utListIterator<NodeList>    NodeIterator;


public:
	gkLogicTree(gkResourceManager *creator, const gkResourceName &name, const gkResourceHandle &handle);


	gkLogicTree(gkNodeManager* creator, UTsize id, const gkString& name);
	gkLogicTree(gkNodeManager* creator, UTsize id);
	~gkLogicTree();

	// execute all nodes
	void execute(gkScalar tick);


	GK_INLINE gkGameObject* getAttachedObject(void) {return m_object;}
	GK_INLINE bool hasNodes(void)                   {return !m_nodes.empty();}
	GK_INLINE bool isGroup(void)                    {return !m_name.getName().empty();}
	GK_INLINE void markDirty(void)                  {m_initialized = false;}
	GK_INLINE NodeIterator getNodeIterator(void)    {return NodeIterator(m_nodes);}


	void attachObject(gkGameObject* ob);

	// node access

	template<class T>
	T* createNode(void)
	{
		T* pNode = new T(this, m_uniqueHandle);
		GK_ASSERT(pNode);

		if (m_object) pNode->attachObject(m_object);
		m_nodes.push_back(pNode);
		m_uniqueHandle ++;
		return pNode;
	}

	gkLogicNode*            getNode(UTsize idx);
	void                    destroyNodes(void);
	void                    freeUnused(void);
	void                    solveOrder(bool forceSolve = false);

protected:
	bool                m_initialized, m_sorted;
	size_t              m_uniqueHandle;
	gkGameObject*       m_object;
	NodeList            m_nodes;
};


#endif//_gkLogicTree_h_
