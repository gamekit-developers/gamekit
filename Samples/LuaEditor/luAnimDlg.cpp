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
#include "okUtils.h"
#include "luUtils.h"
#include "Lib/liUtils.h"
#include "luMainApp.h"
#include "luMainFrame.h"
#include "luAnimDlg.h"


luAnimDlg::luAnimDlg(wxWindow* parent)
	:	wxDialog(parent, wxID_ANY, "Object Animations", wxDefaultPosition, wxSize( 703,346 ), wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxCLIP_CHILDREN),
		m_scene(NULL),
		m_obj(NULL),
		m_playAnim(false),
		m_animProp(NULL),
		m_animList(NULL),
		m_timeSlider(NULL),
		m_playBtn(NULL),
		m_lengthEdit(NULL),
		m_curPosEdit(NULL),
		m_incPosBtn(NULL),
		m_decPosBtn(NULL),
		m_posStepEdit(NULL),
		m_curAnimLength(0),
		m_curAnimPlayer(NULL),
		m_posStep("0.1"),
		m_curPos("0"),
		m_length("0")
{
	m_animProp = new wxPropertyGrid(this,ID_ANIM_PROPS,wxDefaultPosition,wxSize(-1,-1),
		wxPG_SPLITTER_AUTO_CENTER |
		wxPG_BOLD_MODIFIED );

	setupControls(); //wxbuild

	m_timeSlider->SetTickFreq(10,0);	
	Show();
}

luAnimDlg::~luAnimDlg()
{

}

BEGIN_EVENT_TABLE(luAnimDlg, wxDialog)
	EVT_CLOSE(luAnimDlg::OnClose)
	EVT_SLIDER(ID_ANIM_TIME_SLIDER,  luAnimDlg::OnTimeSliderChanged)
	EVT_BUTTON(ID_ANIM_PLAY_BUTTON, luAnimDlg::OnPlayClick)
	EVT_BUTTON(ID_ANIM_ADD_BUTTON, luAnimDlg::OnAddClick)
	EVT_BUTTON(ID_ANIM_DEL_BUTTON, luAnimDlg::OnDelClick)
	EVT_BUTTON(ID_ANIM_CODE_BUTTON, luAnimDlg::OnCodeClick)
	EVT_BUTTON(ID_ANIM_CREATE_BUTTON, luAnimDlg::OnCreateClick)
	EVT_BUTTON(ID_ANIM_INCPOS_BUTTON, luAnimDlg::OnIncPosClick)
	EVT_BUTTON(ID_ANIM_DECPOS_BUTTON, luAnimDlg::OnDecPosClick)
	EVT_BUTTON(ID_ANIM_REFRESH_BUTTON, luAnimDlg::OnRefreshClick)
END_EVENT_TABLE()

void luAnimDlg::OnClose( wxCloseEvent& event )
{
	getLuMainFrame()->onAnimDlgClosed();
	Destroy();
}

void luAnimDlg::update()
{
	if (!m_playAnim || !m_curAnimPlayer) return;

	gkScalar pos = m_curAnimPlayer->getTimePosition();
	m_timeSlider->SetValue(int(pos / m_curAnimLength * 100));
	m_curPos = wxString::FromDouble(pos);
	m_curPosEdit->GetValidator()->TransferToWindow();
}

void luAnimDlg::selectObject(gkGameObject* obj, gkScene* scene)
{
	m_scene = scene;
	m_obj = obj;

	m_entityNameEdit->SetLabelText(m_obj->getName());

	m_animList->Clear();
	m_sceneAnimList->Clear();
	m_curPosEdit->SetLabelText("0");

	m_curAnimName = "";
	m_curAnimPlayer = NULL;


	utArray<utHashedString> names;
	obj->getAnimationNames(names);

	for (UTsize i = 0; i < names.size(); i++)
	{
		m_animList->Append(names[i].str());
	}

	gkResourceManager::ResourceIterator it = gkAnimationManager::getSingleton().getResourceIterator();
	while (it.hasMoreElements())
	{
		gkAnimation *anim = (gkAnimation*)it.getNext().second;
		if (anim->getGroupName() == scene->getGroupName())
			m_sceneAnimList->Append(anim->getName());
	}
}

void luAnimDlg::setTimePos(gkScalar pos, bool updateTimeSlider)
{
	if (m_playAnim || !m_curAnimPlayer) return;

	m_curAnimPlayer->setTimePosition(pos);
	m_obj->updateAnimationBlender(0);
	m_curPos = GK2STR(pos);
	m_curPosEdit->GetValidator()->TransferToWindow();

	if (updateTimeSlider)
		m_timeSlider->SetValue(int(pos / m_curAnimLength * 100));
}

void luAnimDlg::OnTimeSliderChanged( wxCommandEvent& WXUNUSED(event)  )
{
	gkScalar pos = float(m_timeSlider->GetValue()) / 100 * m_curAnimLength;

	setTimePos(pos, false);
}


