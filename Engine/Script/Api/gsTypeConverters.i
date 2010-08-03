%{
static int gsGetProperty(lua_State *L, const gsProperty& prop)
{
	int SWIG_arg= 0;
	switch (prop.getType())
	{
	case PROP_BOOL:
		lua_pushboolean(L, prop.toBool()); 
		SWIG_arg++;
		break;
	case PROP_NUMBER:
		lua_pushnumber(L, prop.toNumber()); 
		SWIG_arg++;
		break;
	case PROP_STRING:
		{
			gkString str = prop.toString();
			lua_pushlstring(L, str.c_str(), str.size());
			SWIG_arg++;
			break;
		}
	}
	return SWIG_arg;
}
%}
