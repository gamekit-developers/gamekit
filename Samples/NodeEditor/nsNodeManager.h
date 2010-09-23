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
#ifndef _nsNodeManager_h_
#define _nsNodeManager_h_

#include "nsCommon.h"
#include "nsSingleton.h"
#include "nsNodeTypeInfo.h"
#include "nsSocket.h"
#include "nsNode.h"
#include "nsNodeTree.h"



class nsNodeManager : public nsSingleton<nsNodeManager>
{
protected:
	nsTrees         m_trees;
public:
	nsNodeManager();
	~nsNodeManager();

	void            clear(void);
	void            deleteTree(nsNodeTree* tree);
	nsNodeTree*      createTree(const nsString& name = "Node Tree");
	nsTreeIterator  getTreeIterator(void) {return nsTreeIterator(m_trees);}

	NS_DECLARE_SINGLETON(nsNodeManager);
};


#endif//_nsNodeManager_h_
