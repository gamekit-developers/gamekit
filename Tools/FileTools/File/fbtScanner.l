/*
-------------------------------------------------------------------------------
    This file is part of FBT (File Binary Table.).
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
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <memory.h>
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

// Scalar types are defined as [a-z]"Scalar"
#ifdef FBT_SCALAR_DOUBLE
#define scalar_t double
#else
#define scalar_t float
#endif

typedef enum fbtTokenID {

    COMMA       = ',',
    POINTER     = '*',
    LBRACE      = '[',
    RBRACE      = ']',
    LPARN       = '(',
    RPARN       = ')',
    LBRACKET    = '{',
    RBRACKET    = '}',
    TERM        = ';',


    IDENTIFIER = 256,
    CHAR,
    SHORT,
    INT,
    LONG,
    FLOAT,
    DOUBLE,
    SCALAR,
    VOID,
    FUNCTION_POINTER_BEG,
    FUNCTION_POINTER_END,
    CONSTANT,

    NAMESPACE,
    CLASS,
    STRUCT,
    UNION, 

    NULL_TOKEN,
}fbtTokenID;

#define FBT_TOK_IS_TYPE(x) (x >= IDENTIFIER && x <= VOID)
#define ECHO 

int line = 1;
fbtFixedString<270> __source;



%}
%option noyywrap
%option never-interactive
%s OSTRC ISTRC IGENUM ISENUM ICMT GCMT PSCMT PRIVSEC INSP SSTRC

D           [0-9]
L           [a-zA-Z_]
H           [a-fA-F0-9]
E           [Ee][+-]?{D}+
FS          (f|F|l|L)
IS          (u|U|l|L)*
l           [a-z]
WS          [ \t]+
ID          {L}({L}|{D})*
FWDCLS      "class"{WS}*{ID}*{WS}*";"
FWDSTC      "struct"{WS}*{ID}*{WS}*";"
TYPETERM    "}"{WS}*{ID}*{WS}*";"


PRE         "#"{L}[^\n\r]+



%{
FBTsize         m_lastConstant;


struct fbtToken
{
    fbtTokenID  m_cur;
    char*       m_buf;
    FBTsize     m_constantSize;
    FBTsize     m_line;
    char*       m_src;
};



%}


%%

<OSTRC>"\n"                 { line ++; }
<OSTRC>"\r\n"               { line ++; }
<OSTRC>"\r"                 { line ++; }
<OSTRC>{FWDCLS}*            { }
<OSTRC>{FWDSTC}*            { }
<OSTRC>"namespace"          { BEGIN INSP;  return (NAMESPACE); }
<OSTRC>"struct"             { BEGIN ISTRC; return STRUCT; }
<OSTRC>"class"              { BEGIN ISTRC; return STRUCT; }
<OSTRC>"enum"               { BEGIN IGENUM; }
<OSTRC>{PRE}*               { }
<OSTRC>"#"                  { BEGIN SSTRC; }
<OSTRC>"/*"                 { BEGIN GCMT; }
<OSTRC>"//"[^\n\r]+         { }
<OSTRC>[.]+                 { }

<INSP>{L}({L}|{D})*         { return IDENTIFIER; }
<INSP>"{"                   { BEGIN OSTRC; }

<ISTRC>"/*"                 { BEGIN ICMT; }
<ISTRC>"//"[^\n\r]+         { }
<ISTRC>"privsec:"           { BEGIN PRIVSEC;}
<ISTRC>"public:"            { }
<ISTRC>"private:"           { }
<ISTRC>"protected:"         { }
<ISTRC>"enum"               { BEGIN ISENUM; }
<ISTRC>"("[ \t\*]+          { return FUNCTION_POINTER_BEG; }
<ISTRC>")"[^;]+             { return FUNCTION_POINTER_END; }
<ISTRC>"char" |
<ISTRC>"uchar"              { return CHAR; }
<ISTRC>"short" |           
<ISTRC>"ushort"             { return SHORT; }
<ISTRC>"int" |
<ISTRC>"uint"               { return INT; }
<ISTRC>"long" |        
<ISTRC>"ulong"              { return LONG;      }
<ISTRC>"float"              { return FLOAT;     }
<ISTRC>"double"             { return DOUBLE;    }
<ISTRC>[a-z]+"Scalar"       { return SCALAR;    }
<ISTRC>"void"               { return VOID;      }
<ISTRC>"struct"             { }
<ISTRC>"class"              { }
<ISTRC>"signed"             { }
<ISTRC>"unsigned"           { }
<ISTRC>"static"             { }
<ISTRC>"typedef"            { }
<ISTRC>"*"                  { return POINTER;       }
<ISTRC>"["                  { return LBRACE;        }
<ISTRC>"]"                  { return RBRACE;        }
<ISTRC>"{"                  { return LBRACKET;      }
<ISTRC>"("                  { return LPARN;         }
<ISTRC>")"                  { return RPARN;         }
<ISTRC>","                  { return COMMA;         }
<ISTRC>";"                  { return TERM;          }
<ISTRC>{L}({L}|{D})*        { return IDENTIFIER;    }
<ISTRC>{TYPETERM}*          { BEGIN OSTRC; return RBRACKET; }
<ISTRC>{D}+{IS}?            { m_lastConstant = atoi(yytext); return(CONSTANT); }
<ISTRC>"\n"                 { line ++; }
<ISTRC>"\r\n"               { line ++; }
<ISTRC>"\r"                 { line ++; }
<ISTRC>[ \t\v\f]            { }


<ICMT>"\n"                  { line ++; }
<ICMT>"\r\n"                { line ++; }
<ICMT>"\r"                  { line ++; }
<ICMT>"*/"                  { BEGIN ISTRC; }
<ICMT>[.]+                  { }

