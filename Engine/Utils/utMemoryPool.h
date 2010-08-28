/*
-------------------------------------------------------------------------------
    General Purpose Utility Library, should be kept dependency free.
    Unless the dependency can be compiled along with this library.

    Copyright (c) 2009-2010 Charlie C.
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
#ifndef _utMemoryPool_h_
#define _utMemoryPool_h_

#include "utCommon.h"
#include "utTypes.h"

// MemoryPool adapted from PocoC++

template <typename T, UTsize maxAlloc>
class utMemoryPool
{
public:
	typedef utArray<T *> BlockAlloc;

public:


	utMemoryPool(UTsize nrAlloc)
	{
		initializePool(nrAlloc);
	}

	~utMemoryPool()
	{
		for (UTsize i=0; i<m_usedBlocks.capacity(); i++)
			delete m_usedBlocks.at(i);
	}

	T *alloc(void)
	{
		if (m_usedBlocks.empty())
		{
			if (maxAlloc ==0 || m_total < maxAlloc)
			{
				m_total += 1;
				T *ptr = new T();
				return ptr;
			}
		}
		else
		{
			T *r = m_usedBlocks.back();
			m_usedBlocks.pop_back();
			return r;
		}

		printf("Maximum nr of allocs exceeded");
		return 0;
	}

	void dealloc(T *p)
	{
		if (p != 0)
			m_usedBlocks.push_back(p);
	}


	UT_INLINE UTsize        getAllocatedCount(void)     { return m_total; }
	UT_INLINE const UTsize  getMaxAlloc(void)           { return maxAlloc; }
	UT_INLINE UTsize        getBlockSize(void)          { return sizeof (T); }
	UT_INLINE UTsize        getPoolSize(void)           { return sizeof (T) * m_total; }


protected:

	void initializePool(UTsize nr)
	{
		if (nr == 0 || nr == UT_NPOS)
			return;

		m_total = nr;

		m_usedBlocks.reserve(m_total);
		for (UTsize i=0; i<nr; i++)
		{
			T *ptr = new T();
			m_usedBlocks.push_back(ptr);
		}
	}

	BlockAlloc  m_usedBlocks;
	UTsize      m_total;

};

#endif//_utMemoryPool_h_
