///////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/popupwin.cpp
// Purpose:     implements wxPopupWindow for wxMac
// Author:      Stefan Csomor
// Modified by:
// Created:     
// RCS-ID:      $Id: popupwin.cpp 55419 2008-09-02 16:53:23Z SC $
// Copyright:   (c) 2006 Stefan Csomor
// License:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// CAUTION : This is only experimental stuff right now

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_POPUPWIN

#ifndef WX_PRECOMP
#endif //WX_PRECOMP

#include "wx/popupwin.h"
#include "wx/tooltip.h"

#include "wx/osx/private.h"    

// ============================================================================
// implementation
// ============================================================================

wxPopupWindow::~wxPopupWindow()
{
}

bool wxPopupWindow::Create(wxWindow *parent, int flags)
{
    // popup windows are created hidden by default
    Hide();

    return wxPopupWindowBase::Create(parent) &&
               wxNonOwnedWindow::Create(parent, wxID_ANY,
                                wxDefaultPosition, wxDefaultSize,
                                flags | wxPOPUP_WINDOW);

}

#endif // #if wxUSE_POPUPWIN
