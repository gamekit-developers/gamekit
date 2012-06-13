/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr

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

#include "StdAfx.h"
#include "luUtils.h"
#include "luProjPanel.h"
#include "luMainFrame.h"
#include "Lib/liUtils.h"

#define ROOT_ITEM_NAME	"Project"
#define PANEL_NAME		"Project"

#define TREECTRL_STYLE	wxTR_DEFAULT_STYLE


BEGIN_EVENT_TABLE(luProjTree, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(ID_PROJ_TREE, luProjTree::OnSelChanged)
	//EVT_TREE_ITEM_ACTIVATED(ID_PROJ_TREE, luProjTree::OnItemActivated)
	EVT_TREE_ITEM_MENU(ID_PROJ_TREE, luProjTree::OnItemMenu)
	//EVT_CHAR(luProjTree::OnKeyChar)
	//EVT_KEY_DOWN(luProjTree::OnKeyChar)
	EVT_TREE_KEY_DOWN(ID_PROJ_TREE, luProjTree::OnTreeKeyDown)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(luInspPanel, wxPanel)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(luProjPanel, wxPanel)
	EVT_CHOICE(ID_PROJ_OBJ_TYPE_CHICE, luProjPanel::OnObjTypeChanged)
	EVT_TEXT_ENTER(ID_PROJ_OBJ_SEARCH_TEXT, luProjPanel::OnObjSearchEnter)
	EVT_TEXT(ID_PROJ_OBJ_SEARCH_TEXT, luProjPanel::OnObjSearchEnter)
	EVT_LIST_ITEM_SELECTED(ID_PROJ_LIST, luProjPanel::OnObjListChanged)
END_EVENT_TABLE()


//--

luProjTree::luProjTree(wxWindow* parent) 
	:	wxTreeCtrl(parent, ID_PROJ_TREE, wxDefaultPosition, wxDefaultSize, TREECTRL_STYLE)
{
	wxTreeItemId item = AddRoot(ROOT_ITEM_NAME);
}

luProjTree::~luProjTree()
{
}


void luProjTree::OnKeyChar(wxKeyEvent& event)
{
	int key = event.GetKeyCode();
	if (key == WXK_DELETE)
	{
	    wxCommandEvent evt(wxEVT_NULL, ID_PROJ_REMOVE_FILE);
		wxTheApp->GetTopWindow()->GetEventHandler()->ProcessEvent(evt);
		gkPrintf("delete");
	}
	//gkPrintf("key: %d", key);
}


void luProjTree::OnTreeKeyDown(wxTreeEvent& event)
{
	//gkPrintf("treekey");
}

void luProjTree::OnItemMenu(wxTreeEvent& event)
{
    wxTreeItemId itemId = event.GetItem();

    wxPoint clientpt = event.GetPoint();
    wxPoint screenpt = ClientToScreen(clientpt);


    showMenu(itemId, clientpt);
    event.Skip();
}

void luProjTree::OnItemActivated(wxTreeEvent& event)
{
    wxTreeItemId itemId = event.GetItem();
    ItemData* item = (ItemData*)GetItemData(itemId);

	luMainFrame* frame = getLuMainFrame();

	if (!item || !frame) return;
	int data = item->GetData();

	wxString name = item->GetName();
}

void luProjTree::OnSelChanged(wxTreeEvent& event)
{
}

void luProjTree::showMenu(wxTreeItemId id, const wxPoint& pt)
{
	wxMenu menu;
    menu.Append(ID_PROJ_ACTIVE_FILE, "&Open");
	menu.Append(ID_PROJ_REMOVE_FILE, "&Remove");

	int data = getSelectedItemData();

	if (data == ITEM_LUA)
		menu.Append(ID_PROJ_STARTUP_FILE, "&StartUp");

    PopupMenu(&menu, pt);
}


luProjTree::ItemData* luProjTree::_getSelectedItemData()
{
	if (GetFocusedItem().IsOk())
		return (ItemData*)GetItemData(GetFocusedItem());

	return NULL;
}

int luProjTree::getSelectedItemData()
{
	ItemData* itemData = _getSelectedItemData();
	return itemData ? itemData->GetData() : 0;
}

wxString luProjTree::getSelectedItemName()
{
	ItemData* itemData = _getSelectedItemData();
	return itemData ? itemData->GetName() : "";
}


wxTreeItemId luProjTree::findItemByData(const wxTreeItemId& parent, const wxString& data)
{
	if (!parent.IsOk()) return parent;

	ItemData* item = dynamic_cast<ItemData*>(GetItemData(parent));
	if (item && item->GetName() == data)
		return parent;

	if (item) gkPrintf("%s", (const char*)item->GetName());

	if (HasChildren(parent))
	{
		wxTreeItemIdValue cookie;
		wxTreeItemId item = GetFirstChild(parent, cookie);
		while (item.IsOk())
		{
			wxTreeItemId find = findItemByData(item, data); //find is item self or children.
			if (find.IsOk()) return find;

			item = GetNextChild(item, cookie);
		}
	}

	return wxTreeItemId(); //not found
}




wxTreeItemId luProjTree::findItemByText(const wxTreeItemId& parent, const wxString& text)
{
	if (!parent.IsOk()) return parent;

	if (GetItemText(parent) == text)
		return parent;

	if (HasChildren(parent))
	{
		wxTreeItemIdValue cookie;
		wxTreeItemId item = GetFirstChild(parent, cookie);
		while (item.IsOk())
		{
			wxTreeItemId find = findItemByText(item, text); //find is item self or children.
			if (find.IsOk()) return find;

			item = GetNextChild(item, cookie);
		}
	}

	return wxTreeItemId(); //not found
}


//-- insp

luInspPanel::luInspPanel(wxWindow* parent) 
	:	wxPanel(parent, ID_PROJ_INSP_WIN),
		m_search(NULL),
		m_list(NULL),
		m_choice(NULL)
{


	wxFlexGridSizer* sizer;
	sizer = new wxFlexGridSizer( 2, 1, 0, 0 );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 1 );
	sizer->SetFlexibleDirection( wxBOTH );
	sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* top;
	top = new wxFlexGridSizer( 1, 2, 0, 0 );
	top->AddGrowableCol( 1 );
	top->SetFlexibleDirection( wxBOTH );
	top->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_search = new wxSearchCtrl(this, ID_PROJ_OBJ_SEARCH_TEXT);


#ifndef __WXMAC__
	m_search->ShowSearchButton( true );
#endif

	m_choice = new wxChoice(this, ID_PROJ_OBJ_TYPE_CHICE);
	m_choice->SetMinSize(wxSize(80,-1));
	top->Add(m_choice, 0, wxALL, 5);
	top->Add(m_search, 1, wxALL | wxEXPAND, 5);

	m_choice->Append("All", (void*)-10); //don't use -1
	for (int i = 0; i <= GK_SKELETON; i++)
	{
		m_choice->Append(getLuObjectTypeName((gkGameObjectTypes)i), (void*)i);
	}


	//-- list

	m_list = new wxListCtrl(this, ID_PROJ_LIST, wxDefaultPosition, wxDefaultSize,
		wxLC_REPORT | wxLC_SINGLE_SEL | wxBORDER_THEME);

	wxListItem itemCol;
    itemCol.SetText(_T("Name"));
    m_list->InsertColumn(0, itemCol);

    itemCol.SetText(_T("Type"));
    itemCol.SetAlign(wxLIST_FORMAT_CENTRE);
    m_list->InsertColumn(1, itemCol);

	sizer->Add( top, 1, wxALIGN_TOP|wxEXPAND, 5 );
	sizer->Add( m_list, 1, wxALL|wxEXPAND, 5 );

	SetSizer( sizer );
	Layout();
}



