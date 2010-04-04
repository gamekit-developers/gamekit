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

#include "nsCommon.h"
#include "nsMath.h"
#include "nsVariable.h"
#include "nsSingleton.h"


// ----------------------------------------------------------------------------
// Color Wheel   
enum nsColorScheme
{
    // primary
    NS_COL_RED = 0,
    NS_COL_YELLOW,
    NS_COL_BLUE,

    // secondary
    NS_COL_ORANGE,
    NS_COL_GREEN,
    NS_COL_PURPLE,

    // tertiary
    NS_COL_ORANGE_RED,
    NS_COL_YELLOW_ORANGE,
    NS_COL_YELLOW_GREEN,
    NS_COL_BLUE_GREEN,
    NS_COL_BLUE_PURPLE,
    NS_COL_RED_PURPLE,
    NS_COL_GREY,

    //end
    NS_COL_MAX,
};

// ----------------------------------------------------------------------------
// Socket declaration
class nsSocketType
{
public:
    enum Direction {
        In,
        Out,
    };

public:

    int                         m_index;
    utString                    m_name;
    Direction                   m_direction;
    NSrect                      m_rect;
    utString                    m_default;
    utString                    m_briefHelp;
    nsColorScheme               m_color;
    nsVariable::PropertyTypes   m_type;
};


// ----------------------------------------------------------------------------
class nsEnumItem
{
public:
    utString    m_name;
    int         m_value;
};
typedef utArray<nsEnumItem>             nsEnumItems;
typedef utArrayIterator<nsEnumItems>    nsEnumItemIterator;




// ----------------------------------------------------------------------------
// Variable declration
class nsVariableType
{
public:

    utString                    m_name;
    nsVariable::PropertyTypes   m_type;

    // values
    utString        m_value;
    nsEnumItems     m_enum;
    utString        m_briefHelp;
};


typedef utArray<nsVariableType*>        nsVariables;
typedef utArrayIterator<nsVariables>    nsVariableIterator;


// ----------------------------------------------------------------------------
// Node declaration
class nsNodeType
{
public:

    typedef utList<nsSocketType *>          Sockets;
    typedef utListIterator<Sockets>         SocketIterator;
    typedef utListReverseIterator<Sockets>  SocketReverseIterator;

public:

    ~nsNodeType();

    utString        m_typename;
    utString        m_groupname;
    NSvec2          m_size;
    int             m_id, m_groupId;
    Sockets         m_inputs, m_outputs;
    utString        m_briefHelp;
    nsColorScheme   m_color;
    nsVariables     m_variables;
};


// ----------------------------------------------------------------------------
// System type information
class nsNodeTypeInfo : public nsSingleton<nsNodeTypeInfo>
{
public:
    typedef utList<nsNodeType*>                     TypeList;
    typedef utListIterator<TypeList>                TypeIterator;
    typedef utHashTable<utHashedString, TypeList>   GroupTypeList;
    typedef utHashTableIterator<GroupTypeList>      GroupTypeIterator;


protected:
    TypeList        m_types;
    GroupTypeList   m_groups;

public:

    nsNodeTypeInfo();
    ~nsNodeTypeInfo();

    nsNodeType*                 findTypeInfo(int i);


    int                         getGroupSize(void)      {return (int)m_groups.size();}
    GroupTypeIterator           getGroupIterator(void)  {return GroupTypeIterator(m_groups);}
    TypeIterator                getTypeIterator(void)   {return TypeIterator(m_types);}
    TypeList&                   getTypes(void)          {return m_types;}


    // load types from file
    void                        parseTypes(const utString &path);

    NS_DECLARE_SINGLETON(nsNodeTypeInfo);
};

#endif//_nsNodeTypeInfo_h_
