/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#ifndef _utPropertyTree_h_
#define _utPropertyTree_h_

#include "gkPropertyCommon.h"
#include "gkPropertyNode.h"
#include "gkPropertyAttribute.h"



// ----------------------------------------------------------------------------
class gkPropertyTree
{
public:

	gkPropertyTree(const Ogre::String& fname);
	~gkPropertyTree();

	const Ogre::String& getName(void);

	/// data access
	gkNodeList& getNodes(void);
	gkNodeIterator getNodeIterator(void);
	gkPropertyNode *findNode(const Ogre::String& name);
	gkPropertyNode *findFirstType(const Ogre::String& type);


	/// creation
	void addRootNode(gkPropertyNode *root);

	/// tracking
	void addNode(gkPropertyNode *node);
	void addAttribute(gkPropertyAttribute *attr);

	void addInclude(const Ogre::String& include);
	Ogre::StringVector &getIncludes(void);

protected:

	const Ogre::String	m_file;
	gkNodeList		m_nodes;
	gkAttributeList   m_attributes;
	gkNodeList		m_roots;
	Ogre::StringVector	m_includes;
};


// ----------------------------------------------------------------------------
GK_INLINE const Ogre::String& gkPropertyTree::getName(void)
{
	return m_file;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkPropertyTree::addRootNode(gkPropertyNode *root)
{
	/// top down (add to back)
	m_roots.push_back(root);
}


// ----------------------------------------------------------------------------
GK_INLINE gkNodeList& gkPropertyTree::getNodes(void)
{
	return m_nodes;
}

// ----------------------------------------------------------------------------
GK_INLINE gkNodeIterator gkPropertyTree::getNodeIterator(void)
{
	return gkNodeIterator(m_roots.begin());
}

// ----------------------------------------------------------------------------
GK_INLINE void gkPropertyTree::addInclude(const Ogre::String& include)
{
	m_includes.push_back(include);
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::StringVector &gkPropertyTree::getIncludes(void)
{
	return m_includes;
}




#endif//_utPropertyTree_h_
