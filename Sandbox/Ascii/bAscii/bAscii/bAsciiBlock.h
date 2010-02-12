/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#ifndef _bAsciiBlockr_h_
#define _bAsciiBlockr_h_


#include "bUtils/utAllocator.h"
#include "bAsciiCommon.h"
#include "bAsciiOpCodes.h"


// ---------------------------------------------------------------------------
class bAsciiBlock : public utAllocObject
{
protected:
	const bStringF			m_type;
	const int				m_code;

	bStringF				m_name;
	bNumber					m_len;

	bAsciiBlock*			m_parent;
	bAsciiBlockArray		m_children;
	bAsciiArgumentMap		m_arguments;

public:

	bAsciiBlock(const bStringF& type, const int code);
	~bAsciiBlock();

	void addArgument(bAsciiArgument *argument);

	bAsciiArgument* getArgument(const bCode& arg);
	UT_INLINE bool hasArgument(const bCode& arg) {return m_arguments.find(arg) != UT_NPOS;}

	void setParent(bAsciiBlock *par);


	UT_INLINE bAsciiBlock*			getParent(void)				{ return m_parent; }
	UT_INLINE bool					hasParent(void)				{ return m_parent != 0; }
	UT_INLINE bAsciiBlockArray&		getChildren(void)			{ return m_children; }
	UT_INLINE bAsciiBlockIterator	getChildIterator(void)		{ return bAsciiBlockIterator(m_children);}
	UT_INLINE void					setName(const bStringF& v)	{ m_name = v;}
	UT_INLINE const bStringF&		getName(void)				{ return m_name;}
	UT_INLINE const int				getType(void)				{ return m_code;}
	UT_INLINE const bStringF&		getTypeName(void)			{ return m_type;}
	UT_INLINE void					setLength(bNumber v)		{ m_len = v;}
	UT_INLINE bNumber				getLength(void)				{ return m_len; }
};


// ---------------------------------------------------------------------------
UT_INLINE void bAsciiBlock::setParent(bAsciiBlock *par)
{
	if (par)
	{
		m_parent = par;
		m_parent->m_children.push_back(this);
	}
}

#endif//_bAsciiCompiler_h_
