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
#include <wx/xml/xml.h>
#include <wx/filename.h>
#include "nsNodeWriter.h"
#include "nsNodeManager.h"
#include "nsMainWindow.h"



nsNodeWriter::nsNodeWriter()
{
}


nsNodeWriter::~nsNodeWriter()
{
}


static void nsWriteSocket(FILE* fp, nsSocket* sock)
{
	fprintf(fp, "\t\t\tSocket %s {  ", sock->getType()->getName().c_str());

	if (sock->isInput())
	{
		if (sock->isConnected())
			fprintf(fp, "value = ND_%p.%s;", sock->getSocketLink()->getParent(), sock->getSocketLink()->getType()->getName().c_str());
		else
			fprintf(fp, "value = %s;", sock->getValue().toString().c_str());
	}
	fprintf(fp, "  }\n");
}



static void nsWriteVariable(wxXmlNode* xnode, nsNode* node)
{
}



static nsString nsGetNodeTypeString(nsNode* node)
{
	switch (node->getType()->getType())
	{
	case NT_SKELETON:
		return "NT_SKELETON";
	case NT_MOUSE_MOTION:
		return "NT_MOUSE_MOTION";
	case NT_MOUSE_BUTTON:
		return "NT_MOUSE_BUTTON";
	case NT_OBJECT_MOTION:
		return "NT_OBJECT_MOTION";
	case NT_BTI:
		return "NT_BTI";
	case NT_BTF:
		return "NT_BTF";
	case NT_ITB:
		return "NT_ITB";
	case NT_ITF:
		return "NT_ITF";
	case NT_FTB:
		return "NT_FTB";
	case NT_FTI:
		return "NT_FTI";
	}
	return "NT_UNKNOWN";
}


void nsNodeWriter::writeToFile(const char* path)
{
}
