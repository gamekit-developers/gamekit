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

#ifndef AKBUFFERINFO_H
#define AKBUFFERINFO_H

#include "utTypes.h"


/// A simple class to describe a vertex/face buffer. It contains pointers
/// to the data but does not hown the memory.
class akBufferInfo
{
public:

	/// VB_DU_ELEMENT     Vertex index (for face list)
	/// VB_DU_VERTEX      Vertex position
	/// VB_DU_NORMAL      Normals
	/// VB_DU_COLOR       Vertices colors
	/// VB_DU_UV          UV coordinates often as 2 float
	/// VB_DU_BONE_IDX    Indices of the bones deforming the vertex often as 4 8bit uint
	/// VB_DU_BONE_WEIGHT Weights for the bones in the same order as the indices as 4 floats
	/// VB_DU_CUSTOM      For custom or temporary data
	enum DataUsage
	{
		BI_DU_ELEMENT,
		BI_DU_VERTEX,
		BI_DU_NORMAL,
		BI_DU_COLOR,
		BI_DU_UV,
		BI_DU_BONE_IDX,
		BI_DU_BONE_WEIGHT,
		BI_DU_CUSTOM
	};
	
	/// VB_DT_4UINT8    4 8bit uint in a 32 bit word, used for bones index
	/// VB_DT_INT32,    32 bits integer
	/// VB_DT_UINT32,   32 bits unsigned integer
	/// VB_DT_FLOAT32,  One 32 bits float
	/// VB_DT_2FLOAT32, 2 float, for UV coord for example
	/// VB_DT_3FLOAT32, 3 floats, can be asumed to be same as VB_DT_4FLOAT32 because of SIMD allignement
	/// VB_DT_4FLOAT32, 4 floats
	enum DataType
	{
		VB_DT_4UINT8,
		VB_DT_INT32,
		VB_DT_UINT32,
		VB_DT_FLOAT32,
		VB_DT_2FLOAT32,
		VB_DT_3FLOAT32,
		VB_DT_4FLOAT32,
	};
	
	/// A element of a vertex buffer, it could be a position buffer, normal buffer, weight buffer ...
	/// Decribe the data location but does not own memory.
	class Element
	{
	public:
		int dataUsage;
		int dataType;
		int stride;
		void* data;
		void* base;
		
		UT_INLINE const Element& operator= (const Element& o)
		{
			dataUsage = o.dataUsage;
			dataType = o.dataType;
			stride = o.stride;
			data = o.data;
			base = o.base;
			return *this;
		}
		
		UT_INLINE bool operator == (const Element& o) const
		{
			return ((dataUsage == o.dataUsage) &&
					(dataType == o.dataType) &&
					(stride == o.stride) &&
					(data == o.data) &&
					(base == o.base));
		}
		
		UT_INLINE UTuint32 getOffset(void) const
		{
			return (UTuint64)data-(UTuint64)base;
		}
	};

private:
	UTuint32        m_size;
	utList<Element> m_elements;
	
public:
	akBufferInfo();
	~akBufferInfo();
	
	/// Add a element to the buffer
	void addElement(int dataUsage, int dataType, int stride, void* data, void* base);
	
	/// Retrieve an element of the specified usage and type
	/// occurrence should be 1 to get the first corresponding element
	/// Elements order is the same as they are added 
	bool getElement(const int dataUsage, const int dataType, const unsigned int occurrence, void **dataOut, unsigned int* strideOut) const;
	
	const Element* getElement(const int dataUsage, const int dataType, const unsigned int occurrence) const;
	
	/// Remove an element from the buffer.
	bool removeElement(const void* elemData);
	
	/// Return the number of elements that compose the buffer.
	UT_INLINE UTuint32 getElementsCount(void) const
	{
		return m_elements.size();
	}
	
	/// Set the size of the buffer (and thus all its elements)
	UT_INLINE void setSize(UTuint32 num)
	{
		m_size = num;
	}

	/// Returns the size of the buffer.
	UT_INLINE UTuint32  getSize(void) const
	{
		return m_size;
	}
	
	UT_INLINE void clear()
	{
		m_elements.clear();
	}
};

#endif // AKBUFFERINFO_H
