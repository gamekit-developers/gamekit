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
#ifndef _nsNodeTree_h_
#define _nsNodeTree_h_

#include "nsCommon.h"
#include "nsNodeTypeInfo.h"
#include "nsNode.h"

// ----------------------------------------------------------------------------
class nsNodeTree
{
protected:
    nsNodes         m_nodes;
    nsString        m_name;
    nsString        m_groupName;

    bool            m_isGroup, m_open;
    NSrect          m_projection;
    NSvec2          m_size;


    // attached canvas 
    nsNodeCanvas    *m_client;



    // Root object for nodes within this tree
    nsString        m_attachedObject;


public:

    nsNodeTree(const nsString &name);
    ~nsNodeTree();

    NS_INLINE void              setName(const nsString &name)           {m_name = name;}
    NS_INLINE void              setGroup(bool val)                      {m_isGroup = val;}
    NS_INLINE void              setGroupName(const nsString &name)      {m_groupName = name;}
    NS_INLINE void              setAttachedName(const nsString &name)   {m_attachedObject = name;}


    NS_INLINE const nsString      &getName(void)                          {return m_name;}
    NS_INLINE const nsString      &getGroupName(void)                     {return m_groupName;}
    NS_INLINE bool                isGroup(void)                           {return m_isGroup;}
    NS_INLINE const nsString      &getAttachedName(void)                  {return m_attachedObject;}


    // save / load data
    NS_INLINE void                setOpen(bool v)                         {m_open = v;}
    NS_INLINE void                setSize(const NSvec2 &size)             {m_size = size;}
    NS_INLINE void                setProjection(const NSrect &proj)       {m_projection = proj;}
    NS_INLINE bool                isOpen(void)                            {return m_open;}
    NS_INLINE NSvec2              &getSize(void)                          {return m_size;}
    NS_INLINE NSrect              &getProjection(void)                    {return m_projection;}


    // Temporary canvas data (only valid as long as the editor is open) 
    NS_INLINE void                attachCanvas(nsNodeCanvas *cnvs)        {m_client = cnvs;}
    NS_INLINE nsNodeCanvas        *getAttachedCanvas(void)                {return m_client;}


    // node access
    NS_INLINE nsNodeIterator      getNodeIterator(void)                   {return nsNodeIterator(m_nodes);}
    NS_INLINE NSsize              getNodeCount(void)                      {return m_nodes.size();}


    nsNode  *createNode(nsNodeDef *nt);
    nsNode  *createCloneNode(nsNode *nd);

    void    deleteNode(nsNode *node);
    void    clear(void);

    // render list sorting 
    void    bringToFront(nsNode *node);
    void    bringToFront(nsNodes &list, nsNode *node);
};


#endif//_nsNodeTree_h_
