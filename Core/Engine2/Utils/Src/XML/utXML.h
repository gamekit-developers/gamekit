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
TODO, finish. just testing expat for now 
*/
#ifndef _utXML_h_
#define _utXML_h_


#include "utCommon.h"
#include "utString.h"

typedef utString utXMLString;


// Key, Value pair for attributes
struct utXMLAttributePair
{
    utXMLString key;
    utXMLString val;
};



// Default handler, extend to build DOM tree 
class utXMLHandler
{
public:
    typedef utArray<utXMLAttributePair> AttributeList;

public:
    virtual ~utXMLHandler() {}

    virtual void startElement(const utXMLString& name, AttributeList &attrs) = 0;
    virtual void endElement(const utXMLString& name) = 0;
};


// Root document 
class utXMLDocument
{
protected:
    utXMLHandler *m_handler;

public:
    utXMLDocument(utXMLHandler *handler);
    ~utXMLDocument();
    void parse(const char *file);
};


#endif//_utXML_h_
