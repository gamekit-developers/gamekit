
%typemap(out) Property { 
    
    switch (($result).getType())
    {
    case Property::BOOL:
        lua_pushboolean(L, ($result).getValueBool());
        return 1;
    case Property::INT:
        lua_pushnumber(L, ($result).getValueInt());
        return 1;
    case Property::FLOAT:
        lua_pushnumber(L, ($result).getValueReal());
        return 1;        
    case Property::STRING:
        lua_pushstring(L, ($result).getValueString());
        return 1;
    }
    return 0;
}

