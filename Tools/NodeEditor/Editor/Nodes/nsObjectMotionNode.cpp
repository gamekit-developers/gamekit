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
#include "Nodes/nsObjectMotionNode.h"
#include "nsSocketEditors.h"
#include "Editors/nsSocketEditors.h"
#include "nsNodeProperties.h"
#include "Logic/gkMotionNode.h"


// ----------------------------------------------------------------------------
class nsObjectMotionNodePropertyPage : public nsNodePropertyPage
{
private:
    wxEnumProperty          *m_enum, *m_transform;
    nsVector2ClampProperty  *m_cx, *m_cy, *m_cz;
    wxStringProperty        *m_relitaveObject;
    wxBoolProperty          *m_keep;

public:

    nsObjectMotionNodePropertyPage(nsPropertyManager *manager, nsNodeDef *type) 
        :   nsNodePropertyPage(manager, type), m_enum(0), m_transform(0),
            m_cx(0), m_cy(0), m_cz(0), m_relitaveObject(0), m_keep(0)
    {
    }

    virtual ~nsObjectMotionNodePropertyPage() {}


    bool wantsCustomData(void)  
    {
        return true;
    }

    // setup properties in m_data
    void initCustomData(void)   
    {
        static const wxChar *labels[] = {
            wxT("Rotation"),
            wxT("Translation"),
            wxT("Scale"),
            wxT("Force"),
            wxT("Torque"),
            wxT("Linear Velocity"),
            wxT("Angular Velocity"),
        };


        static long values[] = {
            MT_ROTATION,
            MT_TRANSLATION,
            MT_SCALE,
            MT_FORCE,
            MT_TORQUE,
            MT_LINV,
            MT_ANGV,
        };


        m_enum = new wxEnumProperty("Motion Type", wxPG_LABEL, labels, values, 0);
        m_enum->SetHelpString("Type of motion affected by this node.");
        m_data->AppendChild(m_enum);

        static const wxChar *trans_label[] = {
            wxT("Local"),
            wxT("Parent"),
            wxT("World"),
        };

        static long trans_values[] = {
            TRANSFORM_LOCAL,
            TRANSFORM_PARENT,
            TRANSFORM_WORLD,
        };


        m_transform = new wxEnumProperty("Tranformation Space", wxPG_LABEL, trans_label, trans_values, 0);
        m_transform->SetHelpString("Space affected by this node.");
        m_data->AppendChild(m_transform);


        m_keep = new wxBoolProperty("Keep Velocity", wxPG_LABEL);
        m_keep->SetHelpString("Keep physics velocities on unconnected sockets.");
        m_data->AppendChild(m_keep);


        m_relitaveObject = new wxStringProperty("Relitave To", wxPG_LABEL);
        m_relitaveObject->SetHelpString("Apply motion relitave to this game object.");
        m_data->AppendChild(m_relitaveObject);

        m_cx = new nsVector2ClampProperty("Clamp X", wxPG_LABEL, NSClampedVec2(NSvec2(0,0), NS_LINEAR_MIN,  NS_LINEAR_MAX, false));
        m_cy = new nsVector2ClampProperty("Clamp Y", wxPG_LABEL, NSClampedVec2(NSvec2(0,0), NS_LINEAR_MIN,  NS_LINEAR_MAX, false));
        m_cz = new nsVector2ClampProperty("Clamp Z", wxPG_LABEL, NSClampedVec2(NSvec2(0,0), NS_LINEAR_MIN,  NS_LINEAR_MAX, false));

        m_data->AppendChild(m_cx);
        m_data->AppendChild(m_cy);
        m_data->AppendChild(m_cz);
    }