void luAnimDlg::OnIncPosClick( wxCommandEvent& WXUNUSED(event) )
{
	m_posStepEdit->GetValidator()->TransferFromWindow();	
	gkScalar pos = m_curAnimPlayer->getTimePosition() + WX2SCALAR(m_posStep);
	setTimePos(pos);
}

void luAnimDlg::OnDecPosClick( wxCommandEvent& WXUNUSED(event) )
{
	m_posStepEdit->GetValidator()->TransferFromWindow();	
	gkScalar pos = m_curAnimPlayer->getTimePosition() - WX2SCALAR(m_posStep);
	setTimePos(pos);
}

void luAnimDlg::OnPlayClick( wxCommandEvent& WXUNUSED(event) )
{
	wxString name = m_animList->GetStringSelection();
	if (name.IsEmpty()) return;

	m_playAnim = !m_playAnim;

	if (m_playAnim)
		playAnim(name);
	else
		stopAnim();
}

void luAnimDlg::playAnim(const wxString& name)
{
	if (!m_obj) return;

	if (!(m_scene->getUpdateFlags() & gkScene::UF_ANIMATIONS)) 
		m_scene->setUpdateFlags(gkScene::UF_ANIMATIONS);

	gkString animName(WX2GK(name));
	gkAnimationPlayer* player = m_obj->getAnimationPlayer(animName);
	if (!player)
		player = m_obj->addAnimation(animName);

	GK_ASSERT(player);
	m_curAnimPlayer = player;
	m_curAnimName = animName;
	m_curAnimLength = player->getLength();

	m_length = GK2STR(m_curAnimLength);
	m_curPos = "0";
	m_curPosEdit->GetValidator()->TransferToWindow();
	m_lengthEdit->GetValidator()->TransferToWindow();
	
	m_timeSlider->SetRange(0, 100);
	m_timeSlider->SetValue(0);

	m_obj->playAnimation(animName, 1); //gkAppData::gkGlobalActionBlend);

	m_playBtn->SetLabelText(wxT("Stop"));

	gkPrintf("play anim: %s", (const char*)name);
}

void luAnimDlg::stopAnim()
{
	if (m_scene)
		m_scene->setUpdateFlags(m_scene->getUpdateFlags() & ~gkScene::UF_ANIMATIONS);

	m_playBtn->SetLabelText(wxT("Play"));
}


void luAnimDlg::OnAddClick( wxCommandEvent& WXUNUSED(event) )
{
	wxString name = m_sceneAnimList->GetStringSelection();
	if (name.IsEmpty()) return;
	if (m_animList->FindString(name) != wxNOT_FOUND)
		return;
	m_animList->Append(name);
}

void luAnimDlg::OnDelClick( wxCommandEvent& WXUNUSED(event) )
{
	unsigned int pos = m_animList->GetSelection();
	m_animList->Delete(pos);
}

void luAnimDlg::OnCodeClick( wxCommandEvent& WXUNUSED(event) )
{
}

void luAnimDlg::OnCreateClick( wxCommandEvent& WXUNUSED(event) )
{
}


void luAnimDlg::OnRefreshClick( wxCommandEvent& WXUNUSED(event) )
{
	getLuMainFrame()->showAnimDlg();
}

