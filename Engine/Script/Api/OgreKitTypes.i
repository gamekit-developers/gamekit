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
