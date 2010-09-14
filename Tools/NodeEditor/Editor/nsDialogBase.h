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
#ifndef _nsDialogBase_h_
#define _nsDialogBase_h_

#include <wx/dialog.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/combobox.h>
#include <wx/textctrl.h>

class nsNode;



class nsDialog : public wxDialog
{
public:

	nsDialog(wxWindow* parent, int id, const wxString& name, nsNode* node);
	virtual ~nsDialog();
	virtual void Flush(void) = 0;


protected:

	wxSizer* GetRootSizer(void);
	void ApplyRootSizer(wxSizer* sz);
	nsNode* m_node;

	wxTextCtrl* m_name;

};



class nsMouseMotionDialog : public nsDialog
{
private:

	wxSpinCtrlDouble* m_scaleX;
	wxSpinCtrlDouble* m_scaleY;


public:

	nsMouseMotionDialog(wxWindow* parent, nsNode* node);
	virtual ~nsMouseMotionDialog() {}
	void Flush(void);

};


class nsMouseButtonDialog : public nsDialog
{
private:

	wxCheckBox* m_update;
	wxComboBox* m_button;
	wxSpinCtrl* m_delay;


public:

	nsMouseButtonDialog(wxWindow* parent, nsNode* node);
	virtual ~nsMouseButtonDialog() {}

	void Flush(void);
};





class nsObjectMotionDialog : public nsDialog
{
private:

	wxCheckBox* m_u;
	wxSpinCtrlDouble* m_x;
	wxSpinCtrlDouble* m_y;
	wxSpinCtrlDouble* m_z;
	wxSpinCtrlDouble* m_d;


	wxTextCtrl* m_gameObject;
	wxComboBox* m_motionType;
	wxComboBox* m_transformSpace;


	wxCheckBox* m_cx, *m_cy, *m_cz;
	wxSpinCtrlDouble* m_minX, *m_maxX;
	wxSpinCtrlDouble* m_minY, *m_maxY;
	wxSpinCtrlDouble* m_minZ, *m_maxZ;


public:

	nsObjectMotionDialog(wxWindow* parent, nsNode* node);
	virtual ~nsObjectMotionDialog() {}

	void Flush(void);
};



// Type Dialogs

class nsBoolDialog : public nsDialog
{
private:
	wxCheckBox* m_ctrl;

public:

	nsBoolDialog(wxWindow* parent, nsNode* node);
	virtual ~nsBoolDialog() {}
	void Flush(void);
};


class nsIntDialog : public nsDialog
{
private:
	wxSpinCtrl* m_ctrl;

public:

	nsIntDialog(wxWindow* parent, nsNode* node);
	virtual ~nsIntDialog() {}
	void Flush(void);
};

class nsFloatDialog : public nsDialog
{
private:
	wxSpinCtrlDouble* m_ctrl;

public:

	nsFloatDialog(wxWindow* parent, nsNode* node);
	virtual ~nsFloatDialog() {}
	void Flush(void);
};


#endif//_nsDialogBase_h_
