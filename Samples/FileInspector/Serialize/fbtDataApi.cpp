/*
-------------------------------------------------------------------------------
    This file is part of FBT (File Binary Tables).
    http://gamekit.googlecode.com/

    Copyright (c) 2010 Charlie C & Erwin Coumans.

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
#define FBT_IN_SOURCE
#include "fbtDataApi.h"
#include "fbtTypes.h"
#include "fbtPlatformHeaders.h"

char* fbtPtrCopy(const char* old, int len)
{
	char* ret = new char [len + 1];
	fbtMemcpy(ret, old, len);
	ret [len] = 0;
	return ret;
}





fbtText::fbtText()
	:   m_next(0), m_prev(0), m_data(0),
	    m_size(0), m_flag(0), m_textFile(0)

{
	m_name[0] = 0;
}

fbtText::~fbtText()
{
	clear();
}


void fbtText::clear(void)
{
	delete []m_data;
	m_data = 0;
	m_size = 0;
}


void fbtText::copy(const char* buffer, int len)
{
	if (buffer && len > 0)
	{
		clear();

		m_size = len;
		m_data = fbtPtrCopy(buffer, len);
	}
}



fbtFileList::fbtFileList()
	:   m_first(0), m_last(0)
{
}


void fbtFileList::remove(fbtText* fp)
{
	if (!fp)
		return;
	if (fp->m_next)
		fp->m_next->m_prev = fp->m_prev;
	if (fp->m_prev)
		fp->m_prev->m_next = fp->m_next;
	if (m_last == fp)
		m_last = fp->m_prev;
	if (m_first == fp)
		m_first = fp->m_next;
}


void fbtFileList::push_back(fbtText* fp)
{
	if (!fp)
		return;

	fp->m_prev = m_last;
	if (m_last) m_last->m_next = fp;
	if (!m_first) m_first = fp;

	m_last = fp;
}





fbtProjectFile::fbtProjectFile()
	:   m_version(INSPECTOR_VERSION_FILE),
	    m_windowLayout(0),
	    m_layoutSize(0),
	    m_flags(0)
{
}


fbtProjectFile::~fbtProjectFile()
{
	if (m_windowLayout)
		delete []m_windowLayout;

	fbtText* iter = m_projectFiles.m_first, *tmp;


	while (iter)
	{
		tmp = iter;
		iter = iter->m_next;


		tmp->clear();


		if (tmp->m_flag & fbtText::FILE_OWN)
			delete tmp;
	}
}



void fbtProjectFile::addFile(fbtText* text)
{
	if (text)
	{
		if (text->m_data)
			text->m_data = fbtPtrCopy(text->m_data, text->m_size);

		m_projectFiles.push_back(text);
	}
}

void fbtProjectFile::removeFile(fbtText* text)
{
	if (!text)
		return;

	m_projectFiles.remove(text);

	text->clear();
	if (text->isOwner())
		delete text;

}


fbtText* fbtProjectFile::getFile(const char* fname)
{
	fbtText* iter = m_projectFiles.m_first;
	while (iter)
	{
		if (fbtCharEq(iter->m_name, fname))
			return iter;
		iter = iter->m_next;
	}
	return 0;
}


bool fbtProjectFile::hasFile(const char* fname)
{
	fbtText* iter = m_projectFiles.m_first;
	while (iter)
	{
		if (fbtCharEq(iter->m_name, fname))
			return true;
		iter = iter->m_next;
	}
	return false;
}



void fbtProjectFile::setLayout(const char* windowLayout, int len)
{
	if (!windowLayout || !len)
		return;

	if (m_windowLayout)
		delete []m_windowLayout;


	m_layoutSize    = len;
	m_windowLayout  = fbtPtrCopy(windowLayout, m_layoutSize);
}



void fbtProjectFile::copy(const fbtProjectFile* rhs)
{
	m_version = rhs->m_version;
	if (rhs->m_windowLayout)
		setLayout(rhs->m_windowLayout, rhs->m_layoutSize);
}
