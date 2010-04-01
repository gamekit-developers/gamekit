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
#include "nsPropertyPage.h"
#include "nsNodeManager.h"
#include "nsTreeProperties.h"
#include "nsNodeProperties.h"
#include <wx/sizer.h>

// ----------------------------------------------------------------------------
NS_IMPLEMENT_SINGLETON(nsPropertyPage);


// ----------------------------------------------------------------------------
// Property Layout Manager
class nsPropertyManager : public wxPropertyGridManager
{
public:
    nsPropertyManager(wxWindow *parent, int id)
        :   wxPropertyGridManager(parent, id, wxPoint(0,0), wxDefaultSize,
                                  wxPGMAN_DEFAULT_STYLE | nsBorderDefault)
    {
    }
    virtual ~nsPropertyManager() {}

};


// ----------------------------------------------------------------------------
nsPropertyPage::nsPropertyPage(wxWindow *parent)
    :   wxPanel(parent, NS_WID_PROPERTY, wxPoint(0,0), nsDefaultSize, nsBorderNone)
{
    wxSizer *size = new wxBoxSizer(wxVERTICAL);
    m_manager = new nsPropertyManager(this, NS_WID_PROPERTY_DATA);

    m_tree = 0;
    m_default = 0;

    wxPanel *header = new wxPanel(this, wxID_ANY);
    header->SetSize(0, 24);

    size->Add(header,       wxSizerFlags(0).Expand().Border(wxALL,nsHeaderBorderSize));
    size->Add(m_manager,    wxSizerFlags(1).Expand().Border(wxALL,nsContentBorderSize));
    SetSizer(size);
    Layout();
}


// ----------------------------------------------------------------------------
nsPropertyPage::~nsPropertyPage()
{
}

// ----------------------------------------------------------------------------
void nsPropertyPage::initialize(void)
{
    if (m_default) return;

    m_default = new wxPropertyGridPage();
    m_manager->AddPage(wxEmptyString, wxNullBitmap, m_default);


    m_tree = new nsTreePropertyPage(m_manager);
    m_manager->AddPage(wxEmptyString, wxNullBitmap, m_tree);
    m_tree->createProperties();

    // create node types

    nsNodeTypeInfo::TypeList &types = nsNodeTypeInfo::getSingleton().getTypes();
    m_nodeTypes.resize(types.size(), 0);
    nsNodeTypeInfo::TypeIterator iter = nsNodeTypeInfo::TypeIterator(types);
    while (iter.hasMoreElements())
    {
        nsNodeType *type = iter.getNext();
        if (type->m_id >=0 && type->m_id < (int)types.size())
        {
            nsNodePropertyPage *props = new nsNodePropertyPage(m_manager, type);
            m_manager->AddPage(wxEmptyString, wxNullBitmap, props);
            props->createProperties();
            m_nodeTypes[type->m_id] = props;
        }
    }

    m_manager->SelectPage(m_default);

    Refresh();
}

// ----------------------------------------------------------------------------
void nsPropertyPage::treeEvent(nsTreeEvent &evt)
{
    if (evt.getId() == NS_TREE_CHANGED)
    {
        nsNodeTree *ntree = evt.ptr();
        if (ntree)
        {
            m_tree->setTree(ntree); m_tree->updateData();
            m_manager->SelectPage(m_tree);
        }
    }
    else if (evt.getId() == NS_TREE_ADD || evt.getId() == NS_TREE_SELECT)
    {
        nsNodeTree *ntree = evt.ptr();
        if (ntree)
        {
            m_tree->setTree(ntree);
            m_tree->updateData();
            m_manager->SelectPage(m_tree);
        }
    }
    else if (evt.getId() == NS_TREE_REMOVE || evt.getId() == NS_TREE_DESELECT)
    {
        m_tree->setTree(0);
        m_manager->SelectPage(m_default);
    }
}

// ----------------------------------------------------------------------------
void nsPropertyPage::nodeEvent(nsNodeEvent &evt)
{
    if (evt.getId() == NS_NODE_ADD || evt.getId() == NS_NODE_SELECT)
    {
        nsNode *node = evt.ptr();
        if (node)
        {
            nsNodeType *type = node->getType();

            if (type->m_id >=0 && type->m_id < (int)m_nodeTypes.size())
            {
                nsNodePropertyPage *page = m_nodeTypes[type->m_id];
                if (page)
                {
                    page->setNode(node);
                    m_manager->SelectPage(page);
                }
            }
        }
    }
    else if (evt.getId() == NS_NODE_REMOVE || evt.getId() == NS_NODE_DESELECT)
    {
        nsNode *node = evt.ptr();
        if (node)
        {
            nsNodeType *type = node->getType();
            if (type->m_id >=0 && type->m_id < (int)m_nodeTypes.size())
            {
                nsNodePropertyPage *page = m_nodeTypes[type->m_id];
                if (page)
                    page->setNode(0);
            }

            m_tree->setTree(node->getParent());
            m_manager->SelectPage(m_tree);
        }
        else
            m_manager->SelectPage(m_default);
    }
}

