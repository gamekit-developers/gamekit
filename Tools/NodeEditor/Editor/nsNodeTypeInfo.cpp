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
#include <wx/variant.h>
#include "nsNodeTypeInfo.h"
#include "nsVariable.h"


// ----------------------------------------------------------------------------
NS_IMPLEMENT_SINGLETON(nsNodeTypeInfo);


#define NS_INVALID_ATTR wxString("INVALID_ATTRIBUTE")


// string conversion 
#define wxFromAscii(x)  wxString(x.c_str(), x.size())
#define wxToAscii(x)    (const char*)x.mb_str()


// ----------------------------------------------------------------------------
nsNodeType::~nsNodeType()
{
    nsNodeType::SocketIterator in(m_inputs);
    while (in.hasMoreElements())
        delete in.getNext();

    nsNodeType::SocketIterator out(m_outputs);
    while (out.hasMoreElements())
        delete out.getNext();

    nsVariableIterator var(m_variables);
    while (var.hasMoreElements())
        delete var.getNext();
}


// ----------------------------------------------------------------------------
nsNodeTypeInfo::nsNodeTypeInfo()
{
}

// ----------------------------------------------------------------------------
nsNodeTypeInfo::~nsNodeTypeInfo()
{
    TypeIterator iter(m_types);
    while (iter.hasMoreElements())
        delete iter.getNext();
}


// ----------------------------------------------------------------------------
nsNodeType* nsNodeTypeInfo::findTypeInfo(int i)
{
    TypeIterator iter(m_types);
    while (iter.hasMoreElements())
    {
        nsNodeType* nt= iter.getNext();
        if (nt->m_id == i)
            return nt;
    }
    return 0;
}


// ----------------------------------------------------------------------------
static nsVariable::PropertyTypes nsGetDataType(const utString& t)
{
    // todo Replace socket::m_type with nsVariable::PropertyTypes
    if (t == "Boolean")
        return nsVariable::VAR_BOOL;
    if (t == "Int")
        return nsVariable::VAR_INT;
    if (t == "Float")
        return nsVariable::VAR_REAL;
    if (t == "String")
        return nsVariable::VAR_STRING;
    if (t == "Vector2")
        return nsVariable::VAR_VEC2;
    if (t == "Vector3")
        return nsVariable::VAR_VEC3;
    if (t == "Vector4")
        return nsVariable::VAR_VEC4;
    if (t == "Quaternion")
        return nsVariable::VAR_QUAT;
    if (t == "Enum")
        return nsVariable::VAR_ENUM;
    return nsVariable::VAR_BOOL;
}

// ----------------------------------------------------------------------------
static nsColorScheme nsGetColor(const utString& t)
{
    if (t == "Red")
        return NS_COL_RED;
    if (t == "Yellow")
        return NS_COL_YELLOW;
    if (t == "Blue")
        return NS_COL_BLUE;
    if (t == "Orange")
        return NS_COL_ORANGE;
    if (t == "Green")
        return NS_COL_GREEN;
    if (t == "Purple")
        return NS_COL_PURPLE;
    if (t == "OrangeRed")
        return NS_COL_ORANGE_RED;
    if (t == "YellowOrange")
        return NS_COL_YELLOW_ORANGE;
    if (t == "YellowGreen")
        return NS_COL_YELLOW_GREEN;
    if (t == "BlueGreen")
        return NS_COL_BLUE_GREEN;
    if (t == "BluePurple")
        return NS_COL_BLUE_PURPLE;
    if (t == "RedPurple")
        return NS_COL_RED_PURPLE;
    return NS_COL_GREY;
}



// ----------------------------------------------------------------------------
// Find IO type
static nsSocketType::Direction nsGetDirection(const utString& t)
{
    if (t == "In")
        return nsSocketType::In;
    return nsSocketType::Out;
}


// ----------------------------------------------------------------------------
// Sort function 
static bool nsSocketSort(nsSocketType* const& a, nsSocketType* const &b)
{
    return a->m_index > b->m_index;
}

