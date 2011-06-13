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

#include "akBufferInfo.h"

akBufferInfo::akBufferInfo() : m_size(0)
{
}

akBufferInfo::~akBufferInfo()
{
	m_elements.clear();
}

void akBufferInfo::addElement(int dataUsage, int dataType, int stride, void* data, void* base)
{
	akBufferInfo::Element elem;
	
	elem.dataUsage = dataUsage;
	elem.dataType = dataType;
	elem.stride = stride;
	elem.data = data;
	elem.base = base;
	
	m_elements.push_back(elem);
}


bool akBufferInfo::getElement(const int dataUsage, const int dataType, const unsigned int occurrence, void **dataOut, unsigned int* strideOut) const
{
	int i, occur=0;
	
	for(i=0; i<m_elements.size(); i++)
	{
		if(m_elements.at(i).dataUsage == dataUsage && m_elements.at(i).dataType == dataType )
		{
			occur +=1;
			
			if(occur >= occurrence)
			{
				if(dataOut)
					*dataOut = m_elements.at(i).data;
				if(strideOut)
					*strideOut = m_elements.at(i).stride;
				
				return true;
			}
		}
	}
	if(dataOut)
		*dataOut = 0;
	if(strideOut)
		*strideOut = 0;
	return false;
}

const akBufferInfo::Element* akBufferInfo::getElement(const int dataUsage, const int dataType, const unsigned int occurrence) const
{
	int i, occur=0;
	
	for(i=0; i<m_elements.size(); i++)
	{
		if(m_elements.at(i).dataUsage == dataUsage && m_elements.at(i).dataType == dataType )
		{
			occur +=1;
			
			if(occur >= occurrence)
			{
				return &m_elements.at(i);
			}
		}
	}
	return 0;
}

bool akBufferInfo::removeElement(const void *elemData)
{
	for(int i=0; i<m_elements.size(); i++)
	{
		if(m_elements.at(i).data == elemData)
		{
			m_elements.erase(m_elements.at(i));
			return true;
		}
	}
	return false;
}

