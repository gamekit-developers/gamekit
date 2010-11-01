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

#ifndef _luProjPanel_h_
#define _luProjPanel_h_



class luProjTree : public wxTreeCtrl
{
public:
	class ItemData : public wxTreeItemData
	{
		wxString m_name;
		int m_data;
	public:
		ItemData(const wxString& name, int data = 0) : m_name(name), m_data(data) {}
		wxString GetName() { return m_name; }
		int GetData() { return m_data; }
	};

protected:

	void showMenu(wxTreeItemId id, const wxPoint& pt);

	void OnKeyChar(wxKeyEvent& event);
	void OnContextMenu(wxContextMenuEvent& event);
    void OnItemMenu(wxTreeEvent& event);
	void OnItemActivated(wxTreeEvent& event);
	void OnSelChanged(wxTreeEvent& event);

	void OnTreeKeyDown(wxTreeEvent& event);

	wxTreeItemId findItemByData(const wxTreeItemId& parent, const wxString& data);
	wxTreeItemId findItemByText(const wxTreeItemId& parent, const wxString& text);

	ItemData* _getSelectedItemData();

public:
	luProjTree(wxWindow* parent);
	~luProjTree();

	wxTreeItemId findItemByData(const wxString& data) { return findItemByData(GetRootItem(), data); }
	wxTreeItemId findItemByText(const wxString& text) { return findItemByText(GetRootItem(), text); }

	int getSelectedItemData();
	wxString getSelectedItemName();


	DECLARE_EVENT_TABLE()
};


class luInspPanel : public wxPanel
{
	wxSearchCtrl* m_search;
	wxListCtrl* m_list;
	wxChoice* m_choice;

public:
	luInspPanel(wxWindow* parent);

	wxListCtrl* getListCtrl() { return m_list; }
	wxChoice* getChoice() { return m_choice; }
	wxSearchCtrl* getSearch() { return m_search; }

	DECLARE_EVENT_TABLE()
};

class luProjPanel : public wxPanel
{
	luProjTree* m_tree;
	wxSplitterWindow* m_splitter;
	luInspPanel* m_insp;
	wxFont* m_fontNormal;
	wxFont* m_fontBold;

	void setItemFont(wxTreeItemId item, bool bold);

	void OnObjTypeChanged(wxCommandEvent& event);
	void OnObjSearchEnter(wxCommandEvent& event);

public:
	luProjPanel(wxWindow* parent);
	~luProjPanel();

	void clearAll()
	{
		clearTreeItems();
		clearListItems();
	}

	//-- tree
	void clearTreeItems();

	luProjTree* getTreeCtrl() { return m_tree; }

	wxTreeItemId getRootItem()
	{
		return m_tree ? m_tree->GetRootItem() : wxTreeItemId();
	}


	void setRootItemText(const wxString& text)
	{
		wxTreeItemId item = getRootItem();
		if (item.IsOk()) m_tree->SetItemText(item, text);
	}

	wxTreeItemId getSelectedItem()
	{
		if (!m_tree) return wxTreeItemId();
		return m_tree->GetFocusedItem();
	}

	wxString getSelectedItemText()
	{
		wxTreeItemId item = getSelectedItem();
		return item.IsOk() ? m_tree->GetItemText(item) : "";
	}

	wxTreeItemId addBlendFile(const wxString& path, gkBlendFile* blend);

	wxTreeItemId addFile(const wxString& path, int data);
	bool removeFile(const wxString& path);
	wxString getSelectedFile()
	{
		return m_tree ? m_tree->getSelectedItemName() : "";
	}

	wxTreeItemId getFile(const wxString& path)
	{
		return m_tree ? m_tree->findItemByData(path) : wxTreeItemId();
	}

	bool setStartupFile(const wxString& path, bool enable);
	bool setProjectName(const wxString& name);

	void expandRoot()
	{
		wxTreeItemId item = getRootItem();
		if (item.IsOk()) m_tree->Expand(item);
	}


	//-- list
	wxListCtrl* getListCtrl() { return m_insp ? m_insp->getListCtrl() : NULL; }
	wxChoice* getChoice() { return m_insp ? m_insp->getChoice() : NULL; }
	wxSearchCtrl* getSearch() { return m_insp ? m_insp->getSearch() : NULL; }
	void addListItem(const wxString& name, const wxString& type);
	void clearListItems();

	void OnObjListChanged(wxListEvent& event);

	DECLARE_EVENT_TABLE()

};

#endif //_luProjPanel_h_
