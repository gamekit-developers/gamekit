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

#ifndef _luWizard_h_
#define _luWizard_h_

#include <wx/filepicker.h>

class luNewProjNamePage;
class luNewProjTemplatePage;
class luNewProjFinishPage;

class luNewProjWizard : public wxWizard
{	
    wxWizardPageSimple* m_page1;
	luNewProjNamePage* m_page2;
	luNewProjTemplatePage* m_page3;
	luNewProjFinishPage* m_page4;

public:
	luNewProjWizard(wxFrame *frame, bool useSizer = true);

	wxWizardPage* getFirstPage() const { return m_page1; }

	void OnWizardCancel(wxWizardEvent& event);
    void OnWizardFinished(wxWizardEvent& event);

	
	DECLARE_EVENT_TABLE()
};

#endif //_luWizard_h_