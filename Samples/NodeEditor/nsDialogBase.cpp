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

Common Layout:

    ---
    Node Data
    ---
    Input Sockets
    ---
    OK | Cancel



*/
#include "nsDialogBase.h"
#include "nsNode.h"
#include "nsSocket.h"

#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>

#define DEFAULT_SIZEX 150
#define DEFAULT_SIZEY 200
#define MAIN_SIZER_DEF wxSizerFlags(1).Expand().Align(wxCENTER).Border(wxALL, 5)
#define DATA_SIZER_DEF wxSizerFlags(0).Align(wxLEFT).Border(wxALL, 0)
#define DATA_SIZER_DEFT wxSizerFlags(0).Align(wxLEFT | wxTOP).Border(wxALL, 0)

#define DATA_ALIGN_ARGS 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5




// Base implementation

nsDialog::nsDialog(wxWindow* parent, int id, const wxString& name, nsNode* node)
	:    wxDialog(parent, id, name), m_node(node)
{
}


nsDialog::~nsDialog()
{
}

wxSizer* nsDialog::GetRootSizer(void)
{
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	return sizer;
}


void nsDialog::ApplyRootSizer(wxSizer* sz)
{
	wxBoxSizer* main = new wxBoxSizer(wxVERTICAL);


	wxSizer* okc = CreateButtonSizer(wxOK | wxCANCEL);
	sz->Add(okc, wxSizerFlags(1).Right());
	main->Add(sz, wxSizerFlags(1).Border(wxALL, 10));

	SetSizer( main );
	main->SetSizeHints( this );
}






