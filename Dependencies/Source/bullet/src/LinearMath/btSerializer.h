/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2009 Erwin Coumans  http://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BT_SERIALIZER_H
#define BT_SERIALIZER_H

#include "btScalar.h" // has definitions like SIMD_FORCE_INLINE
#include "btStackAlloc.h"

///Allow to serialize data in a chunk format
class btSerializer
{
	public:

		virtual ~btSerializer() {}

		virtual	void*	allocate(size_t size) = 0;

		virtual void	addStruct(	const	char* structType,void* data, int len, void* oldPtr, int code ) = 0;
};

struct	btChunk
{
	const char*	m_structType;
	void*		m_data;
	int			m_length;
	void*		m_oldPtr;
	int			m_code;
};

class btDefaultSerializer
{

	btStackAlloc	m_stack;

	btAlignedObjectArray<btChunk>	m_chunks;

	public:

		btDefaultSerializer(int totalSize)
			:m_stack(totalSize)
		{
		}

		virtual ~btDefaultSerializer() 
		{
		}

		virtual	void*	allocate(size_t size)
		{
			return m_stack.allocate(size);
		}

		virtual void	addStruct(	btChunk& chunk )
		{
			m_chunks.push_back(chunk);
		}
};


#endif //BT_SERIALIZER_H

