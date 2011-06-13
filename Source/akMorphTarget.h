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

#ifndef AKMORPHTARGET_H
#define AKMORPHTARGET_H

#include "akCommon.h"
#include "akMathUtils.h"

#include "btAlignedObjectArray.h"

#include "utTypes.h"
#include "utString.h"

class akMorphTarget
{
private:
	utHashedString m_name;
	UTuint32 m_size;
	bool m_hasNormals;
	
	utArray<UTuint32> m_indices;
	btAlignedObjectArray<akVector3> m_vertexOffsets;
	btAlignedObjectArray<akVector3> m_normals;

public:
	akMorphTarget(bool hasNormals) : m_hasNormals(hasNormals), m_size(0) {}
	akMorphTarget(UTuint32 size, bool hasNormals);
	~akMorphTarget();

	void add(UTuint32 vertexIndex, const akVector3& vertexOffset, const akVector3& normal = akVector3(0,0,0));
	void set(UTuint32 idx, UTuint32 vertexIndex, const akVector3& vertexOffset, const akVector3& normal = akVector3(0,0,0));

	UT_INLINE void setName(const utHashedString& v)
	{
		m_name = v;
	}

	UT_INLINE const utHashedString& getName(void)
	{
		return m_name;
	}
	
	UT_INLINE UTuint32 getSize(void) const
	{
		return m_size;
	}
	
	UT_INLINE const UTuint32 getIndex(UTuint32 i) const
	{
		return m_indices[i];
	}
	
	UT_INLINE const akVector3& getVertexOffset(UTuint32 i) const
	{
		return m_vertexOffsets[i];
	}
	
	UT_INLINE const akVector3& getNormal(UTuint32 i) const
	{
		return m_normals[i];
	}
};

#endif // AKMORPHTARGET_H