nsMouseMotionDialog::nsMouseMotionDialog(wxWindow* parent, nsNode* node)
	:    nsDialog(parent, -1, wxT("Mouse Motion"), node)
{

	wxSizer* sizer = GetRootSizer();

	// No data

	nsSocket* scaleX = node->getInput(0);
	nsSocket* scaleY = node->getInput(1);

	nsSocketDef* stype;
	wxStaticText* xlab, *ylab;

	{
		stype = scaleX->getType();
		xlab = new wxStaticText(this, wxID_ANY, wxT("ScaleX"));

		m_scaleX = new wxSpinCtrlDouble(this, wxID_ANY);
		m_scaleX->SetRange(stype->getMin(), stype->getMax());
		m_scaleX->SetValue((float)scaleX->getValue());

		m_scaleX->SetIncrement(0.001);

		if (scaleX->isConnected())
		{
			xlab->Disable();
			m_scaleX->Disable();
		}
	}

	{
		stype = scaleY->getType();

		ylab = new wxStaticText(this, wxID_ANY, wxT("ScaleY"));

		m_scaleY = new wxSpinCtrlDouble(this, wxID_ANY);
		m_scaleY->SetRange(stype->getMin(), stype->getMax());
		m_scaleY->SetValue((float)scaleY->getValue());

		m_scaleY->SetIncrement(0.001);


		if (scaleY->isConnected())
		{
			ylab->Disable();
			m_scaleY->Disable();
		}
	}





	wxStaticBoxSizer* sbs = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Input Sockets"));


	// Input Sockets

	wxFlexGridSizer* fgs = new wxFlexGridSizer(2, 2, 0, 0);
	fgs->Add(xlab, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	fgs->Add(m_scaleX, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	fgs->Add(ylab,  1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	fgs->Add(m_scaleY,  1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);

	sbs->Add(fgs,  1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);

	sizer->Add(sbs, MAIN_SIZER_DEF);

	ApplyRootSizer(sizer);

}

void nsMouseMotionDialog::Flush(void)
{
	m_node->getInput(0)->setValue((float)m_scaleX->GetValue());
	m_node->getInput(1)->setValue((float)m_scaleY->GetValue());
}



nsMouseButtonDialog::nsMouseButtonDialog(wxWindow* parent, nsNode* node)
	:    nsDialog(parent, -1, wxT("Mouse Button"), node)
{
	wxSizer* sizer = GetRootSizer();
	nsButtonData* bd = (nsButtonData*)node->getData();




	wxStaticBoxSizer* sbs = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Node Data"));

	wxArrayString opts;
	opts.push_back("Left");
	opts.push_back("Middle");
	opts.push_back("Right");


	wxFlexGridSizer* labelData = new wxFlexGridSizer( 2, 2, 1, 1);

	wxString def = wxT("Left");
	int val = bd->getValue();
	if (val == 1)
		def = wxT("Middle");
	if (val == 2)
		def = wxT("Right");


	wxStaticText* label = new wxStaticText(this, -1, wxT("Mouse Button"));
	m_button = new wxComboBox(this, -1, def, wxDefaultPosition, wxDefaultSize, opts);

	labelData->Add(label,    1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	labelData->Add(m_button, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);

	label   = new wxStaticText(this, -1, wxT("Click Delay"));
	m_delay = new wxSpinCtrl(this, -1, wxT("0"));


	m_delay->SetValue(bd->getDelay());
	m_delay->SetRange(0, NS_LINEAR_MAX);

	labelData->Add(label,   1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	labelData->Add(m_delay, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);

	sbs->Add( labelData, DATA_SIZER_DEF);
	sizer->Add(sbs, MAIN_SIZER_DEF);


	// Input Sockets

	sbs = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Input Sockets"));
	m_update = new wxCheckBox(this, wxID_ANY, wxT("Update"));
	m_update->SetValue(m_node->getInput(0)->getValue());

	if (m_node->getInput(0)->isConnected())
		m_update->Disable();


	sbs->Add( m_update, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	sizer->Add(sbs, MAIN_SIZER_DEF);


	ApplyRootSizer(sizer);
}


void nsMouseButtonDialog::Flush(void)
{
	nsButtonData* bd = (nsButtonData*)m_node->getData();
	bd->setValue(m_button->GetSelection());
	bd->setDelay(m_delay->GetValue());
	m_node->getInput(0)->setValue(m_update->GetValue());
}







nsObjectMotionDialog::nsObjectMotionDialog(wxWindow* parent, nsNode* node)
	:    nsDialog(parent, -1, "Object Motion", node)
{
	wxSizer* sizer = GetRootSizer();

	// Data
	nsMotionData* data = (nsMotionData*)node->getData();

	wxStaticBoxSizer* sbs = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Motion Data"));


	wxArrayString mvals;

	wxStaticText* mt, *ts;
	{
		mt = new wxStaticText(this, wxID_ANY, wxT("Motion Type"));
		mvals.push_back(wxT("Rotation"));
		mvals.push_back(wxT("Translation"));
		mvals.push_back(wxT("Scale"));
		mvals.push_back(wxT("Force"));
		mvals.push_back(wxT("Torque"));
		mvals.push_back(wxT("Linear Velocity"));
		mvals.push_back(wxT("Angular Velocity"));


		m_motionType = new wxComboBox(this, wxID_ANY);
		m_motionType->Append(mvals);
		m_motionType->SetSelection(data->getEnum());

	}

	{
		ts = new wxStaticText(this, wxID_ANY, wxT("Transform Space"));
		mvals.Clear();
		mvals.push_back(wxT("Local"));
		mvals.push_back(wxT("Parent"));
		mvals.push_back(wxT("World"));
		m_transformSpace = new wxComboBox(this, wxID_ANY);
		m_transformSpace->Append(mvals);
		m_transformSpace->SetSelection(data->getTransform());
	}

	{
		m_cx = new wxCheckBox(this, wxID_ANY, wxT("Clamp X"));
		m_cx->SetValue((data->getClampFlag() & 1) != 0);

		m_minX = new wxSpinCtrlDouble(this);
		m_minX->SetRange(NS_LINEAR_MIN, NS_LINEAR_MAX);
		m_minX->SetValue(data->getClampX().x);
		m_minX->SetIncrement(0.01);

		m_maxX = new wxSpinCtrlDouble(this);
		m_maxX->SetRange(NS_LINEAR_MIN, NS_LINEAR_MAX);
		m_maxX->SetValue(data->getClampX().y);
		m_maxX->SetIncrement(0.01);
	}

	{
		m_cy = new wxCheckBox(this, wxID_ANY, wxT("Clamp Y"));
		m_cy->SetValue((data->getClampFlag() & 2) != 0);

		m_minY = new wxSpinCtrlDouble(this);
		m_minY->SetRange(NS_LINEAR_MIN, NS_LINEAR_MAX);
		m_minY->SetValue(data->getClampY().x);
		m_minY->SetIncrement(0.01);

		m_maxY = new wxSpinCtrlDouble(this);
		m_maxY->SetRange(NS_LINEAR_MIN, NS_LINEAR_MAX);
		m_maxY->SetValue(data->getClampY().y);
		m_maxY->SetIncrement(0.01);
	}

	{
		m_cz = new wxCheckBox(this, wxID_ANY, wxT("Clamp Z"));
		m_cz->SetValue((data->getClampFlag() & 4) != 0);

		m_minZ = new wxSpinCtrlDouble(this);
		m_minZ->SetRange(NS_LINEAR_MIN, NS_LINEAR_MAX);
		m_minZ->SetValue(data->getClampZ().x);
		m_minZ->SetIncrement(0.01);

		m_maxZ = new wxSpinCtrlDouble(this);
		m_maxZ->SetRange(NS_LINEAR_MIN, NS_LINEAR_MAX);
		m_maxZ->SetValue(data->getClampZ().y);
		m_maxZ->SetIncrement(0.01);
	}


	wxFlexGridSizer* labelData = new wxFlexGridSizer( 5, 3, 0, 0);

	labelData->Add(mt, DATA_ALIGN_ARGS);
	labelData->Add(m_motionType, DATA_ALIGN_ARGS);
	labelData->Add(new wxPanel(this), DATA_ALIGN_ARGS);

	labelData->Add(ts, DATA_ALIGN_ARGS);
	labelData->Add(m_transformSpace, DATA_ALIGN_ARGS);
	labelData->Add(new wxPanel(this), DATA_ALIGN_ARGS);


	labelData->Add(m_cx, DATA_ALIGN_ARGS);
	labelData->Add(m_minX, DATA_ALIGN_ARGS);
	labelData->Add(m_maxX, DATA_ALIGN_ARGS);


	labelData->Add(m_cy, DATA_ALIGN_ARGS);
	labelData->Add(m_minY, DATA_ALIGN_ARGS);
	labelData->Add(m_maxY, DATA_ALIGN_ARGS);

	labelData->Add(m_cz, DATA_ALIGN_ARGS);
	labelData->Add(m_minZ, DATA_ALIGN_ARGS);
	labelData->Add(m_maxZ, DATA_ALIGN_ARGS);


	sbs->Add(labelData, DATA_SIZER_DEF);
	sizer->Add(sbs, MAIN_SIZER_DEF);


	// Input Sockets

	sbs = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Input Sockets"));
	labelData = new wxFlexGridSizer( 5, 2, 0, 0);


	m_u = new wxCheckBox(this, wxID_ANY, wxT("Update"));
	m_u->SetValue(m_node->getInput(0)->getValue());

	if (m_node->getInput(0)->isConnected())
		m_u->Disable();


	wxStaticText* lx, *ly, *lz, *ld;


	lx = new wxStaticText(this, wxID_ANY, wxT("X"));
	m_x = new wxSpinCtrlDouble(this);
	m_x->SetRange(NS_LINEAR_MIN, NS_LINEAR_MAX);
	m_x->SetValue((float)m_node->getInput(1)->getValue());
	m_x->SetIncrement(0.01);

	if (m_node->getInput(1)->isConnected())
	{
		lx->Disable();
		m_x->Disable();
	}


	ly = new wxStaticText(this, wxID_ANY, wxT("Y"));
	m_y = new wxSpinCtrlDouble(this);
	m_y->SetRange(NS_LINEAR_MIN, NS_LINEAR_MAX);
	m_y->SetValue((float)m_node->getInput(2)->getValue());
	m_y->SetIncrement(0.01);
	if (m_node->getInput(2)->isConnected())
	{
		ly->Disable();
		m_y->Disable();
	}


	lz = new wxStaticText(this, wxID_ANY, wxT("Z"));
	m_z = new wxSpinCtrlDouble(this);
	m_z->SetRange(NS_LINEAR_MIN, NS_LINEAR_MAX);
	m_z->SetValue((float)m_node->getInput(3)->getValue());
	m_z->SetIncrement(0.01);
	if (m_node->getInput(3)->isConnected())
	{
		lz->Disable();
		m_z->Disable();
	}


	ld = new wxStaticText(this, wxID_ANY, wxT("Damping"));
	m_d = new wxSpinCtrlDouble(this);
	m_d->SetRange(0, 1);
	m_d->SetValue((float)m_node->getInput(4)->getValue());
	m_d->SetIncrement(0.01);
	if (m_node->getInput(4)->isConnected())
	{
		ld->Disable();
		m_d->Disable();
	}

	labelData->Add(m_u, DATA_ALIGN_ARGS);
	labelData->Add(new wxPanel(this), DATA_ALIGN_ARGS);

	labelData->Add(lx, DATA_ALIGN_ARGS);
	labelData->Add(m_x, DATA_ALIGN_ARGS);

	labelData->Add(ly, DATA_ALIGN_ARGS);
	labelData->Add(m_y, DATA_ALIGN_ARGS);

	labelData->Add(lz, DATA_ALIGN_ARGS);
	labelData->Add(m_z, DATA_ALIGN_ARGS);

	labelData->Add(ld, DATA_ALIGN_ARGS);
	labelData->Add(m_d, DATA_ALIGN_ARGS);



	sbs->Add(labelData, DATA_SIZER_DEF);
	sizer->Add(sbs, MAIN_SIZER_DEF);


	ApplyRootSizer(sizer);
}


void nsObjectMotionDialog::Flush(void)
{
	nsMotionData* data = (nsMotionData*)m_node->getData();

	data->setClampFlag(0);
	data->setEnum(m_motionType->GetSelection());
	data->setTransform(m_transformSpace->GetSelection());


	if (m_cx->GetValue())
	{
		data->setClampFlag(1);
		data->setClampX(NSvec2(m_minX->GetValue(), m_maxX->GetValue()));
	}

	if (m_cy->GetValue())
	{
		data->setClampFlag(data->getClampFlag() | 2);
		data->setClampY(NSvec2(m_minY->GetValue(), m_maxY->GetValue()));
	}

	if (m_cz->GetValue())
	{
		data->setClampFlag(data->getClampFlag() | 4);
		data->setClampZ(NSvec2(m_minZ->GetValue(), m_maxZ->GetValue()));
	}


	m_node->getInput(0)->setValue(m_u->GetValue());
	m_node->getInput(1)->setValue((float)m_x->GetValue());
	m_node->getInput(2)->setValue((float)m_y->GetValue());
	m_node->getInput(3)->setValue((float)m_z->GetValue());
	m_node->getInput(4)->setValue((float)m_d->GetValue());
}






nsBoolDialog::nsBoolDialog(wxWindow* parent, nsNode* node)
	: nsDialog(parent, -1, wxT("Bool"), node)
{
	wxSizer* sizer = GetRootSizer();

	wxStaticBoxSizer* sbs = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Input Sockets"));

	m_ctrl = new wxCheckBox(this, wxID_ANY, wxT("Bool"));
	m_ctrl->SetValue(node->getInput(0)->getValue());
	sbs->Add(m_ctrl, DATA_ALIGN_ARGS);

	if (node->getInput(0)->isConnected())
		m_ctrl->Disable();


	sizer->Add(sbs, MAIN_SIZER_DEF);

	ApplyRootSizer(sizer);
}

void nsBoolDialog::Flush(void)
{
	m_node->getInput(0)->setValue(m_ctrl->GetValue());
}



nsIntDialog::nsIntDialog(wxWindow* parent, nsNode* node)
	: nsDialog(parent, -1, wxT("Int"), node)
{
	wxSizer* sizer = GetRootSizer();

	wxStaticBoxSizer* sbs = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Input Sockets"));


	wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* label = new wxStaticText(this, wxID_ANY, wxT("Int"));


	m_ctrl = new wxSpinCtrl(this, wxID_ANY);
	m_ctrl->SetRange(NS_LINEAR_MIN, NS_LINEAR_MAX);
	m_ctrl->SetValue((int)node->getInput(0)->getValue());


	if (node->getInput(0)->isConnected())
	{
		m_ctrl->Disable();
		label->Disable();
	}

	bs->Add(label, DATA_ALIGN_ARGS);
	bs->Add(m_ctrl, DATA_ALIGN_ARGS);

	sbs->Add(bs, DATA_ALIGN_ARGS);
	sizer->Add(sbs, MAIN_SIZER_DEF);

	ApplyRootSizer(sizer);
}

void nsIntDialog::Flush(void)
{
	m_node->getInput(0)->setValue(m_ctrl->GetValue());
}



nsFloatDialog::nsFloatDialog(wxWindow* parent, nsNode* node)
	: nsDialog(parent, -1, wxT("Float"), node)
{
	wxSizer* sizer = GetRootSizer();

	wxStaticBoxSizer* sbs = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Input Sockets"));

	wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* label = new wxStaticText(this, wxID_ANY, wxT("Float"));

	m_ctrl = new wxSpinCtrlDouble(this, wxID_ANY);
	m_ctrl->SetRange(NS_LINEAR_MIN, NS_LINEAR_MAX);
	m_ctrl->SetValue((float)node->getInput(0)->getValue());
	if (node->getInput(0)->isConnected())
	{
		m_ctrl->Disable();
		label->Disable();
	}

	bs->Add(label, DATA_ALIGN_ARGS);
	bs->Add(m_ctrl, DATA_ALIGN_ARGS);

	sbs->Add(bs, DATA_ALIGN_ARGS);
	sizer->Add(sbs, MAIN_SIZER_DEF);

	ApplyRootSizer(sizer);
}

void nsFloatDialog::Flush(void)
{
	m_node->getInput(0)->setValue((float)m_ctrl->GetValue());
}
