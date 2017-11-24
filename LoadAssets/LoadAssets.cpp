// LoadAssets.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "AssetsTools.h"

extern const int gNumFrameResources = 3;

void execute()
{
	std::unique_ptr<LuaInterpreter> luaInter = std::make_unique<LuaInterpreter>();

	// call init lua to start assets loading.
	int error = luaL_loadfile(luaInter->m_L, "Init.lua") || lua_pcall(luaInter->m_L, 0, 0, 0);
	

	ASSERT(Not(error));
	luaInter->Run();

	// The map to store the material data.
	std::vector<Material> globalMaterial;
	std::vector<std::unique_ptr<MeshGeometry>> globalGeometrys;
	// push one empty material into the vector.
	// so the indices in the vectore is same 
	// as in the lua.
	Material placeHolder;
	globalMaterial.push_back(placeHolder);
	

	// call the Assemble function once
	lua_getglobal(luaInter->m_L, "AssembleFunction");
	//return 0 stand for no error
	ThrowIfFalse(0 == lua_pcall(luaInter->m_L, 0, 2, 0));
	//return false stand for no error
	ThrowIfFalse(false == lua_toboolean(luaInter->m_L, -2));


	// Load all the material int the map,
	// the name will be the key.

	luaInter->GetFieldOnTop("MaterialQueue");
	LuaLoadMaterial(luaInter.get(), &globalMaterial);
	luaInter->GetFieldOnTop("GeometryQueue");
	//LuaLoadGeometrys(luaInter.get(), &globalGeometrys, nullptr, nullptr);

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

