#include "LuaInterpreter.h"

LuaInterpreter::LuaInterpreter()
{
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
}


LuaInterpreter::~LuaInterpreter()
{
	lua_close(m_L);
}

void LuaInterpreter::Run()
{
	fprintf(stdout, ">>");
	while (Not(stop) && fgets(buffer, sizeof(buffer), stdin) != NULL)
	{
		error = luaL_loadstring(m_L, buffer) || lua_pcall(m_L, 0, 0, 0);
		if (error)
		{
			fprintf(stderr, "error:\t%s\n", lua_tostring(m_L, -1));
			lua_pop(m_L, 1);
		}
		fprintf(stdout, ">>");
	}
	
	stop = true;
}
