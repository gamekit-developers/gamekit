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

#ifndef _luAnimDlg_h_
#define _luAnimDlg_h_


class luAnimDlg : public wxDialog
{
	gkScene* m_scene;
	gkGameObject* m_obj;
	bool m_playAnim;

	gkAnimationPlayer* m_curAnimPlayer;
	gkString m_curAnimName;
	gkScalar m_curAnimLength;

	wxString m_length; 
	wxString m_curPos; 
	wxString m_posStep; 

	wxPropertyGrid* m_animProp;

	wxListBox* m_animList;
	wxSlider* m_timeSlider;
	wxButton* m_playBtn;
	wxTextCtrl* m_lengthEdit;
	wxTextCtrl* m_curPosEdit;
	wxButton* m_incPosBtn;
	wxButton* m_decPosBtn;
	wxTextCtrl* m_posStepEdit;
	wxButton* m_addBtn;
	wxButton* m_delBtn;
	wxButton* m_codeBtn;
	wxButton* m_createBtn;
	wxTextCtrl* m_codeEdit;
	wxListBox* m_sceneAnimList;
	wxTextCtrl* m_entityNameEdit;
	wxButton* m_refreshBtn;

	void setupControls();

	void OnClose( wxCloseEvent& event );
	
	void OnPlayClick( wxCommandEvent& event );
	void OnAddClick( wxCommandEvent& event );
	void OnDelClick( wxCommandEvent& event );
	void OnCodeClick( wxCommandEvent& event );
	void OnCreateClick( wxCommandEvent& event );
	void OnIncPosClick( wxCommandEvent& event );
	void OnDecPosClick( wxCommandEvent& event );
	void OnRefreshClick( wxCommandEvent& event );

	void OnTimeSliderChanged( wxCommandEvent& event );

	void playAnim(const wxString& name);
	void stopAnim();
	void setTimePos(gkScalar pos, bool updateTimeSlider=true);
public:
	luAnimDlg(wxWindow* parent);
	virtual ~luAnimDlg();

	void selectObject(gkGameObject* obj, gkScene* scene);
	void update();
	
	DECLARE_EVENT_TABLE()
};

#endif //_luAnimDlg_h_