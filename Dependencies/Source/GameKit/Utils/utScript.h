/*
-------------------------------------------------------------------------------
    General Purpose Utility Library, should be kept dependency free.
    Unless the dependency can be compiled along with this library.

    Copyright (c) 2009-2010 Charlie C.
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
#ifndef _utScript_h_
#define _utScript_h_

#include "utString.h"


class utScriptTree;
class utScriptNode;
class utScriptAttribute;
class utScriptScanner;




struct utScriptDebug
{
	UTsize line;
	utString source;
};

class utScriptAttribute
{
public:

	utScriptAttribute(const utString &name, const utString &val);
	~utScriptAttribute();


	void  setDebugNode(const utScriptDebug &dn);
	const utScriptDebug &getDebugNode(void);


	const utString &getName(void);
	const utString &getValue(void);

	void setName(const utString &v);
	void setValue(const utString &v);

protected:
	utString      m_name;
	utString      m_value;
	utScriptDebug m_debug;
};



UT_INLINE void utScriptAttribute::setDebugNode(const utScriptDebug &dn)
{
	m_debug.line= dn.line;
	m_debug.source= dn.source;
}



UT_INLINE const utScriptDebug &utScriptAttribute::getDebugNode(void)
{
	return m_debug;
}



UT_INLINE const utString &utScriptAttribute::getName(void)
{
	return m_name;
}


UT_INLINE const utString &utScriptAttribute::getValue(void)
{
	return m_value;
}


UT_INLINE void utScriptAttribute::setName(const utString &v)
{
	m_name= v;
}


UT_INLINE void utScriptAttribute::setValue(const utString &v)
{
	m_value= v;
}

class utScriptNode
{
public:

	typedef utArray<utScriptNode *> Nodes;
	typedef utArray<utScriptAttribute *> Attributes;
	typedef utHashTable<utHashedString, utScriptAttribute *> AttributeMap;

public:
	utScriptNode(utScriptTree *tree);
	~utScriptNode();

	void setDebugNode(const utScriptDebug &dn);
	const utScriptDebug &getDebugNode(void);

	const utString &getName(void);
	const utString &getType(void);
	const utString &getBase(void);

	void setName(const utString &v);
	void setType(const utString &v);
	void setBase(const utString &v);


	void addChild(utScriptNode *node);
	void addAttribute(utScriptAttribute *attr);

	bool hasAttribute(const utHashedString &name);
	utScriptAttribute *getAttribute(const utHashedString &name, bool includeExtern = true);

	bool hasReferencedAttribute(const utHashedString &name);

	utScriptNode        *getParent(void);
	Nodes               &getChildren(void);
	Attributes          &getAttributes(void);

	void getChildren(Nodes &dest, const utString &type, bool fromBaseClass = true, bool fromBaseClassRecursive = false);

protected:

	void setParent(utScriptNode *par);
	utScriptAttribute *getParentAttribute(const utHashedString &name);

	utString          m_name;
	utString          m_type;
	utString          m_base;
	utScriptNode     *m_parent;
	utScriptTree     *m_root;
	Nodes             m_children;
	Attributes        m_attributes;
	AttributeMap      m_attrMap;
	utScriptDebug     m_debug;
};



UT_INLINE void utScriptNode::setDebugNode(const utScriptDebug &dn)
{
	m_debug.line= dn.line;
	m_debug.source= dn.source;
}


UT_INLINE const utScriptDebug &utScriptNode::getDebugNode(void)
{
	return m_debug;
}


UT_INLINE const utString &utScriptNode::getName(void)
{
	return m_name;
}


UT_INLINE void utScriptNode::setName(const utString &v)
{
	m_name= v;
}


UT_INLINE const utString &utScriptNode::getType(void)
{
	return m_type;
}


UT_INLINE void utScriptNode::setType(const utString &v)
{
	m_type= v;
}


UT_INLINE const utString &utScriptNode::getBase(void)
{
	return m_base;
}


UT_INLINE void utScriptNode::setBase(const utString &v)
{
	m_base= v;
}


UT_INLINE utScriptNode *utScriptNode::getParent(void)
{
	return m_parent;
}


UT_INLINE void utScriptNode::setParent(utScriptNode *par)
{
	m_parent= par;
}


UT_INLINE utScriptNode::Nodes &utScriptNode::getChildren(void)
{
	return m_children;
}


UT_INLINE utScriptNode::Attributes &utScriptNode::getAttributes(void)
{
	return m_attributes;
}


UT_INLINE bool utScriptNode::hasAttribute(const utHashedString &name)
{
	return m_attrMap.find(name) != UT_NPOS;
}

class utScriptTree
{
public:

	typedef utArray<utScriptNode *> Nodes;
	typedef utArray<utScriptAttribute *> Attributes;
	typedef utHashTable<utHashedString, utScriptAttribute *> AttributeMap;


public:

	utScriptTree(const utString &fname);
	~utScriptTree();

	const utString &getName(void);

	Nodes        &getNodes(void);
	utScriptNode *findNode(const utString &name);
	utScriptNode *findFirstType(const utString &type);


	void addRootNode(utScriptNode *root);

	void addNode(utScriptNode *node);
	void addAttribute(utScriptAttribute *attr);


	bool hasAttribute(const utHashedString &name);
	utScriptAttribute *getAttribute(const utHashedString &name);


protected:

	const utString      m_file;
	Nodes               m_nodes, m_roots;
	Attributes          m_attributes;
	AttributeMap        m_attrMap;

};



UT_INLINE const utString &utScriptTree::getName(void)
{
	return m_file;
}


UT_INLINE void utScriptTree::addRootNode(utScriptNode *root)
{
	m_roots.push_back(root);
}



UT_INLINE utScriptTree::Nodes &utScriptTree::getNodes(void)
{
	return m_nodes;
}


UT_INLINE bool utScriptTree::hasAttribute(const utHashedString &name)
{
	return m_attrMap.find(name) != UT_NPOS;
}


class utScript
{
public:
	utScript();
	~utScript();

	void parseBuffer(const utString &name, const utString &membuf);

	utScriptTree &getTree(void);
	utScriptTree *getTreePtr(void);

protected:


	void scan(utScriptScanner *scnr);
	utScriptTree *m_tree;
};


UT_INLINE utScriptTree &utScript::getTree(void)
{
	UT_ASSERT(m_tree);
	return *m_tree;
}



UT_INLINE utScriptTree *utScript::getTreePtr(void)
{
	return m_tree;
}



#endif//_utScript_h_
