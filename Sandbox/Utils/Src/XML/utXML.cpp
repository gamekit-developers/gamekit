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
#include <stdio.h>
#include "utXML.h"
#include "utStream.h"
#include "expat/expat.h"

#define BLK_SIZE 4096


class utXMLBuilderPrivate
{
public:
    utXMLHandler *handler;

    static void BuilderStart(utXMLBuilderPrivate *bld,
                             const XML_Char *name,
                             const XML_Char **atts) {
        if (!bld || !bld->handler) return;

        utXMLHandler *proc = bld->handler;
        utXMLHandler::AttributeList attr;

        int j = 0;
        while (atts[j] != 0) {
            utXMLAttributePair pair;
            pair.key = atts[j+0];
            pair.val = atts[j+1];
            attr.push_back(pair);
            j += 2;
        }

        proc->startElement(name, attr);
    }

    static void BuilderEnd(utXMLBuilderPrivate *bld,
                           const XML_Char *name) {
        if (!bld || !bld->handler) return;
        bld->handler->endElement(name);
    }
};



utXMLDocument::utXMLDocument(utXMLHandler *handler)
:       m_handler(handler)
{
}

utXMLDocument::~utXMLDocument()
{
}


void utXMLDocument::parse(const char *file)
{

    utFileStream fs;
    if (!fs.open(file, "rb")) {
        printf("File loading failed.\n");
        return;
    }


    XML_Parser parser = XML_ParserCreate(0);
    if (!parser) return;


    utXMLBuilderPrivate bld;
    bld.handler = m_handler;

    XML_SetUserData(parser, &bld);
    XML_SetStartElementHandler(parser, (XML_StartElementHandler)utXMLBuilderPrivate::BuilderStart);
    XML_SetEndElementHandler(parser, (XML_EndElementHandler)utXMLBuilderPrivate::BuilderEnd);


    do {
        void *buf = XML_GetBuffer(parser, BLK_SIZE);
        if (!buf) {
            printf("Failed tor get buffer.\n");
            break;
        }


        UTint32 br = 0;
        if ((br = fs.read(buf, BLK_SIZE)) < 0) {
            printf("Invalid read.\n");
            break;
        }

        int status = XML_ParseBuffer(parser, BLK_SIZE, br == 0);

        if (status == XML_STATUS_ERROR && !fs.eof())
        {
            XML_Error code = XML_GetErrorCode(parser);

            if (code != XML_ERROR_NONE)
            {
                printf("%s:%i\n", XML_ErrorString(code), XML_GetCurrentLineNumber(parser));
            }
        }

    } while (!fs.eof());




    XML_ParserFree(parser);
}
