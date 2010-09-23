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
#ifndef _gkMapNode_h_
#define _gkMapNode_h_

#include "gkLogicNode.h"

template<typename K, typename V>
class gkMapNode : public gkLogicNode
{
public:

	enum
	{
		UPDATE,
		INPUT,
		MAPPING,
		OUTPUT,
		HAS_OUTPUT
	};

	typedef typename std::map<K, V> MAP;

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(MAPPING, MAP);
	DECLARE_SOCKET_TYPE(INPUT, K);
	DECLARE_SOCKET_TYPE(OUTPUT, V);
	DECLARE_SOCKET_TYPE(HAS_OUTPUT, bool);


	gkMapNode(gkLogicTree* parent, size_t id)
		: gkLogicNode(parent, id)
	{
		ADD_ISOCK(UPDATE, true);
		ADD_ISOCK(INPUT, K());
		ADD_ISOCK(MAPPING, MAP());
		ADD_OSOCK(OUTPUT, V());
		ADD_OSOCK(HAS_OUTPUT, false);
	}

	virtual ~gkMapNode() {}

	bool evaluate(gkScalar tick)
	{
		return GET_SOCKET_VALUE(UPDATE);
	}

	void update(gkScalar tick)
	{
		K key = GET_SOCKET_VALUE(INPUT);

		if (m_currentKey != key)
		{
			MAP& mapping = GET_SOCKET_REF_VALUE(MAPPING);

			typename MAP::const_iterator it = mapping.find(key);

			if (it == mapping.end())
			{
				SET_SOCKET_VALUE(HAS_OUTPUT, false);
			}
			else
			{
				SET_SOCKET_VALUE(OUTPUT, it->second);

				SET_SOCKET_VALUE(HAS_OUTPUT, true);
			}

			m_currentKey = key;
		}
	}

private:

	K m_currentKey;
};

#endif//_gkMapNode_h_
