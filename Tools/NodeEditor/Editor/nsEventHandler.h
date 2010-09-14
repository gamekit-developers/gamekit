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
#ifndef _nsEventHandler_h_
#define _nsEventHandler_h_

#include <assert.h>
#include "nsCommon.h"
class wxWindow;




enum nsNodifierID
{
	NS_TREE_ADD,
	NS_TREE_REMOVE,
	NS_TREE_SELECT,
	NS_TREE_DESELECT,
	NS_TREE_CHANGED,
	NS_TREE_CLEAR,

	NS_NODE_ADD,
	NS_NODE_REMOVE,
	NS_NODE_SELECT,
	NS_NODE_DESELECT,

	NS_SOCKET_LINK,
	NS_SOCKET_UNLINK,
};

template<typename T>
class nsAbstractEvent
{
protected:
	wxWindow*        m_caller;
	T*               m_object;
	nsNodifierID    m_id;

public:
	nsAbstractEvent(nsNodifierID id, wxWindow* caller, T* ob)
		:   m_caller(caller), m_object(ob), m_id(id)
	{
	}

	wxWindow*        getCaller(void)    {return m_caller;}
	T&               ref(void)          {assert(m_object); return *m_object;}
	T*               ptr(void)          {assert(m_object); return m_object;}
	nsNodifierID    getId(void)         {return m_id;}

};


typedef nsAbstractEvent<nsNodeTree> nsTreeEvent;
typedef nsAbstractEvent<nsNode>     nsNodeEvent;
typedef nsAbstractEvent<nsSocket>   nsSocketEvent;



// simple callback handler
class nsNotifierBase
{
public:
	virtual ~nsNotifierBase() {}

	virtual void treeEvent(nsTreeEvent& evt)         {}
	virtual void nodeEvent(nsNodeEvent& evt)         {}
	virtual void socketEvent(nsSocketEvent& evt)     {}
};



#endif//_nsEventHandler_h_