<GCMT>"\n"                  { line ++; }
<GCMT>"\r\n"                { line ++; }
<GCMT>"\r"                  { line ++; }
<GCMT>"*/"                  { BEGIN OSTRC; }
<GCMT>[.]+                  { }

<PSCMT>"\n"                 { line ++; }
<PSCMT>"\r\n"               { line ++; }
<PSCMT>"\r"                 { line ++; }
<PSCMT>"*/"                 { BEGIN PRIVSEC; }
<PSCMT>[.]+                 { }


<PRIVSEC>"/*"               { BEGIN PSCMT; }
<PRIVSEC>"//"[^\n\r]+       { }
<PRIVSEC>"\n"               { line ++; }
<PRIVSEC>"\r\n"             { line ++; }
<PRIVSEC>"\r"               { line ++; }
<PRIVSEC>"public:"          { BEGIN ISTRC; }
<PRIVSEC>"private:"         { BEGIN ISTRC; }
<PRIVSEC>"protected:"       { BEGIN ISTRC; }
<PRIVSEC>[.]+               { }


<SSTRC>"\n"                 { line ++;      }
<SSTRC>"\r\n"               { line ++;      }
<SSTRC>"\r"                 { line ++;      }
<SSTRC>"}"                  { BEGIN OSTRC;  }
<SSTRC>[.]+                 { }


<IGENUM>"\n"                { line ++;      }
<IGENUM>"\r\n"              { line ++;      }
<IGENUM>"\r"                { line ++;      }
<IGENUM>{TYPETERM}*         { BEGIN OSTRC;  }
<IGENUM>[.]+                { }

<ISENUM>"\n"                { line ++;      }
<ISENUM>"\r\n"              { line ++;      }
<ISENUM>"\r"                { line ++;      }
<ISENUM>{TYPETERM}*         { BEGIN ISTRC;  }
<ISENUM>[.]+                { }


%%

fbtTokenID fbtLex()
{
    return (fbtTokenID)fbtlex();
}

fbtTokenID fbtLex(fbtToken &tok)
{
    tok.m_cur           = fbtLex();
    tok.m_buf           = yytext;
    tok.m_constantSize  = m_lastConstant;
    tok.m_line          = line;
    tok.m_src           = (char*)__source.c_str();
    m_lastConstant      = -1;
    return tok.m_cur;
}

void *fbtCurFile = 0;
yy_buffer_state *fbtCurBuf = 0;

void* fbtInitParse(const char *ifile)
{
    line    = 1;
    BEGIN   OSTRC;
    __source = ifile;


    fbtCurFile = (void*)fopen(ifile, "rb");
    if (!fbtCurFile)
    {
        fbtPrintf("File '%s' loading failed!\n", ifile);
        return 0;
    }

    yyin = (FILE*)fbtCurFile;
    return fbtCurFile;
}


void* fbtInitParse(const char *name, const char *buf)
{
    line    = 1;
    BEGIN   OSTRC;
    __source = name;

    fbtCurBuf = fbt_scan_string(buf);
    return fbtCurBuf;
}

void fbtFreeParse(void *p)
{   
    if (p == fbtCurBuf)
    {
        fbt_delete_buffer(fbtCurBuf);
        return;
    }


    if (p == fbtCurFile)
        fclose((FILE*)fbtCurFile);
}
