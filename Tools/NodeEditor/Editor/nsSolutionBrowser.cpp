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
#include "nsSolutionBrowser.h"
#include "nsWorkspace.h"
#include "nsPropertyPage.h"
#include "nsNodeManager.h"
#include "nsMainWindow.h"
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/menu.h>


NS_IMPLEMENT_SINGLETON(nsSolutionBrowser);



// local events
enum nsSolutionBrowserEvents
{
	// Project
	SBE_ADD,

	// Tree
	SBE_OPEN,
	SBE_REMOVE,
	SBE_RENAME,

	// Node
};




// local tree items
class nsTreeItem : public wxTreeItemData
{
public:
	nsTreeItem()
		:   m_node(0), m_tree(0)
	{
	}

	nsNode*      m_node;
	nsNodeTree*  m_tree;
};



// Event tables
BEGIN_EVENT_TABLE( nsSolutionBrowser, wxPanel )
	EVT_TREE_SEL_CHANGED(NS_WID_SOLUTION_DATA,      nsSolutionBrowser::itemChangeEvent)
	EVT_TREE_ITEM_ACTIVATED(NS_WID_SOLUTION_DATA,   nsSolutionBrowser::itemActivatedEvent)
	EVT_TREE_ITEM_MENU(NS_WID_SOLUTION_DATA,        nsSolutionBrowser::treeOpenMenu)
	EVT_TREE_END_LABEL_EDIT(NS_WID_SOLUTION_DATA,   nsSolutionBrowser::labelEditEvent)

	// menus
	EVT_MENU(SBE_ADD,       nsSolutionBrowser::addTreeEvent)
	EVT_MENU(SBE_REMOVE,    nsSolutionBrowser::removeTreeEvent)
	EVT_MENU(SBE_OPEN,      nsSolutionBrowser::openTreeEvent)
	EVT_MENU(SBE_RENAME,    nsSolutionBrowser::labelMenuEditEvent)

END_EVENT_TABLE()



nsSolutionBrowser::nsSolutionBrowser(wxWindow* parent)
	:   wxPanel(parent, NS_WID_SOLUTION, wxDefaultPosition, wxSize(200, 200), nsBorderNone)

{
	wxSizer* size = new wxBoxSizer(wxVERTICAL);
	m_tree = new wxTreeCtrl(this, NS_WID_SOLUTION_DATA, wxDefaultPosition, wxDefaultSize,
	                        wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | nsBorderDefault);
	m_tree->AddRoot("Project");

	wxPanel* header = new wxPanel(this);
	header->SetSize(0, 24);

	size->Add(header,   wxSizerFlags(0).Expand().Border(wxALL, nsHeaderBorderSize));
	size->Add(m_tree,   wxSizerFlags(1).Expand().Border(wxALL, nsContentBorderSize));

	SetSizer(size);
	Layout();
}


nsSolutionBrowser::~nsSolutionBrowser()
{
}


// access to the current tree
nsNodeTree* nsSolutionBrowser::getSelectedTree(void)
{
	nsTreeItem* item = (nsTreeItem*)m_tree->GetItemData(m_tree->GetSelection());
	if (item && item->m_tree)
		return item->m_tree;
	return 0;

}


void nsSolutionBrowser::treeEvent(nsTreeEvent& evt)
{
	if (evt.getId() == NS_TREE_ADD)
	{
		nsTreeItem* item = new nsTreeItem();
		item->m_tree = evt.ptr();


		wxTreeItemId root = m_tree->GetRootItem();
		m_tree->AppendItem(root, item->m_tree->getName().c_str(), -1, -1, item);
		m_tree->Expand(root);
	}
	else if (evt.getId() == NS_TREE_REMOVE && evt.getCaller() != this)
	{
		nsNodeTree* ntree = evt.ptr();


		// remove this tree
		wxTreeItemIdValue cookie;
		wxTreeItemId id = m_tree->GetFirstChild(m_tree->GetRootItem(), cookie);

		int max = m_tree->GetCount(), i = 0;
		for (; id.IsOk() && i < max; ++i)
		{
			nsTreeItem* item = (nsTreeItem*)m_tree->GetItemData(id);
			if (item && item->m_tree == ntree)
			{
				m_tree->DeleteChildren(item->GetId());
				m_tree->Delete(item->GetId());
				return;
			}
			id = m_tree->GetNextSibling(id);
		}
	}
}



