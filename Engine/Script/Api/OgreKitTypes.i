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
#ifdef SWIGLUA

%typemap(in,checkfn="lua_isstring") gkString %{
    $1 = gkString((const char*)lua_tostring(L, $input));
%}
%typemap(in,checkfn="lua_isstring") gkString&, const gkString& (gkString temp) %{
    temp = gkString((const char*)lua_tostring(L, $input));
    $1 = &temp;
%}

%typecheck(SWIG_TYPECHECK_STRING) gkString, gkString&, const gkString& {
  $1 = lua_isstring(L,$input);
}

%typemap(out) gkString %{
    lua_pushstring(L, $1.c_str()); SWIG_arg++;
%}

%typemap(out) gkString&, const gkString& %{
    lua_pushstring(L, $1->c_str()); SWIG_arg++;
%}

%typemap(out) OgreKit::Property  %{ 
    
    switch (($result).getType())
    {
    case OgreKit::PROP_BOOL:
        lua_pushboolean(L, ($result).toBool()); SWIG_arg++;
        break;
    case OgreKit::PROP_NUMBER:
        lua_pushnumber(L, ($result).toNumber()); SWIG_arg++;
        break;
    case OgreKit::PROP_STRING:
        {
            OgreKit::String str = ($result).toString();
            lua_pushlstring(L, str.c_str(), str.size());
            SWIG_arg++;
            break;
        }
    }
%}

%typemap(out) OgreKit::Property&, const OgreKit::Property&  %{ 
    
    switch (($result)->getType())
    {
    case OgreKit::PROP_BOOL:
        lua_pushboolean(L, ($result)->toBool()); SWIG_arg++;
        break;
    case OgreKit::PROP_NUMBER:
        lua_pushnumber(L, ($result)->toNumber()); SWIG_arg++;
        break;
    case OgreKit::PROP_STRING:
        {
            OgreKit::String str = ($result)->toString();
            lua_pushlstring(L, str.c_str(), str.size());
            SWIG_arg++;
            break;
        }
    }
%}



%typemap(in,checkfn="lua_isfunction") OgreKit::Function
%{ $1.m_id = $input; $1.L = L; %}

%typemap(typecheck) OgreKit::Function
%{ $1 = lua_isfunction(L, $input); %}


%typemap(in,checkfn="lua_istable") OgreKit::Self
%{ $1.m_id = $input; $1.L = L; %}

%typemap(typecheck) OgreKit::Self
%{ $1 = lua_istable(L, $input); %}

#else

// TODO

#endif