    // set properties for node instances
    void doSetCustomData(void)
    {
        if (m_node)
        {
            nsMotionData *data = static_cast<nsMotionData*>(m_node->getData());
            m_enum->SetValue(data->getEnum());
            m_transform->SetValue(data->getTransform());
            m_relitaveObject->SetValue(wxString(data->getRelitaveObject()));
            m_keep->SetValue(data->getKeep());

            m_cx->SetValue(WXVARIANT(NSClampedVec2(data->getClampX(), NS_LINEAR_MIN,  NS_LINEAR_MAX, (data->getFlag() & 1) != 0)));
            m_cy->SetValue(WXVARIANT(NSClampedVec2(data->getClampY(), NS_LINEAR_MIN,  NS_LINEAR_MAX, (data->getFlag() & 2) != 0)));
            m_cz->SetValue(WXVARIANT(NSClampedVec2(data->getClampZ(), NS_LINEAR_MIN,  NS_LINEAR_MAX, (data->getFlag() & 4) != 0)));


        }
    }


    // test data members 
    bool propertyChanged(wxPGProperty *prop)    
    {
        if (m_node)
        {
            nsMotionData *data = static_cast<nsMotionData*>(m_node->getData());
            if (data)
            {
                if (prop == m_enum)
                {
                    data->setEnum(prop->GetValue().GetLong());
                    return true;
                }
                else if (prop == m_cx)
                {
                     const NSClampedVec2 &cv2 = NSClampedVec2RefFromVariant(prop->GetValue());
                     data->setClampX(cv2.m_data);
                     if (cv2.m_clamp)
                         data->setFlag(data->getFlag() | 1);
                     else
                         data->setFlag(data->getFlag() & ~1);
                    return true;
                }
                else if (prop == m_cy)
                {
                     const NSClampedVec2 &cv2 = NSClampedVec2RefFromVariant(prop->GetValue());
                     data->setClampY(cv2.m_data);
                     if (cv2.m_clamp)
                         data->setFlag(data->getFlag() | 2);
                     else
                         data->setFlag(data->getFlag() & ~2);
                    return true;
                }
                else if (prop == m_cz)
                {
                     const NSClampedVec2 &cv2 = NSClampedVec2RefFromVariant(prop->GetValue());
                     data->setClampZ(cv2.m_data);
                     if (cv2.m_clamp)
                         data->setFlag(data->getFlag() | 4);
                     else
                         data->setFlag(data->getFlag() & ~4);
                    return true;
                }
                else if (prop == m_transform)
                {
                    data->setTransform(m_transform->GetValue().GetLong());
                    return true;
                }
                else if (prop == m_relitaveObject)
                {
                    data->setRelitaveObject(nsString(m_relitaveObject->GetValue().GetString()));
                    return true;
                }
                else if (prop == m_keep)
                {
                    data->setKeep(m_keep->GetValue().GetBool());
                    return true;
                }
            }
        }
        return false;
    }

};


// ----------------------------------------------------------------------------
nsObjectMotionNode::nsObjectMotionNode() 
    :   nsNodeDef(NT_OBJECT_MOTION, NT_GROUP_OUTPUT, "Object Motion")
{
    m_size.x = NS_MAKE_NODE_DEF_WIDTH(nsString("Object Motion").size());
    m_palette = nsSystemPalette::getPalette(NS_SPE_OUTPUT);


    NS_CREATE_BOOL_SOCKET( NS_SOCK_IN, "Update",    gkMotionNode::UPDATE,       0,  1,  false);
    NS_CREATE_FLOAT_SOCKET( NS_SOCK_IN, "X",        gkMotionNode::X,            NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
    NS_CREATE_FLOAT_SOCKET( NS_SOCK_IN, "Y",        gkMotionNode::X,            NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
    NS_CREATE_FLOAT_SOCKET( NS_SOCK_IN, "Z",        gkMotionNode::X,            NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
    NS_CREATE_FLOAT_SOCKET( NS_SOCK_IN, "Damping",  gkMotionNode::DAMPING,      0.0, 1.0,  0);
}

// ----------------------------------------------------------------------------
nsNodeData* nsObjectMotionNode::getData(void)
{
    nsMotionData *data= new nsMotionData(this);
    data->setEnum(MT_ROTATION);
    data->setFlag(0);
    data->setTransform(TRANSFORM_LOCAL);
    return data;
}

// ----------------------------------------------------------------------------
nsNodePropertyPage *nsObjectMotionNode::getEditor(nsPropertyManager *parent)
{ 
    return new nsObjectMotionNodePropertyPage(parent, this); 
}
