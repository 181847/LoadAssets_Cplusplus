// LoadAssets.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "LuaInterpreter.h"
#include <memory>

static int addfunction(lua_State * L)
{
	int n1 = luaL_checkinteger(L, 1);
	int n2 = luaL_checkinteger(L, 2);
	lua_pushinteger(L, n1 + n2);
	return 1;
}


int main()
{
	std::unique_ptr<LuaInterpreter> luaInter = std::make_unique<LuaInterpreter>();

	luaInter->m_L;

	lua_pushcfunction(luaInter->m_L, addfunction);

	lua_setglobal(luaInter->m_L, "add");

	luaInter->Run();

	
	fprintf(stdout, "Program terminate.");
	getchar();
    return 0;
}

