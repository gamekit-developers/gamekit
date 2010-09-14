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
#ifndef _nsNodeTypeInfo_h_
#define _nsNodeTypeInfo_h_

#include <wx/mstream.h>
#include "nsCommon.h"
#include "nsMath.h"
#include "nsSingleton.h"

#include "nsColorPalette.h"
#include "nsNodeEnums.h"
#include "nsData.h"



// System type information
class nsNodeTypeInfo : public nsSingleton<nsNodeTypeInfo>
{
public:
	typedef utList<nsNodeDef*>                     GroupList;
	typedef utListIterator<GroupList>               GroupListIterator;
	typedef utHashTable<utIntHashKey, GroupList>    GroupMap;
	typedef utHashTableIterator<GroupMap>           GroupIterator;


protected:
	nsNodeListClass     m_types;
	GroupMap            m_groups;


	void addType(nsNodeDef* ndef);

public:

	nsNodeTypeInfo();
	~nsNodeTypeInfo();

	nsNodeDef*                   findTypeInfo(int i);
	nsNodeDef*                   findTypeInfo(const nsString& name);
	nsString                    getGroupName(int in);


	int                         getGroupSize(void)      {return (int)m_groups.size();}
	GroupIterator               getGroupIterator(void)  {return GroupIterator(m_groups);}
	nsNodeDefIterator           getTypeIterator(void)   {return nsNodeDefIterator(m_types);}
	nsNodeListClass&             getTypes(void)         {return m_types;}

	void registerTypes(void);

	NS_DECLARE_SINGLETON(nsNodeTypeInfo);
};

#endif//_nsNodeTypeInfo_h_