static bool nsHasAttribute(const utString &attr, wxXmlNode *node)
{
    return node->HasAttribute(wxFromAscii(attr));
}


static bool nsGetAttributeBool(const utString &attr, wxXmlNode *node, bool def = false)
{
    wxString attrWx = node->GetAttribute(wxFromAscii(attr), NS_INVALID_ATTR);
    if (attrWx == NS_INVALID_ATTR)
        return def;
    attrWx = attrWx.MakeLower();
    return attrWx == ("yes") || attrWx == ("true") || atoi(wxToAscii(attrWx)) > 0;
}


static wxLongLong nsGetAttributeInt(const utString &attr, wxXmlNode *node, wxLongLong def = 0)
{
    wxString attrWx = node->GetAttribute(wxFromAscii(attr), NS_INVALID_ATTR);
    if (attrWx == NS_INVALID_ATTR)
        return def;

    return wxLongLong(atoi(wxToAscii(attrWx)));
}


static float nsGetAttributeFloat(const utString &attr, wxXmlNode *node, float def = 0)
{
    wxString attrWx = node->GetAttribute(wxFromAscii(attr), NS_INVALID_ATTR);
    if (attrWx == NS_INVALID_ATTR)
        return def;
    return (float)atof(wxToAscii(attrWx));
}

static utString nsGetAttributeString(const utString &attr, wxXmlNode *node, const utString& def = "")
{
    wxString attrWx = node->GetAttribute(wxFromAscii(attr), NS_INVALID_ATTR);
    if (attrWx == NS_INVALID_ATTR)
        return def;
    return utString(wxToAscii(attrWx));
}



static NSvec2 nsGetAttributeVec2(const utString &attr, wxXmlNode *node, const NSvec2& def = NSvec2(0,0))
{
    wxString attrWx = node->GetAttribute(wxFromAscii(attr), NS_INVALID_ATTR);
    if (attrWx == NS_INVALID_ATTR)
        return def;
    return utStringConverter::toVec2(wxToAscii(attrWx));
}


static NSrect nsGetAttributeRect(const utString &attr, wxXmlNode *node, const NSrect& def = NSrect(0,0,0,0))
{
    wxString attrWx = node->GetAttribute(wxFromAscii(attr), NS_INVALID_ATTR);
    if (attrWx == NS_INVALID_ATTR)
        return def;
    return utStringConverter::toRect(wxToAscii(attrWx));
}



static nsColorScheme nsGetAttributeColor(const utString &attr, wxXmlNode *node, const nsColorScheme& def = NS_COL_GREY)
{
    wxString attrWx = node->GetAttribute(wxFromAscii(attr), NS_INVALID_ATTR);
    if (attrWx == NS_INVALID_ATTR)
        return def;
    return nsGetColor(wxToAscii(attrWx));
}


// ----------------------------------------------------------------------------
static void nsParsePropertySection(wxXmlNode *propertyRoot, nsNodeType *node)
{
    for (wxXmlNode *props = propertyRoot->GetChildren(); props; props = props->GetNext())
    {
        if (!nsHasAttribute("name", props))
            continue;

        if (props->GetName() == "Enum")
        {
            nsVariableType *vt = new nsVariableType;
            vt->m_name = nsGetAttributeString("name",  props);
            vt->m_type = nsVariable::VAR_ENUM;
            vt->m_value = "0";
            vt->m_briefHelp = nsGetAttributeString("docs",  props);
            node->m_variables.push_back(vt);

            int enumId = 0;
            for (wxXmlNode *items = props->GetChildren(); items; items = items->GetNext())
            {
                if (items->GetName() != "EnumItem")
                    continue;
                if (!nsHasAttribute("name", items))
                    continue;

                nsEnumItem item;
                item.m_name = nsGetAttributeString("name",  items);
                item.m_value = enumId++;

                vt->m_enum.push_back(item);
            }
        }
        else if (props->GetName() == "Variable")
        {
            nsVariableType *vt = new nsVariableType;
            vt->m_name = nsGetAttributeString("name",  props);
            vt->m_type = nsGetDataType(nsGetAttributeString("type", props));
            vt->m_value = nsGetAttributeString("default", props, "0");
            vt->m_briefHelp = nsGetAttributeString("docs",  props);
            node->m_variables.push_back(vt);
        }
    }
}

