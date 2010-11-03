/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#ifndef GKKEYEDACTION_H
#define GKKEYEDACTION_H

#include "gkAction.h"

class gkAnimationChannel;

class gkKeyedAction : public gkAction
{
public:
	typedef utArray<gkAnimationChannel*> Channels;
	
protected:
	Channels             m_channels;

public:
	gkKeyedAction(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkKeyedAction();

	GK_INLINE Channels::ConstPointer getChannels(void) const    { return m_channels.ptr(); }
	GK_INLINE int                    getNumChannels(void) const { return(int)m_channels.size(); }
	
	void addChannel(gkAnimationChannel* chan);
	gkAnimationChannel* getChannel(const gkString& name);
	
	virtual void evaluate(const gkScalar& time, const gkScalar& delta, const gkScalar& weight, gkGameObject* object) const;
};


#endif // GKKEYEDACTION_H
