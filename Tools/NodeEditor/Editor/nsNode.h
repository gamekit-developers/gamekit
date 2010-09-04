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
#ifndef _nsNode_h_
#define _nsNode_h_

#include "nsCommon.h"
#include "nsNodeTypeInfo.h"
#include "nsSocket.h"
#include "nsNodeTree.h"


class nsNodeDef : public nsNodeListClass::Link
{
protected:
    nsNodeTypes         m_type;
    nsGroupTypes        m_group;
    nsString            m_name;
    nsString            m_docStr;
    nsColorPalette      m_palette;
    nsSocketListClass   m_inputs;
    nsSocketListClass   m_outputs;
    NSvec2              m_size;
    int                 m_id;

public:

    nsNodeDef();
    nsNodeDef(nsNodeTypes type, nsGroupTypes group, const nsString &name, const nsString& docs = "");


    virtual ~nsNodeDef();

    NS_INLINE nsNodeTypes           getType(void)       const           {return m_type;}
    NS_INLINE nsGroupTypes          getGroup(void)      const           {return m_group;}
    NS_INLINE const nsString        &getName(void)      const           {return m_name;}
    NS_INLINE nsColorPalette        &getPalette(void)                   {return m_palette;}
    NS_INLINE const NSvec2          &getSize(void)      const           {return m_size;}
    NS_INLINE const nsString        &getDocStr(void)    const           {return m_docStr;}
    NS_INLINE int                   getId(void)                         {return m_id;}


    NS_INLINE void                  setType(nsNodeTypes v)              {m_type = v;}
    NS_INLINE void                  setGroup(nsGroupTypes v)            {m_group = v;}
    NS_INLINE void                  setName(const nsString &v)          {m_name = v;}
    NS_INLINE void                  setPalette(const nsColorPalette &v) {m_palette = v;}
    NS_INLINE void                  setDocStr(const nsString &v)        {m_docStr = v;}
    NS_INLINE void                  setId(int v)                        {m_id = v;}


    NS_INLINE nsSocketListClass     &getInputs(void)                    {return m_inputs;}
    NS_INLINE nsSocketListClass     &getOutputs(void)                   {return m_outputs;}

    NS_INLINE nsSocketDefIterator   getInputIterator(void)              {return nsSocketDefIterator(m_inputs);}
    NS_INLINE nsSocketDefIterator   getOutputIterator(void)             {return nsSocketDefIterator(m_outputs);}
    


    void addInput(nsSocketDef *def);
    void addOutput(nsSocketDef *def);


    void alignInputs(nsSocketAlignment align);
    void alignOutputs(nsSocketAlignment align);


    // custom node data
    virtual nsNodeData* getData(void) = 0;
	virtual void edit(nsNode *nd) {}
};


// ----------------------------------------------------------------------------
class nsNode
{
protected:

    NSrect          m_rect;
    // definition 
    nsNodeDef       *m_type;
    // parent 
    nsNodeTree      *m_tree;
    nsSocketClass   m_inputs;
    nsSocketClass   m_outputs;
    nsNodeState     m_state;
    nsString        m_attachedObject;
    nsNodeData      *m_data;


public:
    nsNode(nsNode *cpy, nsNodeTree *tree=0);
    nsNode(nsNodeTree *tree, nsNodeDef *nt);
    ~nsNode();


    NS_INLINE const NSrect    &getRect(void)                          {return m_rect;}
    NS_INLINE nsNodeDef       *getType(void)                          {NS_ASSERT(m_type); return m_type;}
    NS_INLINE nsNodeTree      *getParent(void)                        {NS_ASSERT(m_tree); return m_tree;}
    NS_INLINE nsSocket        *getInput(NSsize i)                     {return ((i < m_inputs.size()  && i >=0 ) ? m_inputs.at(i)  : 0);}
    NS_INLINE nsSocket        *getOutput(NSsize i)                    {return ((i < m_outputs.size() && i >=0 ) ? m_outputs.at(i) : 0);}
    NS_INLINE nsSocket        *getFirstInput(void)                    {if (m_inputs.empty()) return 0; return m_inputs.begin();}
    NS_INLINE nsSocket        *getFirstOutput(void)                   {if (m_outputs.empty()) return 0; return m_outputs.begin();}

    // info about the current game object.
    NS_INLINE void            setAttachedName(const nsString &name)   {m_attachedObject = name;}
    NS_INLINE const nsString  &getAttachedName(void)                  {return m_attachedObject;}


    // activation state
    NS_INLINE nsNodeState       getState(void)                          {return m_state;}
    NS_INLINE void              setState(nsNodeState st)                {m_state = st;}

    NS_INLINE nsNodeData        *getData(void)                          {return m_data;}

    NS_INLINE NSrect            getEditRect(void)
	{
		return NSrect((m_rect.getRight() - (nsNodeEditSize + nsNodeEditOffset)), m_rect.y + nsNodeEditOffset, nsNodeEditSize, nsNodeEditSize);
	}

        
    void            unlink(void);
    nsNode          *clone(nsNodeTree *newTree = 0);

    void            translate(NSfloat x, NSfloat y);
    void            setPosition(NSfloat x, NSfloat y);

    NS_INLINE void  translate(const NSvec2 &v)      { translate(v.x, v.y); }
    NS_INLINE void  setPosition(const NSvec2 &v)    { setPosition(v.x, v.y); }
};


#define NS_DECLARE_NODE(cls)\
class cls  : public nsNodeDef { \
public: \
    cls();\
    virtual ~cls(); \
	nsNodeData* getData(void); \
	void edit(nsNode *node);\
};


#define NS_IMPLEMENT_NODE(cls, editor, data)\
	cls::~cls() {}\
	nsNodeData* cls::getData(void) {return new data(this); }\
	void cls::edit(nsNode *node)\
	{\
		editor dia(nsMainWindow::getSingletonPtr(), node);\
		if (dia.ShowModal() == wxID_OK)\
			dia.Flush();\
	}\
	cls::cls()



#endif//_nsNode_h_
