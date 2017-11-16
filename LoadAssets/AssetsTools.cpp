#include "AssetsTools.h"

bool LuaLoadMaterial(LuaInterpreter * pLuaInter, std::unordered_map<std::string, Material> * matMap)
{
	ASSERT(pLuaInter);
	ASSERT(matMap);

	KeyGenerator<32> formater;
	auto * L = pLuaInter->m_L;

	// the MatPart is the MaterialSet which contain information 
	// about all the material.
	lua_getglobal(L, "Assemble");

	//return 0 stand for no error
	ThrowIfFalse(0 == lua_pcall(L, 0, 2, 0));


	//return false stand for no error
	ThrowIfFalse(false == lua_toboolean(L, -2));

	
	ASSERT(
		fprintf(stderr, "stack Size: %d\n", pLuaInter->GetStackSize())
		|| true);
	lua_getfield(L, -1, "MaterialQueue"); 
	ASSERT(
		fprintf(stderr, "stack Size: %d\n", pLuaInter->GetStackSize())
		|| true);

	// get the material count
	lua_getfield(L, -1, "n");
	int matCount;
	int isInt;
	matCount = lua_tointegerx(L, -1, &isInt);
	ASSERT(isInt);
	printf("material count: %d\n", matCount);
	lua_pop(L, 1);		// pop the n immediatelly

	// notice that the index start from 1.
	for (int i = 1; i < matCount; ++i)
	{
		lua_getfield(L, -1, formater.format(i));

		// ensure the top is a material,
		// call the function to get one material.
		LuaLoadSingleMaterial(pLuaInter, matMap);
	}

	return true;
}

bool LuaLoadSingleMaterial(LuaInterpreter * pLuaInter, std::unordered_map<std::string, Material>* matMap)
{
	ASSERT(pLuaInter);
	ASSERT(matMap);
	int checker;				// to check if the element is expected type.
	KeyGenerator<32> formater;	// to generatoer the key string

	lua_State * L = pLuaInter->m_L;	// get for convience

	// get diffuseAlbedo, contain 4 number;
	double dAlbe = 0;
	lua_getfield(L, -1, "diffuseAlbedo");
	int clearStackSize = 2;

	for (int i = 1; i <= 4; ++i)
	{
		lua_getfield(L, -1, formater.format(i));
		lua_tonumberx(L, -1, &checker);
		ASSERT(checker);
		printf("diffuseAlbedo[%d]: %lf\n", i, dAlbe);
		lua_pop(L, 1);		// pop the number immediately
	}

	// clear the stack, include the material itself.
	lua_pop(L, clearStackSize);

	return true;
}
