/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#ifndef _gkAnimationNode_h_
#define _gkAnimationNode_h_

#include "gkLogicNode.h"

class gkAnimationNode : public gkLogicNode
{
public:
    
	enum 
	{
		ANIM_NAME,
		BLEND_FRAMES,
		TARGET,
		CURRENT_ANIM_NAME,
		HAS_REACHED_END,
		NOT_HAS_REACHED_END
	};

	DECLARE_SOCKET_TYPE(ANIM_NAME, gkString);
	DECLARE_SOCKET_TYPE(BLEND_FRAMES, gkScalar);
	DECLARE_SOCKET_TYPE(TARGET, gkGameObject*);
	DECLARE_SOCKET_TYPE(CURRENT_ANIM_NAME, gkString);
	DECLARE_SOCKET_TYPE(HAS_REACHED_END, bool);
	DECLARE_SOCKET_TYPE(NOT_HAS_REACHED_END, bool);

    gkAnimationNode(gkLogicTree *parent, size_t id);

    virtual ~gkAnimationNode() {}

    void update(gkScalar tick);
    bool evaluate(gkScalar tick);

	GK_INLINE gkLogicSocket<gkString>* getAnimName() { return GET_SOCKET(ANIM_NAME); }
    GK_INLINE gkLogicSocket<gkScalar>* getBlend() { return GET_SOCKET(BLEND_FRAMES); }
	GK_INLINE gkLogicSocket<gkGameObject*>* getTarget() {return GET_SOCKET(TARGET);}
	GK_INLINE gkLogicSocket<gkString>* getCurrentAnimName() { return GET_SOCKET(CURRENT_ANIM_NAME); }
	GK_INLINE gkLogicSocket<bool>* getHasReachedEnd() { return GET_SOCKET(HAS_REACHED_END); }
	GK_INLINE gkLogicSocket<bool>* getNotHasReachedEnd() { return GET_SOCKET(NOT_HAS_REACHED_END); }

};



#endif//_gkAnimationNode_h_
