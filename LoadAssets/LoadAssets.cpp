// LoadAssets.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "LuaInterpreter.h"
#include <memory>

void execute()
{
	std::unique_ptr<LuaInterpreter> luaInter = std::make_unique<LuaInterpreter>();

	luaInter->m_L;

	int error = luaL_loadfile(luaInter->m_L, "Init.lua") || lua_pcall(luaInter->m_L, 0, 0, 0);
	ASSERT(Not(error))

	luaInter->Run();


	fprintf(stdout, "Program terminate.");
	getchar();
}

int main()
{

	try
	{
		execute();
	}
	catch (SimpleException& e)
	{
		fprintf(stderr, e.ToString().c_str());
		getchar();
	}
	
    return 0;
}

