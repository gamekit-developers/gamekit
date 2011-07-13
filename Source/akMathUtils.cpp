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

#include "akMathUtils.h"


akVector3 akMathUtils::getScale(const akMatrix3& m)
{
	return akVector3(length(m[0]),
					 length(m[1]),
					 length(m[2]));
}

akVector3 akMathUtils::getScale(const akMatrix4& m)
{
	return akVector3(length(m[0]),
					 length(m[1]),
					 length(m[2]));
}

akMatrix4 akMathUtils::setScale(const akMatrix4& m, const akVector3& scale)
{
	akMatrix4 ret = normalizedUpper3x3(m);
	ret[0] *= scale.getX();
	ret[1] *= scale.getY();
	ret[2] *= scale.getZ();
	return ret;
}

akMatrix3 akMathUtils::normalized(const akMatrix3& m)
{
	return akMatrix3(m[0]/length(m[0]),
					 m[1]/length(m[1]),
					 m[2]/length(m[2]));
}

akMatrix4 akMathUtils::normalizedUpper3x3(const akMatrix4& m)
{
	return akMatrix4(m[0]/length(m[0]),
					 m[1]/length(m[1]),
					 m[2]/length(m[2]),
					 m[3]);
}

akMatrix3 akMathUtils::orthoNormalized(const akMatrix3& m)
{
	//Gram-Schmidt orthogonalization.
	akMatrix3 q;
	q[0] = m[0]/length(m[0]);
	q[1] = m[1] - dot(q[0],m[1])*q[0];
	q[1] /= length(q[1]);
	q[2] = m[2] - dot(q[0],m[2])*q[0] - dot(q[1],m[2])*q[1];
	q[2] /= length(q[2]);
	return q;
}


void akMathUtils::normalize(akMatrix3& m)
{
	m[0] /= length(m[0]);
	m[1] /= length(m[1]),
	m[2] /= length(m[2]);
}

void akMathUtils::normalizeUpper3x3(akMatrix4& m)
{
	m[0] /= length(m[0]);
	m[1] /= length(m[1]);
	m[2] /= length(m[2]);
}

void akMathUtils::orthoNormalize(akMatrix3& m)
{
	//Gram-Schmidt orthogonalization.
	m[0] = m[0]/length(m[0]);
	m[1] = m[1] - dot(m[0],m[1])*m[0];
	m[1] /= length(m[1]);
	m[2] = m[2] - dot(m[0],m[2])*m[0] - dot(m[1],m[2])*m[1];
	m[2] /= length(m[2]);
}

void akMathUtils::extractTransform(const akMatrix4& mat, akVector3& loc, akQuat& rot, akVector3& scale)
{
	loc = mat.getTranslation();
	scale = getScale(mat);
	
	akMatrix3 m = mat.getUpper3x3();
	m[0] /= scale.getX();
	m[1] /= scale.getY();
	m[2] /= scale.getZ();
	
	rot = akQuat(m);
	rot = Vectormath::Aos::normalize(rot);
}

