// LoadAssets.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../../Library/Lua/LuaInterpreter/LuaInterpreter.h"
#include "AssetsTools.h"
#pragma comment(lib, "LuaInterpreter.lib")

extern const int gNumFrameResources = 3;

void execute()
{
	std::unique_ptr<Lua::LuaInterpreter> luaInter = std::make_unique<Lua::LuaInterpreter>();

	// call init lua to start assets loading.
	luaInter->DoFile("Init.lua");

	int a, b, c;
	luaInter->GetGlobal("t")
		->GetFieldOnTop("a")
			->ToIntegerAndPop(&a)
		->GetFieldOnTop("b")
			->ToIntegerAndPop(&b)
		->GetFieldOnTop("c")
			->ToIntegerAndPop(&c);

	printf("a:%d, b:%d, c:%d\n", a, b, c);

	/*
	// The map to store the material data.
	std::vector<Material> globalMaterial;
	std::vector<std::unique_ptr<MeshGeometry>> globalGeometrys;
	// push one empty material into the vector.
	// so the indices in the vectore is same 
	// as in the lua.
	Material placeHolder;
	globalMaterial.push_back(placeHolder);
	

	// call the Assemble function once
	lua_getglobal(luaInter->m_L, "Assemble");
	//return 0 stand for no error
	ThrowIfFalse(0 == lua_pcall(luaInter->m_L, 0, 2, 0));
	//return false stand for no error
	ThrowIfFalse(false == lua_toboolean(luaInter->m_L, -2));


	// Load all the material int the map,
	// the name will be the key.

	luaInter->GetFieldOnTop("MaterialQueue");
	//LuaLoadMaterial(luaInter.get(), &globalMaterial);
	luaInter->Pop();
	// pop the MaterialQueue

	luaInter->GetFieldOnTop("GeometryQueue");
	//LuaLoadGeometrys(luaInter.get(), &globalGeometrys, nullptr, nullptr);
	luaInter->Pop();
	// pop the GeometryQueue



	for (auto & m : globalMaterial)
	{
		//ShowDetail(m);
		putchar('\n');
	}
	/*
	DEBUG_MESSAGE("geoname\t\tindexbytesize\t\tvertexbytesize\t\t\n");
	for (auto & g : globalGeometrys)
	{
		DEBUG_MESSAGE("%s\t\t%d\t\t%d\t\t\t\n", g->Name.c_str(), g->IndexBufferByteSize, g->VertexBufferByteSize);
	}
	*/
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

