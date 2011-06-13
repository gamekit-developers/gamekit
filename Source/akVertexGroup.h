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

#ifndef AKVERTEXGROUP_H
#define AKVERTEXGROUP_H

#include "akCommon.h"
#include "akMathUtils.h"
#include "utCommon.h"
#include "utTypes.h"
#include "utString.h"

class akVertexGroup
{
private:
	utString m_name;
	UTuint32 m_size;
	
	utArray<UTuint32> m_indices;
	utArray<akScalar> m_weights;

public:
	akVertexGroup() : m_size(0) {}
	akVertexGroup(UTuint32 size);
	~akVertexGroup();

	void add(UTuint32 vertexIndex, akScalar weight);
	void set(UTuint32 idx, UTuint32 vertexIndex, akScalar weight);

	UT_INLINE void setName(const utString& v)
	{
		m_name = v;
	}

	UT_INLINE const utString& getName(void)
	{
		return m_name;
	}
	
	UT_INLINE UTuint32 getSize(void) const
	{
		return m_size;
	}
	
	UT_INLINE UTuint32 getIndex(UTuint32 i) const
	{
		return m_indices[i];
	}
	
	UT_INLINE akScalar getWeight(UTuint32 i) const
	{
		return m_weights[i];
	}
	
	UT_INLINE UTuint32 findIndex(UTuint32 idx)
	{
		return m_indices.find(idx);
	}
};

#endif // AKVERTEXGROUP_H
