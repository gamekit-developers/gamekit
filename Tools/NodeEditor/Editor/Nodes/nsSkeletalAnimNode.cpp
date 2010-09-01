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
#include "Nodes/nsSkeletalAnimNode.h"
#include "Editors/nsSocketEditors.h"
#include "nsNodeProperties.h"
#if 0
#include "Logic/gkAnimationNode.h"


// ----------------------------------------------------------------------------
nsSkeletalAnimNode::nsSkeletalAnimNode() 
    :   nsNodeDef(NT_SKELETON, NT_GROUP_ANIMATION, "Animation")
{
    m_size.x = NS_MAKE_NODE_DEF_WIDTH(nsString("NotHasReachedEnd").size());
    m_palette = nsSystemPalette::getPalette(NS_SPE_ANIMATION);


    NS_CREATE_STRING_SOCKET(NS_SOCK_IN, "AnimName", gkAnimationNode::ANIM_NAME, NS_STRING_MIN,  NS_STRING_MAX,  "");
    NS_CREATE_FLOAT_SOCKET( NS_SOCK_IN, "BlendFrames", gkAnimationNode::BLEND_FRAMES, 0.0,  NS_LINEAR_MAX,  10);
    NS_CREATE_OBJECT_SOCKET(NS_SOCK_IN, "Target", gkAnimationNode::TARGET, NS_STRING_MIN,  NS_STRING_MAX,  "");

    NS_CREATE_STRING_SOCKET(NS_SOCK_OUT, "AnimName", gkAnimationNode::CURRENT_ANIM_NAME, NS_STRING_MIN,  NS_STRING_MAX,  "");
    NS_CREATE_BOOL_SOCKET(NS_SOCK_OUT, "HasReachedEnd", gkAnimationNode::HAS_REACHED_END, 0.0, 1.0, false);
    NS_CREATE_BOOL_SOCKET(NS_SOCK_OUT, "NotHasReachedEnd", gkAnimationNode::NOT_HAS_REACHED_END, 0.0, 1.0, true);
}


// ----------------------------------------------------------------------------
nsNodeData* nsSkeletalAnimNode::getData(void)
{
    return new nsNodeData(this);
}

// ----------------------------------------------------------------------------
nsNodePropertyPage *nsSkeletalAnimNode::getEditor(nsPropertyManager *parent)
{ 
    return new nsNodePropertyPage(parent, this); 
}
#endif