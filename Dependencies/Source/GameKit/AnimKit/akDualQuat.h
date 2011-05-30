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

#ifndef AKDUALQUAT_H
#define AKDUALQUAT_H

#include "akMathUtils.h"

UT_ATTRIBUTE_ALIGNED_CLASS16(class) akDualQuat
{
public:
	akQuat n;
	akQuat d;

	akDualQuat();
	akDualQuat(akQuat nd, akQuat dual) : n(nd), d(dual) {};
	akDualQuat(akQuat rot, akVector3 trans);
	
	inline const akDualQuat operator +( akDualQuat v ) const
	{
		return akDualQuat(n+v.n, d+v.d);
	};
	
	inline const akDualQuat operator *( const float v ) const
	{
		return akDualQuat(n*v, d*v);
	};
	
	inline const akDualQuat operator /( const float v ) const
	{
		return akDualQuat(n/v, d/v);
	};
	
	inline akDualQuat & operator +=( const akDualQuat & v )
	{
		*this = *this + v;
		return *this;
	};
	
	inline akDualQuat & operator *=( const float & v )
	{
		*this = *this * v;
		return *this;
	};
	
	inline akDualQuat & operator /=( const float & v )
	{
		*this = *this / v;
		return *this;
	};
};

#endif // AKDUALQUAT_H
