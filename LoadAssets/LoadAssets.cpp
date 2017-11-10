// LoadAssets.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "LuaInterpreter.h"
#include <memory>


int main()
{
	std::unique_ptr<LuaInterpreter> luaInter = std::make_unique<LuaInterpreter>();

	luaInter->Run();


	
	fprintf(stdout, "Program terminate.");
	getchar();
    return 0;
}

