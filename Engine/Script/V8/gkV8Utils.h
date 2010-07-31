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
#ifndef _gkV8Utils_h_
#define _gkV8Utils_h_



#include <v8.h>
#include "gkCommon.h"

extern void v8HandleException(v8::TryCatch &caught);
extern void v8RegisterBuiltins(v8::Handle<v8::ObjectTemplate> &globals);
extern void v8CollectionStep(int nr, int factor);
extern void v8Collect();


typedef v8::Persistent<v8::Script>          v8Script;
typedef v8::Persistent<v8::ObjectTemplate>  v8Globals;
typedef v8::Persistent<v8::Context>         v8Context;
typedef v8::Persistent<v8::ObjectTemplate>  v8Module;

typedef v8Module (*v8BindModule)(v8Globals &);



#endif//_gkV8Utils_h_
