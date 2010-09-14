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
#include "nsNodes.h"
#include "nsDialogBase.h"
#include "nsMainWindow.h"

// Inputs


NS_IMPLEMENT_NODE(nsMouseMotionNode, nsMouseMotionDialog, nsNodeData)
	:   nsNodeDef(NT_MOUSE_MOTION, NT_GROUP_USER_INPUT, "Mouse Motion")
{
	m_palette = nsSystemPalette::getPalette(NS_SPE_INPUT);

	NS_CREATE_FLOAT_SOCKET( NS_SOCK_IN, "ScaleX",     0, NS_LINEAR_MIN,  NS_LINEAR_MAX,  1);
	NS_CREATE_FLOAT_SOCKET( NS_SOCK_IN, "ScaleY",     1, NS_LINEAR_MIN,  NS_LINEAR_MAX,  1);

	NS_CREATE_BOOL_SOCKET( NS_SOCK_OUT, "Motion",     2, 0, 1,  false);
	NS_CREATE_FLOAT_SOCKET(NS_SOCK_OUT, "RelX",       3, NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
	NS_CREATE_FLOAT_SOCKET(NS_SOCK_OUT, "RelY",       4, NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
	NS_CREATE_FLOAT_SOCKET(NS_SOCK_OUT, "AbsX",       5, NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
	NS_CREATE_FLOAT_SOCKET(NS_SOCK_OUT, "AbsY",       6, NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
	NS_CREATE_FLOAT_SOCKET(NS_SOCK_OUT, "Wheel",      7, NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
	NS_CREATE_BOOL_SOCKET( NS_SOCK_OUT, "WheelMoved", 8, 0, 1,  false);

	alignInputs(NS_SAL_BOTTOM);
	alignOutputs(NS_SAL_TOP);
}


NS_IMPLEMENT_NODE(nsMouseButtonNode, nsMouseButtonDialog, nsButtonData)
	:   nsNodeDef(NT_MOUSE_BUTTON, NT_GROUP_USER_INPUT, "Mouse Button")
{
	m_palette = nsSystemPalette::getPalette(NS_SPE_INPUT);

	NS_CREATE_BOOL_SOCKET( NS_SOCK_IN,   "Update",    0, 0, 1,  true);
	NS_CREATE_BOOL_SOCKET( NS_SOCK_OUT,  "IsDown",    1, 0, 1,  false);
	NS_CREATE_BOOL_SOCKET( NS_SOCK_OUT,  "IsNotDown", 2, 0, 1,  true);

	alignInputs(NS_SAL_BOTTOM);
	alignOutputs(NS_SAL_TOP);
}




NS_IMPLEMENT_NODE(nsObjectMotionNode, nsObjectMotionDialog, nsMotionData)
	:   nsNodeDef(NT_OBJECT_MOTION, NT_GROUP_OUTPUT, "Object Motion")
{
	m_palette = nsSystemPalette::getPalette(NS_SPE_OUTPUT);


	NS_CREATE_BOOL_SOCKET( NS_SOCK_IN, "Update",    0, 0,  1,  true);
	NS_CREATE_FLOAT_SOCKET( NS_SOCK_IN, "X",        1, NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
	NS_CREATE_FLOAT_SOCKET( NS_SOCK_IN, "Y",        2, NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
	NS_CREATE_FLOAT_SOCKET( NS_SOCK_IN, "Z",        3, NS_LINEAR_MIN,  NS_LINEAR_MAX,  0);
	NS_CREATE_FLOAT_SOCKET( NS_SOCK_IN, "Damping",  4, 0.0, 1.0,  0);

	alignInputs(NS_SAL_BOTTOM);
	alignOutputs(NS_SAL_TOP);
}






// Socket Converters

NS_IMPLEMENT_NODE(nsBoolToIntNode, nsBoolDialog, nsNodeData)
	:   nsNodeDef(NT_BTI, NT_GROUP_CONVERTER, "Bool To Int")
{
	m_palette = nsSystemPalette::getPalette(NS_SPE_CONVERTER);

	NS_CREATE_BOOL_SOCKET( NS_SOCK_IN,  "Bool", 0,  0, 1,  false);
	NS_CREATE_INT_SOCKET(  NS_SOCK_OUT, "Int",  1,  NS_LINEAR_MIN, NS_LINEAR_MAX,  0);
}

NS_IMPLEMENT_NODE(nsBoolToFloatNode, nsBoolDialog, nsNodeData)
	:   nsNodeDef(NT_BTF, NT_GROUP_CONVERTER, "Bool To Float")
{
	m_palette = nsSystemPalette::getPalette(NS_SPE_CONVERTER);

	NS_CREATE_BOOL_SOCKET(   NS_SOCK_IN,  "Bool", 0,  0, 1,  false);
	NS_CREATE_FLOAT_SOCKET(  NS_SOCK_OUT, "Float",  1,  NS_LINEAR_MIN, NS_LINEAR_MAX, 0.f);
}


NS_IMPLEMENT_NODE(nsIntToBoolNode, nsIntDialog, nsNodeData)
	:   nsNodeDef(NT_ITB, NT_GROUP_CONVERTER, "Int To Bool")
{
	m_palette = nsSystemPalette::getPalette(NS_SPE_CONVERTER);

	NS_CREATE_INT_SOCKET(   NS_SOCK_IN,  "Int",  0,  NS_LINEAR_MIN, NS_LINEAR_MAX,  0);
	NS_CREATE_BOOL_SOCKET(  NS_SOCK_OUT, "Bool", 1,  0, 1, false);
}


NS_IMPLEMENT_NODE(nsIntToFloatNode, nsIntDialog, nsNodeData)
	:   nsNodeDef(NT_ITF, NT_GROUP_CONVERTER, "Int To Float")
{
	m_palette = nsSystemPalette::getPalette(NS_SPE_CONVERTER);

	NS_CREATE_INT_SOCKET(   NS_SOCK_IN,   "Int",   0,  NS_LINEAR_MIN, NS_LINEAR_MAX,  0);
	NS_CREATE_FLOAT_SOCKET(  NS_SOCK_OUT, "Float", 1,  NS_LINEAR_MIN, NS_LINEAR_MAX,  0.f);
}



NS_IMPLEMENT_NODE(nsFloatToBoolNode, nsFloatDialog, nsNodeData)
	:   nsNodeDef(NT_FTB, NT_GROUP_CONVERTER, "Float To Bool")
{
	m_palette = nsSystemPalette::getPalette(NS_SPE_CONVERTER);

	NS_CREATE_FLOAT_SOCKET( NS_SOCK_IN,  "Float",    0,  NS_LINEAR_MIN, NS_LINEAR_MAX,  0);
	NS_CREATE_BOOL_SOCKET(  NS_SOCK_OUT, "Bool",     1,  0, 1, false);
}



NS_IMPLEMENT_NODE(nsFloatToIntNode, nsFloatDialog, nsNodeData)
	:   nsNodeDef(NT_FTI, NT_GROUP_CONVERTER, "Float To Int")
{
	m_palette = nsSystemPalette::getPalette(NS_SPE_CONVERTER);

	NS_CREATE_FLOAT_SOCKET(   NS_SOCK_IN,  "Float",  0,  NS_LINEAR_MIN, NS_LINEAR_MAX,  0.f);
	NS_CREATE_INT_SOCKET(     NS_SOCK_OUT, "Int",    1,  NS_LINEAR_MIN, NS_LINEAR_MAX,  0);
}
