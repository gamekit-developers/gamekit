
#ifdef SWIGLUA
%typemap(out) gksProperty { 
    
    switch (($result).getType())
    {
    case gksProperty::BOOL:
        lua_pushboolean(L, ($result).getValueBool());
        return 1;
    case gksProperty::INT:
        lua_pushnumber(L, ($result).getValueInt());
        return 1;
    case gksProperty::FLOAT:
        lua_pushnumber(L, ($result).getValueReal());
        return 1;        
    case gksProperty::STRING:
        lua_pushstring(L, ($result).getValueString());
        return 1;
    }
    return 0;
}
#else

%typemap(out) gksProperty { 
    
    switch (($result).getType())
    {
    case gksProperty::BOOL:
        return v8::Boolean::New(($result).getValueBool());
    case gksProperty::INT:
        return v8::Integer::New(($result).getValueInt());
    case gksProperty::FLOAT:
        return v8::Number::New(($result).getValueReal());
    case gksProperty::STRING:
        return v8::String::New(($result).getValueString());
    }
    return v8::Undefined();
}


#endif