void luAnimDlg::setupControls()
{
	wxStaticText* text1;
	wxStaticText* text2;
	wxStaticText* text3;
	wxStaticText* text6;
	wxStaticText* text7;
	//--

		this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* animSizer;
	animSizer = new wxFlexGridSizer( 1, 2, 0, 0 );
	animSizer->AddGrowableCol( 0 );
	animSizer->AddGrowableRow( 0 );
	animSizer->SetFlexibleDirection( wxBOTH );
	animSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	wxFlexGridSizer* animListSizer;
	animListSizer = new wxFlexGridSizer( 5, 1, 0, 0 );
	animListSizer->AddGrowableCol( 0 );
	animListSizer->AddGrowableRow( 1 );
	animListSizer->SetFlexibleDirection( wxBOTH );
	animListSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* sizerHeader;
	sizerHeader = new wxBoxSizer( wxHORIZONTAL );
	
	text6 = new wxStaticText( this, wxID_ANY, wxT("Object Animations"), wxDefaultPosition, wxDefaultSize, 0 );
	text6->Wrap( -1 );
	sizerHeader->Add( text6, 0, wxALL, 5 );
	
	m_entityNameEdit = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	sizerHeader->Add( m_entityNameEdit, 0, wxALL, 5 );
	
	m_refreshBtn = new wxButton( this, ID_ANIM_REFRESH_BUTTON, wxT("Refresh"), wxDefaultPosition, wxSize( 60,-1 ), 0 );
	sizerHeader->Add( m_refreshBtn, 0, wxALL, 5 );
	
	animListSizer->Add( sizerHeader, 1, wxEXPAND, 5 );
	
	m_animList = new wxListBox( this, ID_ANIM_OBJ_ANIM_LIST, wxDefaultPosition, wxSize( -1,100 ), 0, NULL, wxLB_SINGLE ); 
	animListSizer->Add( m_animList, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* modSizer;
	modSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_addBtn = new wxButton( this, ID_ANIM_ADD_BUTTON, wxT("add"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	modSizer->Add( m_addBtn, 0, wxALL, 5 );
	
	m_delBtn = new wxButton( this, ID_ANIM_DEL_BUTTON, wxT("del"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	modSizer->Add( m_delBtn, 0, wxALL, 5 );
	
	m_codeBtn = new wxButton( this, ID_ANIM_CODE_BUTTON, wxT("code"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	modSizer->Add( m_codeBtn, 0, wxALL, 5 );
	
	m_createBtn = new wxButton( this, ID_ANIM_CREATE_BUTTON, wxT("create"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_createBtn->Enable( false );
	
	modSizer->Add( m_createBtn, 0, wxALL, 5 );
	
	animListSizer->Add( modSizer, 1, wxEXPAND, 5 );
	
	text7 = new wxStaticText( this, wxID_ANY, wxT("Scene Animations"), wxDefaultPosition, wxDefaultSize, 0 );
	text7->Wrap( -1 );
	animListSizer->Add( text7, 0, wxALL, 5 );
	
	m_sceneAnimList = new wxListBox( this, ID_ANIM_SCENE_ANIM_LIST, wxDefaultPosition, wxSize( -1,100 ), 0, NULL, wxLB_SINGLE ); 
	animListSizer->Add( m_sceneAnimList, 0, wxALL|wxEXPAND, 5 );
	
	animSizer->Add( animListSizer, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* controlSizer;
	controlSizer = new wxFlexGridSizer( 4, 1, 0, 0 );
	controlSizer->AddGrowableCol( 0 );
	controlSizer->AddGrowableRow( 2 );
	controlSizer->SetFlexibleDirection( wxBOTH );
	controlSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* playSizer;
	playSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_playBtn = new wxButton( this, ID_ANIM_PLAY_BUTTON, wxT("Play"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	playSizer->Add( m_playBtn, 0, wxALL, 5 );
	
	text1 = new wxStaticText( this, wxID_ANY, wxT("length"), wxDefaultPosition, wxDefaultSize, 0 );
	text1->Wrap( -1 );
	playSizer->Add( text1, 0, wxALL, 5 );
	
	m_lengthEdit = new wxTextCtrl( this, ID_ANIM_LENGTH_EDIT, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_lengthEdit->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &m_length ) );
	
	playSizer->Add( m_lengthEdit, 0, wxALL, 5 );
	
	text2 = new wxStaticText( this, wxID_ANY, wxT("pos"), wxDefaultPosition, wxDefaultSize, 0 );
	text2->Wrap( -1 );
	playSizer->Add( text2, 0, wxALL, 5 );
	
	m_curPosEdit = new wxTextCtrl( this, ID_ANIM_CURPOS_EDIT, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_curPosEdit->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &m_curPos ) );
	
	playSizer->Add( m_curPosEdit, 0, wxALL, 5 );
	
	text3 = new wxStaticText( this, wxID_ANY, wxT("step"), wxDefaultPosition, wxDefaultSize, 0 );
	text3->Wrap( -1 );
	playSizer->Add( text3, 0, wxALL, 5 );
	
	m_incPosBtn = new wxButton( this, ID_ANIM_INCPOS_BUTTON, wxT("+"), wxDefaultPosition, wxSize( 20,-1 ), 0 );
	playSizer->Add( m_incPosBtn, 0, wxALL, 5 );
	
	m_decPosBtn = new wxButton( this, ID_ANIM_DECPOS_BUTTON, wxT("-"), wxDefaultPosition, wxSize( 20,-1 ), 0 );
	playSizer->Add( m_decPosBtn, 0, wxALL, 5 );
	
	m_posStepEdit = new wxTextCtrl( this, ID_ANIM_POSSTEP_EDIT, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
	m_posStepEdit->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &m_posStep ) );
	
	playSizer->Add( m_posStepEdit, 0, wxALL, 5 );
	
	controlSizer->Add( playSizer, 1, wxEXPAND, 5 );
	
	m_timeSlider = new wxSlider( this, ID_ANIM_TIME_SLIDER, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	controlSizer->Add( m_timeSlider, 0, wxALL|wxEXPAND, 5 );
	
	
	m_animProp->SetMinSize( wxSize( -1,100 ) );
	
	controlSizer->Add( m_animProp, 0, wxALL|wxEXPAND, 5 );
	
	m_codeEdit = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,100 ), wxTE_MULTILINE );
	controlSizer->Add( m_codeEdit, 0, wxALL|wxEXPAND, 5 );
	
	animSizer->Add( controlSizer, 1, wxEXPAND, 5 );
	
	this->SetSizer( animSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
}