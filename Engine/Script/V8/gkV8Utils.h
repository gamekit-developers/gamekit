/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-------------------------------------------------------------------------------
*/
#ifndef _gkV8Utils_h_
#define _gkV8Utils_h_



#include <v8.h>
#include "gkCommon.h"

extern void v8HandleException(v8::TryCatch & caught);
extern void v8RegisterBuiltins(v8::Handle<v8::ObjectTemplate> &globals);
extern void v8CollectionStep(int nr, int factor);
extern void v8Collect();


typedef v8::Persistent<v8::Script>          v8Script;
typedef v8::Persistent<v8::ObjectTemplate>  v8Globals;
typedef v8::Persistent<v8::Context>         v8Context;
typedef v8::Persistent<v8::ObjectTemplate>  v8Module;

typedef v8Module (*v8BindModule)(v8Globals&);



#endif//_gkV8Utils_h_
