// LoadAssets.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "LuaInterpreter.h"
#include "lib/DirectX12/FrameResource.h"
#include "AssetsTools.h"

void execute()
{
	std::unique_ptr<LuaInterpreter> luaInter = std::make_unique<LuaInterpreter>();

	// call init lua to start assets loading.
	int error = luaL_loadfile(luaInter->m_L, "Init.lua") || lua_pcall(luaInter->m_L, 0, 0, 0);
	
	ASSERT(Not(error));

	// The map to store the material data.
	std::unordered_map<std::string, Material> globalMaterial;
	
	// Load all the material int the map,
	// the name will be the key.
	LuaLoadMaterial(luaInter.get(), &globalMaterial);

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

