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
#include "Nodes/nsMouseMotionNode.h"
#include "Editors/nsSocketEditors.h"
#include "nsNodeProperties.h"
#include "Logic/gkMouseNode.h"


// ----------------------------------------------------------------------------
nsMouseMotionNode::nsMouseMotionNode() 
    :   nsNodeDef(NT_MOUSE_MOTION, NT_GROUP_USER_INPUT, "Mouse Motion")
{
    m_size.x = NS_MAKE_NODE_DEF_WIDTH(nsString("Mouse Motion").size());
    m_palette = nsSystemPalette::getPalette(NS_SPE_INPUT);

    NS_CREATE_FLOAT_SOCKET( NS_SOCK_IN, "ScaleX", gkMouseNode::SCALE_X, NS_LINEAR_MIN,  NS_LINEAR_MAX,  1);
    NS_CREATE_FLOAT_SOCKET( NS_SOCK_IN, "ScaleY", gkMouseNode::SCALE_Y, NS_LINEAR_MIN,  NS_LINEAR_MAX,  1);

    NS_CREATE_BOOL_SOCKET( NS_SOCK_OUT, "Motion",     gkMouseNode::MOTION, 0, 1,  false);
    NS_CREATE_FLOAT_SOCKET(NS_SOCK_OUT, "RelX",       gkMouseNode::REL_X, NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
    NS_CREATE_FLOAT_SOCKET(NS_SOCK_OUT, "RelY",       gkMouseNode::REL_Y, NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
    NS_CREATE_FLOAT_SOCKET(NS_SOCK_OUT, "AbsX",       gkMouseNode::ABS_X, NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
    NS_CREATE_FLOAT_SOCKET(NS_SOCK_OUT, "AbsY",       gkMouseNode::ABS_Y, NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
    NS_CREATE_FLOAT_SOCKET(NS_SOCK_OUT, "Wheel",      gkMouseNode::WHEEL, NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
    NS_CREATE_BOOL_SOCKET( NS_SOCK_OUT, "WheelMoved", gkMouseNode::WHEEL_MOTION, 0, 1,  false);

    alignInputs(NS_SAL_BOTTOM);
    alignOutputs(NS_SAL_TOP);
}

// ----------------------------------------------------------------------------
nsNodeData* nsMouseMotionNode::getData(void)
{
    return new nsNodeData(this);
}


// ----------------------------------------------------------------------------
nsNodePropertyPage *nsMouseMotionNode::getEditor(nsPropertyManager *parent)
{ 
    return new nsNodePropertyPage(parent, this); 
}
