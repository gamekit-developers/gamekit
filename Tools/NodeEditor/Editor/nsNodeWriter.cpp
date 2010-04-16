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


// ----------------------------------------------------------------------------
nsNodeWriter::nsNodeWriter()
{
}

// ----------------------------------------------------------------------------
nsNodeWriter::~nsNodeWriter()
{
}

// ----------------------------------------------------------------------------
static void nsWriteSocket(wxXmlNode *xsock, nsSocket *sock)
{
    if (sock->isConnectedOut())
        xsock->AddAttribute("id", wxString::Format("SK_%p", sock));
    if (sock->isConnected())
        xsock->AddAttribute("from", wxString::Format("SK_%p", sock->getSocketLink()));

    xsock->AddAttribute("value",        sock->getValue().toString());
    xsock->AddAttribute("direction",    nsToString((int)sock->getType()->getDirection()));
    xsock->AddAttribute("ioid",         nsToString((int)sock->getType()->getUid()));
}


// ----------------------------------------------------------------------------
static void nsWriteVariable(wxXmlNode *xnode, nsNode *node)
{
}

// ----------------------------------------------------------------------------
void nsNodeWriter::writeToFile(const char *path)
{
    wxXmlDocument doc;
    wxXmlNode *root = new wxXmlNode(wxXML_ELEMENT_NODE, "NodeTree");
    root->AddAttribute("xmlns", "http://tempuri.org/NodeResource.xsd");
    doc.SetRoot(root);

    nsNodeManager &mgr = nsNodeManager::getSingleton();

    const wxString &preview = nsMainWindow::getSingleton().getPreviewFile();
    if (!preview.empty())
    {
        wxXmlNode *xprev = new wxXmlNode(root, wxXML_ELEMENT_NODE, "BlendPreview");

        wxFileName pathFile(path);
        wxFileName file(preview);
        file.MakeRelativeTo(pathFile.GetPath());
        xprev->AddAttribute("blend", file.GetFullPath());
    }


    nsTreeIterator trees = mgr.getTreeIterator();
    while (trees.hasMoreElements())
    {
        nsNodeTree *tree = trees.getNext();


        wxXmlNode *tnode = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Tree");
        tnode->AddAttribute("name",         tree->getName());
        tnode->AddAttribute("group",        tree->getGroupName());
        tnode->AddAttribute("isgroup",      nsToString(tree->isGroup()));
        tnode->AddAttribute("projection",   nsToString(tree->getProjection()));
        tnode->AddAttribute("size",         nsToString(tree->getSize()));
        tnode->AddAttribute("open",         nsToString(tree->isOpen()));
        tnode->AddAttribute("object",       tree->getAttachedName());

        nsNodeIterator it = tree->getNodeIterator();
        while (it.hasMoreElements())
        {
            nsNode *node = it.getNext();
            wxXmlNode *xnode = new wxXmlNode(tnode, wxXML_ELEMENT_NODE, "Node");

            xnode->AddAttribute("id",           wxString::Format("ND_%p", node));
            xnode->AddAttribute("type",         node->getType()->getName());
            xnode->AddAttribute("location",     nsToString(node->getRect().getPosition()));
            xnode->AddAttribute("selected",     nsToString(node->getState() == NDST_ACTIVE));
            xnode->AddAttribute("object",       node->getAttachedName());

            nsSocket *sock = node->getFirstInput();
            while (sock)
            {
                wxXmlNode *xsock = new wxXmlNode(xnode, wxXML_ELEMENT_NODE, "Socket");
                nsWriteSocket(xsock, sock);
                sock = sock->getNext();
            }

            sock = node->getFirstOutput();
            while (sock)
            {
                wxXmlNode *xsock = new wxXmlNode(xnode, wxXML_ELEMENT_NODE, "Socket");
                nsWriteSocket(xsock, sock);
                sock = sock->getNext();
            }

        }
    }


    doc.Save(path, 2);
}
