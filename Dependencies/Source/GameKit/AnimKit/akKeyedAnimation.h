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

#ifndef AKKEYEDANIMATION_H
#define AKKEYEDANIMATION_H

#include "akAnimation.h"


class akKeyedAnimation : public akAnimation
{
public:
	typedef utArray<akAnimationChannel*> Channels;
	
protected:
	Channels             m_channels;

public:
	akKeyedAnimation();
	virtual ~akKeyedAnimation();

	UT_INLINE Channels::ConstPointer getChannels(void) const    { return m_channels.ptr(); }
	UT_INLINE int                    getNumChannels(void) const { return(int)m_channels.size(); }
	
	void addChannel(akAnimationChannel* chan);
	akAnimationChannel* getChannel(const utString& name);
	
	virtual void evaluate(const akScalar& time, const akScalar& weight, void* object) const;
};


#endif // AKKEYEDANIMATION_H
