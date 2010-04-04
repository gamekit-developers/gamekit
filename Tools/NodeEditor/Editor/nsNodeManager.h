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
#include "Utils/utTypes.h"
#include "Utils/utString.h"
#include "nsNodeTypeInfo.h"
#include "nsSingleton.h"
#include "nsVariable.h"


// ----------------------------------------------------------------------------
class nsSocket : public utListClass<nsSocket>::Link
{
public:
    typedef utList<nsSocket *> ToLinks;

protected:
    NSrect          m_rect, m_derrivedRect;
    nsSocketType    *m_type;
    nsNode          *m_parent;
    nsSocket        *m_from;
    ToLinks         m_tosockets;
    nsVariable      m_value;
    UTsize          m_index;

public:
    nsSocket(nsNode *nd, nsSocketType *st);
    ~nsSocket();

    bool canConnect(nsSocket *oth)
    {
        UT_ASSERT(oth);
        return  m_parent != oth->m_parent &&  getType()->m_type == oth->getType()->m_type;
    }

    bool                isConnectedOut(void)            {return !m_tosockets.empty();}
    bool                isConnected(void)               {return m_from != 0;}
    nsSocket            *getSocketLink(void)            {return m_from;}

    // sorting utils
    nsNode              *getParent(void)                {return m_parent;}
    UTsize              getIndex(void)                  {return m_index;}
    void                setIndex(UTsize i)              {m_index = i;}

    nsVariable          &getValue(void)                 {return m_value;}
    void                setValue(const nsVariable &v)   {m_value = v;}

    nsSocketType        *getType(void)                  {return m_type;}
    bool                isOutput(void)                  {return m_type->m_direction == nsSocketType::Out;}
    bool                isInput(void)                   {return m_type->m_direction == nsSocketType::In;}
    NSrect              &getDerrivedRect(void)          {return m_derrivedRect;}
    NSrect              &getRect(void)                  {return m_rect;}


    void                unlink(void);
    nsSocket            *clone(nsNode *newParent);
    void                updateFromParent(void);
    void                connect(nsSocket *oth);
};



// ----------------------------------------------------------------------------
class nsNodeData : public utListClass<nsNodeData>::Link
{
protected:

    nsVariableType  *m_type;
    nsVariable       m_value;
    nsNode          *m_parent;


public:
    nsNodeData(nsNode *nd, nsVariableType *type);
    ~nsNodeData();

    // sorting utils
    nsNode              *getParent(void)                {return m_parent;}
    nsVariable          &getValue(void)                 {return m_value;}
    void                setValue(const nsVariable &v)   {m_value = v;}

    nsVariableType      *getType(void)                  {return m_type;}
    nsNodeData          *clone(nsNode *newParent);
};


// ----------------------------------------------------------------------------
class nsNode
{
public:
    typedef utListClass<nsSocket>   Sockets;
    typedef utListClass<nsNodeData> Data;

protected:
    NSrect          m_rect;
    nsNodeType      *m_type;
    nsNodeTree      *m_tree;

    Sockets         m_inputs;
    Sockets         m_outputs;
    Data            m_data;
    nsNodeState     m_state;
    bool            m_editOutputs;

public:

    nsNode(nsNode *cpy, nsNodeTree *tree=0);
    nsNode(nsNodeTree *tree, nsNodeType *nt);
    ~nsNode();


    const NSrect    &getRect(void)              {return m_rect;}
    nsNodeType      *getType(void)              {UT_ASSERT(m_type); return m_type;}
    nsNodeTree      *getParent(void)            {UT_ASSERT(m_tree); return m_tree;}

    nsSocket        *getInput(UTsize i)         {return ((i < m_inputs.size()  && i >=0 ) ? m_inputs.at(i)  : 0);}
    nsSocket        *getOutput(UTsize i)        {return ((i < m_outputs.size() && i >=0 ) ? m_outputs.at(i) : 0);}

    nsSocket        *getFirstInput(void)        {if (m_inputs.empty()) return 0; return m_inputs.begin();}
    nsSocket        *getFirstOutput(void)       {if (m_outputs.empty()) return 0; return m_outputs.begin();}
    nsNodeData      *getFirstVariable(void)     {if (m_data.empty()) return 0; return m_data.begin(); }

    bool            getEditOutputs(void)        {return m_editOutputs;}
    void            setEditOutputs(bool v)      {m_editOutputs = v;}

    nsNodeState     getState(void)              {return m_state;}
    void            setState(nsNodeState st)    {m_state = st;}

    void            unlink(void);
    nsNode          *clone(nsNodeTree *newTree = 0);
    void            translate(NSfloat x, NSfloat y);
    void            setPosition(NSfloat x, NSfloat y);
};

// ----------------------------------------------------------------------------
class nsNodeTree
{
protected:
    nsNodes         m_nodes;
    utString        m_name, m_groupName;
    bool            m_isGroup;
    NSrect          m_projection;
    NSvec2          m_limits;
    NSvec2          m_size;
    nsNodeCanvas    *m_client;


public:

    nsNodeTree(const utString &name);
    ~nsNodeTree();

    void                setName(const utString &name)           {m_name = name;}
    const utString      &getName(void)                          {return m_name;}
    void                setGroup(bool val)                      {m_isGroup = val;}
    bool                isGroup(void)                           {return m_isGroup;}
    void                setGroupName(const utString &name)      {m_groupName = name;}
    const utString      &getGroupName(void)                     {return m_groupName;}


    NSvec2              &getSize(void)                          {return m_size;}
    void                setSize(const NSvec2 &size)             {m_size = size;}

    NSrect              &getProjection(void)                    {return m_projection;}
    void                setProjection(const NSrect &proj)       {m_projection = proj;}
    nsNodeIterator      getNodeIterator(void)                   {return nsNodeIterator(m_nodes);}

    void                attachCanvas(nsNodeCanvas *cnvs)        {m_client = cnvs;}
    nsNodeCanvas        *getAttachedCanvas(void)                {return m_client;}
    UTsize              getNodeCount(void)                      {return m_nodes.size();}


    void                deleteNode(nsNode *node);
    nsNode              *createNode(nsNodeType *nt);
    nsNode              *createCloneNode(nsNode *nd);
    void                clear(void);

    void                bringToFront(nsNode *node);
    void                bringToFront(nsNodes &list, nsNode *node);
};


// ----------------------------------------------------------------------------
class nsNodeManager : public nsSingleton<nsNodeManager>
{
protected:
    nsTrees         m_trees;
public:
    nsNodeManager();
    ~nsNodeManager();

    void            deleteTree(nsNodeTree *tree);
    nsNodeTree      *createTree(const utString &name = "Node Tree");

    nsTreeIterator  getTreeIterator(void) {return nsTreeIterator(m_trees);}

    NS_DECLARE_SINGLETON(nsNodeManager);
};


#endif//_nsNodeManager_h_