// ----------------------------------------------------------------------------
void nsNodeTypeInfo::parseTypes(const utString &path)
{
    // TODO: handle errors
    wxXmlDocument doc;
    if (!doc.Load(wxFromAscii(path)))
        return;

    if (doc.GetRoot()->GetName() != ("NodeTreeDef"))
        return;

    wxString tmpStr;
    UTsize pos;
    int nsUID = 0;
    for (wxXmlNode *child = doc.GetRoot()->GetChildren(); child; child = child->GetNext())
    {
        if (child->GetName() != ("Node"))
            continue;

        if (!nsHasAttribute("typename",     child) ||
            !nsHasAttribute("groupname",    child ) ||
            !nsHasAttribute("size",         child))
            continue;

        nsNodeType *nt = new nsNodeType;
        nt->m_id        = nsUID++;

        nt->m_typename  = nsGetAttributeString("typename",  child);
        nt->m_groupname = nsGetAttributeString("groupname", child);
        nt->m_size      = nsGetAttributeVec2("size",    child, NSvec2(100, 100));
        nt->m_color     = nsGetAttributeColor("color", child);
        m_types.push_back(nt);

        // sort groups
        pos = m_groups.find(nt->m_groupname);
        if (pos == UT_NPOS)
            m_groups.insert(nt->m_groupname, TypeList());
        pos = m_groups.find(nt->m_groupname);
        m_groups.at(pos).push_back(nt);
        nt->m_groupId = (int)pos;


        int sock_id = 0;
        for (wxXmlNode *sockets = child->GetChildren(); sockets; sockets = sockets->GetNext())
        {
            if (sockets->GetName() == ("Properties"))
            {
                nsParsePropertySection(sockets, nt);
                continue;
            }


            if (sockets->GetName() == ("DocString"))
                nt->m_briefHelp = nsGetAttributeString("docs", sockets);

            if (sockets->GetName() != ("Socket"))
                continue;

            if (!nsHasAttribute("type",     sockets)  ||
                !nsHasAttribute("name",     sockets ) ||
                !nsHasAttribute("rect",     sockets ) ||
                !nsHasAttribute("direction",sockets))
                continue;


            int newId = sock_id++;
            newId = nsGetAttributeInt("index", sockets, newId).ToLong();

            nsVariable::PropertyTypes defType = nsGetDataType(nsGetAttributeString("type", sockets));

            nsSocketType *st = new nsSocketType();
            st->m_index         = newId;
            st->m_type          = defType;
            st->m_name          = nsGetAttributeString("name", sockets);
            st->m_direction     = nsGetDirection(nsGetAttributeString("direction", sockets));
            st->m_rect          = nsGetAttributeRect("rect", sockets);
            st->m_default       = nsGetAttributeString("default", sockets, "0");
            st->m_color         = nsGetAttributeColor("color", sockets, NS_COL_ORANGE);
            st->m_briefHelp     = "";

            for (wxXmlNode *socketInfo = sockets->GetChildren(); socketInfo; socketInfo = socketInfo->GetNext())
            {
                if (socketInfo->GetName() == ("DocString"))
                    st->m_briefHelp = nsGetAttributeString("docs", socketInfo);
            }



            if (st->m_direction == nsSocketType::In)
                nt->m_inputs.push_back(st);
            else
               nt->m_outputs.push_back(st);
        }

        nt->m_inputs.sort(nsSocketSort);
        nt->m_outputs.sort(nsSocketSort);

    }
}
