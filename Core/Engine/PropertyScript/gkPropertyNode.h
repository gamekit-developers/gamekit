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
#ifndef _gkPropertyNode_h_
#define _gkPropertyNode_h_

#include "gkPropertyCommon.h"




// ----------------------------------------------------------------------------
class gkPropertyNode
{
public:
	gkPropertyNode();
	~gkPropertyNode();

	void setDebugNode(const gkDebugNode &dn);
	const gkDebugNode& getDebugNode(void);

	const Ogre::String& getName(void);
	const Ogre::String& getType(void);
	const Ogre::String& getBase(void);

	void setName(const Ogre::String &v);
	void setType(const Ogre::String &v);
	void setBase(const Ogre::String &v);


	void addChild(gkPropertyNode *node);
	void addAttribute(gkPropertyAttribute *attr);

	bool hasAttribute(const gkHashedString& name);
	gkPropertyAttribute* getAttribute(const gkHashedString& name);


	gkPropertyNode*	   getParent(void);
	gkNodeList&		   getChildren(void);
	gkAttributeList&	  getAttributes(void);

	gkNodeIterator		getChildIterator(void);
	gkAttributeIterator   getAttributeIterator(void);

protected:

	void setParent(gkPropertyNode *par);

	Ogre::String		  m_name;
	Ogre::String		  m_type;
	Ogre::String		  m_base;
	gkPropertyNode*		   m_parent;
	gkNodeList		m_children;
	gkAttributeList   m_attributes;
	gkAttributeMap	m_attrMap;
	gkDebugNode	   m_debug;
};


// ----------------------------------------------------------------------------
GK_INLINE void gkPropertyNode::setDebugNode(const gkDebugNode &dn)
{
	m_debug.line= dn.line;
	m_debug.source= dn.source;
}

// ----------------------------------------------------------------------------
GK_INLINE const gkDebugNode& gkPropertyNode::getDebugNode(void)
{
	return m_debug;
}

// ----------------------------------------------------------------------------
GK_INLINE const Ogre::String& gkPropertyNode::getName(void)
{
	return m_name;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkPropertyNode::setName(const Ogre::String &v)
{
	m_name= v;
}

// ----------------------------------------------------------------------------
GK_INLINE const Ogre::String& gkPropertyNode::getType(void)
{
	return m_type;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkPropertyNode::setType(const Ogre::String &v)
{
	m_type= v;
}

// ----------------------------------------------------------------------------
GK_INLINE const Ogre::String& gkPropertyNode::getBase(void)
{
	return m_base;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkPropertyNode::setBase(const Ogre::String &v)
{
	m_base= v;
}

// ----------------------------------------------------------------------------
GK_INLINE gkPropertyNode* gkPropertyNode::getParent(void)
{
	return m_parent;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkPropertyNode::setParent(gkPropertyNode *par)
{
	m_parent= par;
}

// ----------------------------------------------------------------------------
GK_INLINE gkNodeList& gkPropertyNode::getChildren(void)
{
	return m_children;
}

// ----------------------------------------------------------------------------
GK_INLINE gkAttributeList& gkPropertyNode::getAttributes(void)
{
	return m_attributes;
}

// ----------------------------------------------------------------------------
GK_INLINE gkNodeIterator gkPropertyNode::getChildIterator(void)
{
	// iterator access
	return gkNodeIterator(m_children.begin());

}

// ----------------------------------------------------------------------------
GK_INLINE gkAttributeIterator gkPropertyNode::getAttributeIterator(void)
{
	// iterator access
	return gkAttributeIterator(m_attributes.begin());
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkPropertyNode::hasAttribute(const gkHashedString& name)
{
	return m_attrMap.find(name) != GK_NPOS;
}




#endif//_gkPropertyNode_h_
