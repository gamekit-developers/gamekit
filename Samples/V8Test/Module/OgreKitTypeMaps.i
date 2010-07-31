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

%typemap(in,checkfn="V8_IsFunction") CallbackObject
%{ $1 = v8::Persistent<v8::Function>::New(v8::Local<v8::Function>::Cast($input->ToObject())); %}


%typemap(in,checkfn="V8_IsString") gkString %{
    $1 = gkString((const char*)V8String($input));
%}

%typemap(in,checkfn="V8_IsString") gkString&, const gkString& (gkString temp) %{
    temp = gkString((const char*)V8String($input));
    $1 = &temp;
%}


%typemap(out) gkString, gkString&, const gkString& %{ 
    return v8::String::New($1.c_str());
%}

