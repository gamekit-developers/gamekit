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
#include <wx/filefn.h>
#include "nsNodeReader.h"
#include "nsNodeManager.h"
#include "nsSolutionBrowser.h"
#include "nsWorkspace.h"
#include "nsPropertyPage.h"
#include "nsMainWindow.h"
#include "nsXmlUtils.h"




nsNodeReader::nsNodeReader()
{
}


nsNodeReader::~nsNodeReader()
{
}



void nsNodeReader::readNode(nsNodeTree *dest, wxXmlNode *node)
{
    nsString nodeType   = nsXmlUtils::getAttributeString("type",    node);
    nsString nodeId     = nsXmlUtils::getAttributeString("id",      node);

    if (nodeType.empty()) return;

    nsNodeDef *inf = nsNodeTypeInfo::getSingleton().findTypeInfo(nodeType);
    if (!inf)
    {
        // missing
        return;
    }

    nsNode *nd = dest->createNode(inf);

    NSvec2 pos = nsXmlUtils::getAttributeVec2("location", node, NSvec2(0,0));
    nd->setPosition(pos.x, pos.y);
    nd->setState(nsXmlUtils::getAttributeBool("selected", node, false) ? NDST_ACTIVE : NDST_INACTIVE);
    nd->setAttachedName(nsXmlUtils::getAttributeString("object", node));

    for (wxXmlNode *sock = node->GetChildren(); sock; sock = sock->GetNext())
    {
        if (sock->GetName() == "Socket")
        {
            int dir     = nsXmlUtils::getAttributeInt("direction", sock, -1);
            int ioid    = nsXmlUtils::getAttributeInt("ioid", sock, -1);


            nsSocket *nsock = 0;
            if (dir == NS_SOCK_IN && ioid != -1)
                nsock = nd->getInput(ioid);
            else if (dir == NS_SOCK_OUT && ioid != -1)
                nsock = nd->getOutput(ioid);


            if (nsock != 0)
            {
                nsock->getValue().fromString(nsXmlUtils::getAttributeString("value", sock));

                nsString fsock = nsXmlUtils::getAttributeString("from", sock);
                if (!fsock.empty())
                {
                    LinkPair p = {fsock, nsock};
                    m_linker.push_back(p);
                }

                nsString sid = nsXmlUtils::getAttributeString("id", sock);
                if (!sid.empty())
                    m_sockMap.insert(sid, nsock);
            }
        }
    }

    m_nodes.insert(nodeId, nd);

}


void nsNodeReader::load(wxMemoryInputStream &buf)
{
    wxXmlDocument doc;
    if (!doc.Load(buf))
        return;


    nsTreeIterator iter = nsNodeManager::getSingleton().getTreeIterator();
    while (iter.hasMoreElements())
    {
        nsNodeTree *tree = iter.getNext();

        // delete all data from windows
        nsTreeEvent evt(NS_TREE_REMOVE, 0, tree);


        nsSolutionBrowser::getSingleton().treeEvent(evt);
        nsWorkspace::getSingleton().treeEvent(evt);
        nsPropertyPage::getSingleton().treeEvent(evt);
    }

    // remove previous tress
    nsNodeManager::getSingleton().clear();


    if (doc.GetRoot()->GetName() != ("NodeTree"))
        return;


    nsNodeManager &nodeManager = nsNodeManager::getSingleton();
    nsNodeTypeInfo &info = nsNodeTypeInfo::getSingleton();


    for (wxXmlNode *tree = doc.GetRoot()->GetChildren(); tree; tree = tree->GetNext())
    {
        if (tree->GetName() == "BlendPreview")
        {
            // reload preview file
            wxString blend = nsXmlUtils::getAttributeString("blend", tree);

            if (wxFileExists(blend))
                nsMainWindow::getSingleton().setPreviewFile(blend);
            else nsMainWindow::getSingleton().setPreviewFile("");
            continue;
        }

        if (tree->GetName() != "Tree")
            continue;

        nsNodeTree *ntree = nodeManager.createTree(nsXmlUtils::getAttributeString("name", tree, "NodeTree"));

        NSrect proj = nsXmlUtils::getAttributeRect("projection", tree, NSrect(-400,-300, 800, 600));
        if (!proj.getSize().valid())
            proj = NSrect(-400,-300, 800, 600);

        ntree->setProjection(proj);

        NSvec2 size = nsXmlUtils::getAttributeVec2("size", tree, NSvec2(800, 600));
        if (!size.valid())
            size = NSvec2(800, 600);

        ntree->setSize(size);

        if (nsXmlUtils::getAttributeBool("isgroup", tree))
        {
            nsString group = nsXmlUtils::getAttributeString("group", tree);
            if (!group.empty())
            {
                ntree->setGroup(true);
                ntree->setGroupName(group);
            }
        }
        else ntree->setGroup(false);

        // attached name
        ntree->setAttachedName(nsXmlUtils::getAttributeString("object", tree));

        for (wxXmlNode *node = tree->GetChildren(); node; node = node->GetNext())
            readNode(ntree, node);

        if (!m_linker.empty())
        {
            // do link sockets
            nsArrayIterator<SocketLink> iter = nsArrayIterator<SocketLink>(m_linker);
            while (iter.hasMoreElements())
            {
                LinkPair &p = iter.getNext();

                nsString ftype = p.fromId;
                nsSocket *isock = p.inSock;


                NSsize pos = m_sockMap.find(ftype);
                if (pos != NS_NPOS)
                {
                    nsSocket *osock = m_sockMap.at(pos);
                    isock->connect(osock);
                }
            }
            m_linker.clear();
            m_sockMap.clear();
        }

        nsTreeEvent evt(NS_TREE_ADD, 0, ntree);
        nsSolutionBrowser::getSingleton().treeEvent(evt);
        if (nsXmlUtils::getAttributeBool("open", tree))
            nsWorkspace::getSingleton().treeEvent(evt);
    }
}



void nsNodeReader::load(const wxString &path)
{
    if (!wxFileExists(path))
        return;


    nsString buffer;
    nsStringUtils::loadFileAsString(buffer, wxToAscii(path));
    if (buffer.empty())
        return;

    wxMemoryInputStream mem(buffer.c_str(), buffer.size());
    load(mem);
}