//-- panel

luProjPanel::luProjPanel(wxWindow *parent)
	:	wxPanel(parent, ID_PROJ_PANEL, wxDefaultPosition, wxSize(200, 200)),
		m_tree(NULL),
		m_splitter(NULL),
		m_insp(NULL),
		m_fontNormal(NULL),
		m_fontBold(NULL)
{
	//m_fontNormal = new wxFont(*wxNORMAL_FONT);
	m_fontNormal = new wxFont(10, wxDEFAULT, wxNORMAL, wxNORMAL);
	m_fontBold = new wxFont(m_fontNormal->Bold());

	m_splitter = new wxSplitterWindow(this);
	m_splitter->SetSashGravity(1.0);

	m_tree = new luProjTree(m_splitter);

	m_insp = new luInspPanel(m_splitter);

	m_splitter->SplitHorizontally(m_tree, m_insp);

	addControlToPanel(this, m_splitter, PANEL_NAME);
}

luProjPanel::~luProjPanel()
{
	SAFE_DELETE(m_fontNormal);
	SAFE_DELETE(m_fontBold);
}

void luProjPanel::setItemFont(wxTreeItemId item, bool bold)
{
	if (!item.IsOk()) return;

	m_tree->SetItemFont(item, bold ? *m_fontBold : *m_fontNormal);
}

