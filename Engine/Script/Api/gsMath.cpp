/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Thomas Trocha(dertom)
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
#include "gsMath.h"
#include "gkMesh.h"

 gsVector3 lerp(const gsVector3& vec1,const gsVector3& vec2, float t)
 {
	 return gsVector3(gkMathUtils::interp(gkVector3(vec1),gkVector3(vec2),t));
 }

 gkVector3 getTriNormal(const gkVertex& v0,const gkVertex& v1,const gkVertex& v2)
 {
	 gkVector3 n = (v1.co - v2.co).crossProduct(v2.co - v0.co).normalisedCopy();
	 return n;
 }
