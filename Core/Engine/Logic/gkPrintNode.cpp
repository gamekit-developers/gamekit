/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "gkPrintNode.h"
#include "gkConsole.h"
#include <stdio.h>

using namespace Ogre;


// ----------------------------------------------------------------------------
gkPrintNode::gkPrintNode(gkLogicTree *parent, size_t id) :
		gkLogicNode(parent, NT_PRINT, id)
{
	ADD_ISOCK(mSockets[0], this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(mSockets[1], this, gkLogicSocket::ST_VARIABLE);
}

// ----------------------------------------------------------------------------
bool gkPrintNode::evaluate(Real tick)
{
	return mSockets[0].getValueBool();
}

// ----------------------------------------------------------------------------
void gkPrintNode::update(Real tick)
{
	// TODO: print to gui
	printf("%s\n", mSockets[1].getValueString().c_str());
}