wxTreeItemId luProjPanel::addFile(const wxString& path, int data)
{
	wxTreeItemId root = getRootItem();
	if (!root.IsOk()) return root;

	wxString name = wxFileName(path).GetFullName(); //strip dir

	wxTreeItemId item = m_tree->AppendItem(root, name, -1, -1, new luProjTree::ItemData(path, data));
	setItemFont(item, false);

	return item;
}

wxTreeItemId luProjPanel::addBlendFile(const wxString& path, gkBlendFile* blend)
{
	wxTreeItemId parent = addFile(path, ITEM_BLEND);
	if (parent.IsOk())
	{

		const gkBlendFile::Scenes& scenes = blend->getScenes();
		for (size_t i = 0; i < scenes.size(); i++)
		{
			gkScene* scene = scenes[i];
			m_tree->AppendItem(parent, scene->getName(), -1, -1, new luProjTree::ItemData(scene->getName(), ITEM_SCENE));
		}
	}

	return parent;
}

bool luProjPanel::setStartupFile(const wxString& path, bool enable)
{
	if (!m_tree) return false;

	wxTreeItemId item = getFile(path);
	if (!item.IsOk()) return false;

	setItemFont(item, enable);

	return true;
}

bool luProjPanel::removeFile(const wxString& path)
{
	if (!m_tree) return false;

	wxTreeItemId item = m_tree->findItemByData(path);
	if (!item.IsOk()) return false;

	m_tree->Delete(item);

	return true;
}


bool luProjPanel::setProjectName(const wxString& name)
{
	wxTreeItemId root = getRootItem();
	if (!root.IsOk()) return false;

	m_tree->SetItemText(root, name);

	return true;
}

void luProjPanel::clearTreeItems()
{
	wxTreeCtrl* tree = getTreeCtrl();
	if (!tree) return;

	tree->DeleteAllItems();
	tree->AddRoot(ROOT_ITEM_NAME);
}

//-- 

void luProjPanel::addListItem(const wxString& name, const wxString& type)
{
	wxListCtrl *list = getListCtrl();
	int i = list->GetItemCount();
	i = list->InsertItem(i, name);
	list->SetItem(i, 1, type);
}


void luProjPanel::clearListItems()
{
	wxListCtrl* list = getListCtrl();
	if (!list) return;
	list->DeleteAllItems();
}


void luProjPanel::OnObjTypeChanged(wxCommandEvent& event)
{
	wxChoice* choice = getChoice();
	int sel = choice->GetCurrentSelection();
	intptr_t type = (intptr_t)choice->GetClientData(sel);
	//gkPrintf("%d %d", sel, type);

}

void luProjPanel::OnObjSearchEnter(wxCommandEvent& event)
{
	wxSearchCtrl* search = getSearch();
	wxString text = search->GetValue().MakeLower();

	wxListCtrl* list = getListCtrl();
	for (int i = 0; i < list->GetItemCount(); i++)
	{
		if (list->GetItemText(i).MakeLower().find(text) != wxString::npos)
		{
			list->SetItemState(i, wxLIST_STATE_SELECTED|wxLIST_STATE_FOCUSED, wxLIST_STATE_SELECTED|wxLIST_STATE_FOCUSED);
			return;
		}
	}

	wxBell();
}


void luProjPanel::OnObjListChanged(wxListEvent& event)
{
	wxString name = event.GetText();
	wxString type = getListCtrl()->GetItemText(event.GetIndex(), 1);

	getLuMainFrame()->selectGameObject(name);		
}