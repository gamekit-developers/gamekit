/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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

namespace
{
template<typename T, gkBoolStatement stmt>
gkIfNode<T, stmt>* gkCreateIfNode(gkLogicTree* tree, gkILogicSocket* a, gkILogicSocket* b)
{
	typedef gkIfNode<T, stmt> NODE_TYPE;

	NODE_TYPE* ifNode = tree->createNode<NODE_TYPE >();
	ifNode->getA()->link(a);
	ifNode->getB()->link(b);

	return ifNode;
}

template<typename T, gkBoolStatement stmt>
gkIfNode<T, stmt>* gkCreateIfNode(gkLogicTree* tree, gkILogicSocket* a, T b)
{
	typedef gkIfNode<T, stmt> NODE_TYPE;

	NODE_TYPE* ifNode = tree->createNode<NODE_TYPE >();
	ifNode->getA()->link(a);
	ifNode->getB()->setValue(b);

	return ifNode;
}

template<typename T, gkBoolStatement stmt>
gkIfNode<T, stmt>* gkCreateIfNode(gkLogicTree* tree, T a, gkILogicSocket* b)
{
	typedef gkIfNode<T, stmt> NODE_TYPE;

	NODE_TYPE* ifNode = tree->createNode<NODE_TYPE >();
	ifNode->getA()->setValue(a);
	ifNode->getB()->link(b);

	return ifNode;
}

#define BOOL_AND_NODE( a, b ) gkCreateIfNode<bool, CMP_AND>(m_tree, a, b)
#define BOOL_OR_NODE( a, b ) gkCreateIfNode<bool, CMP_OR>(m_tree, a, b)
#define STRING_EQUAL_NODE( a, b ) gkCreateIfNode<gkString, CMP_EQUALS>(m_tree, a, b)
#define INT_EQUAL_NODE( a, b ) gkCreateIfNode<int, CMP_EQUALS>(m_tree, a, b)
#define PGAMEOBJ_EQUAL_NODE( a, b ) gkCreateIfNode<gkGameObject*, CMP_EQUALS>(m_tree, a, b)
#define REAL_GREATER_NODE( a, b ) gkCreateIfNode<gkScalar, CMP_GREATER>(m_tree, a, b)

#define IS_TRUE( a ) a->getIS_TRUE()
#define IS_FALSE( a ) a->getIS_FALSE()

typedef gkIfNode<bool, CMP_AND> BOOL_AND_NODE_TYPE;
typedef gkIfNode<bool, CMP_OR> BOOL_OR_NODE_TYPE;
typedef gkIfNode<gkGameObject*, CMP_EQUALS> PGAMEOBJ_EQUAL_NODE_TYPE;
typedef gkIfNode<int, CMP_EQUALS> INT_EQUAL_NODE_TYPE;
}
