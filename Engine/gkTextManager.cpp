/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#include "gkTextManager.h"
#include "gkTextFile.h"


gkTextManager::gkTextManager()
{
}

gkTextManager::~gkTextManager()
{
    destroyAll();
}



gkTextFile* gkTextManager::getFile(const gkString& name)
{
    size_t pos;
    if ((pos = m_files.find(name)) == GK_NPOS)
        return 0;
    return m_files.at(pos);
}



gkTextFile* gkTextManager::create(const gkString& name, const gkTextFile::TextType &type)
{
    size_t pos;
    if ((pos = m_files.find(name)) != GK_NPOS)
        return 0;

    gkTextFile *ob = new gkTextFile(this, name, type);
    m_files.insert(name, ob);
    return ob;
}


void gkTextManager::destroy(const gkString& name)
{
    size_t pos;
    if ((pos = m_files.find(name)) != GK_NPOS)
    {
        gkTextFile *ob = m_files.at(pos);
        m_files.remove(name);
        delete ob;
    }
}


void gkTextManager::destroy(gkTextFile *ob)
{
    GK_ASSERT(ob);

    gkString name = ob->getName();
    size_t pos;
    if ((pos = m_files.find(name)) != GK_NPOS)
    {
        gkTextFile *ob = m_files.at(pos);
        m_files.remove(name);
        delete ob;
    }
}



void gkTextManager::destroyAll(void)
{
    utHashTableIterator<TextFiles> iter(m_files);
    while (iter.hasMoreElements())
    {
        gkTextFile *ob = iter.peekNextValue();
        delete ob;
        iter.next();
    }

    m_files.clear();
}



bool gkTextManager::hasFile(const gkString& name)
{
    return m_files.find(name) != GK_NPOS;
}



GK_IMPLEMENT_SINGLETON(gkTextManager);
