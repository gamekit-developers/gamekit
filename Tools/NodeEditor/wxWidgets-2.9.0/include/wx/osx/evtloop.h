///////////////////////////////////////////////////////////////////////////////
// Name:        include/wx/mac/evtloop.h
// Purpose:     simply forwards to wx/mac/carbon/evtloop.h for consistency with
//              the other Mac headers
// Author:      Vadim Zeitlin
// Modified by:
// Created:     2006-01-12
// RCS-ID:      $Id: evtloop.h 57621 2008-12-28 14:32:49Z VZ $
// Copyright:   (c) 2006 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifdef __WXOSX_COCOA__
    #include "wx/osx/cocoa/evtloop.h"
#else
    #include "wx/osx/carbon/evtloop.h"
#endif

