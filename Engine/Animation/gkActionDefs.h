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
#ifndef _gkActionDefs_h_
#define _gkActionDefs_h_



enum gkActionEvalMode
{
	///Reset loop when done.
	GK_ACT_LOOP    = (1 << 0),
	///Play till the end and stop.
	GK_ACT_END     = (1 << 1),
	///Invert frames
	GK_ACT_INVERSE = (1 << 2)
};

class gkActionPlayer;
class gkAction;
class gkActionBlender;
class gkActionBlend;
class gkKeyedAction;
class gkActionSequence;
class gkBezierSpline;
class gkTransformChannel;
class gkObjectChannel;
class gkBoneChannel;


#endif//_gkActionDefs_h_