void nsSolutionBrowser::labelEditEvent(wxTreeEvent& evt)
{
	nsTreeItem* item = (nsTreeItem*)m_tree->GetItemData(m_tree->GetSelection());
	if (item && item->m_tree)
	{
		nsNodeTree* ntree = item->m_tree;
		ntree->setName((const char*)evt.GetLabel().mb_str());

		nsTreeEvent change(NS_TREE_CHANGED, this, ntree);

		nsWorkspace::getSingleton().treeEvent(change);
		nsPropertyPage::getSingleton().treeEvent(change);
	}
}


void nsSolutionBrowser::labelMenuEditEvent(wxCommandEvent& evt)
{
	nsTreeItem* item = (nsTreeItem*)m_tree->GetItemData(m_tree->GetSelection());
	if (item && item->m_tree)
		m_tree->EditLabel(item->GetId());
}


void nsSolutionBrowser::addTreeEvent(wxCommandEvent& evt)
{
	// new tree
	nsNodeTree* tree = nsNodeManager::getSingleton().createTree();

	nsTreeEvent add(NS_TREE_ADD, this, tree);

	// send events
	treeEvent(add);

	nsWorkspace::getSingleton().treeEvent(add);
	nsPropertyPage::getSingleton().treeEvent(add);
}



void nsSolutionBrowser::removeTreeEvent(wxCommandEvent& evt)
{
	nsTreeItem* item = (nsTreeItem*)m_tree->GetItemData(m_tree->GetSelection());
	if (item && item->m_tree)
	{
		nsNodeTree* ntree = item->m_tree;

		// tell other windows
		nsTreeEvent add(NS_TREE_REMOVE, this, ntree);
		nsWorkspace::getSingleton().treeEvent(add);
		nsPropertyPage::getSingleton().treeEvent(add);


		m_tree->DeleteChildren(item->GetId());
		m_tree->Delete(item->GetId());

		nsNodeManager::getSingleton().deleteTree(ntree);
		evt.Skip();
	}
}


void nsSolutionBrowser::openTreeEvent(wxCommandEvent& evt)
{
	nsTreeItem* item = (nsTreeItem*)m_tree->GetItemData(m_tree->GetSelection());
	if (item && item->m_tree)
	{
		nsNodeTree* ntree = item->m_tree;

		// tell workspace
		nsTreeEvent add(NS_TREE_ADD, this, ntree);
		nsWorkspace::getSingleton().treeEvent(add);

		evt.Skip();
	}
}


void nsSolutionBrowser::treeOpenMenu(wxTreeEvent& evt)
{
	// tree context menu
	if (evt.GetItem() == m_tree->GetRootItem())
	{
		wxMenu menu;
		menu.Append(SBE_ADD, "New Tree");
		PopupMenu(&menu);
	}
	else
	{
		// tree item menu
		nsTreeItem* item = (nsTreeItem*)m_tree->GetItemData(evt.GetItem());
		if (item && item->m_tree)
		{
			// just incase
			m_tree->SelectItem(evt.GetItem());

			wxMenu menu, *addMenu;
			menu.Append(SBE_OPEN, "Open");
			menu.AppendSeparator();
			addMenu = new wxMenu;
			nsMainWindow::getSingleton().makeNodeMenu(this, addMenu);
			menu.AppendSubMenu(addMenu, "Add");

			menu.Append(SBE_RENAME, "Rename");
			menu.AppendSeparator();
			menu.Append(SBE_REMOVE, "Remove Tree");
			PopupMenu(&menu);
		}
	}
}


void nsSolutionBrowser::itemChangeEvent(wxTreeEvent& evt)
{
	// called whe the selection changes
	nsTreeItem* item = (nsTreeItem*)m_tree->GetItemData(evt.GetItem());
	if (item && item->m_tree)
	{
		nsNodeTree* ntree = item->m_tree;

		// tell workspace
		nsTreeEvent sel(NS_TREE_SELECT, this, ntree);
		nsPropertyPage::getSingleton().treeEvent(sel);
		evt.Skip();
	}
}



void nsSolutionBrowser::itemActivatedEvent(wxTreeEvent& evt)
{
	// called whe the selection is double clicked

	nsTreeItem* item = (nsTreeItem*)m_tree->GetItemData(evt.GetItem());
	if (item && item->m_tree)
	{
		nsNodeTree* ntree = item->m_tree;

		// tell workspace
		nsTreeEvent add(NS_TREE_ADD, this, ntree);
		nsWorkspace::getSingleton().treeEvent(add);

		evt.Skip();
	}
}
