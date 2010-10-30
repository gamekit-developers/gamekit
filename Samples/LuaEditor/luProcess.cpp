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
#include "luProcess.h"
#include "luMainFrame.h"
#include <wx/txtstrm.h> 

luProcess::luProcess(luMainFrame *parent, const wxString& cmd)
    : wxProcess(parent), m_cmd(cmd)
{
    m_parent = parent;
}


void luProcess::OnTerminate(int pid, int status)
{
    wxLogStatus(m_parent, _T("Process %u ('%s') terminated with exit code %d."),
                pid, m_cmd.c_str(), status);

    m_parent->OnAsyncTermination(this);
}


bool luPipedProcess::HasInput()
{
    bool hasInput = false;

#if 0
    if ( IsInputAvailable() )
    {
        wxTextInputStream tis(*GetInputStream());

        wxString msg;
        msg << tis.ReadLine(); //stdout

        m_parent->addPipedOutput(msg);

        hasInput = true;
    }
#endif
    if ( IsErrorAvailable() )
    {
        wxTextInputStream tis(*GetErrorStream());

        wxString msg;
        msg << tis.ReadLine(); //stderr

        m_parent->addPipedOutput(msg);

        hasInput = true;
    }

    return hasInput;
}

void luPipedProcess::OnTerminate(int pid, int status)
{
    while (HasInput())
        ;

    m_parent->OnProcessTerminated(this);

    luProcess::OnTerminate(pid, status);
}


