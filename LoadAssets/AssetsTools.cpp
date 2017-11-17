#include "AssetsTools.h"

#ifdef _DEBUG
#define	ShowStackSize(luaInterpreter) printf("StackSize: %d\n", luaInterpreter->GetStackSize())
#else
#define ShowStackSize(luaInterpreter)
#endif

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

	ASSERT(
		fprintf(stderr, "stack Size: %d\n", pLuaInter->GetStackSize())
		|| true);

	// notice that the index start from 1.
	for (int i = 1; i <= matCount; ++i)
	{
		lua_geti(L, -1, i);

		// ensure the top is a material,
		// call the function to get one material.
		LuaLoadSingleMaterial(pLuaInter, matMap);

		lua_pop(L, 1);
	}

	return true;
}

bool LuaLoadSingleMaterial(LuaInterpreter * pLuaInter, std::unordered_map<std::string, Material>* matMap)
{
	ASSERT(pLuaInter);
	ASSERT(matMap);
	int checker;							// to check if the element is expected type.
	KeyGenerator<32> formater;				// to generatoer the key string

	lua_State * L = pLuaInter->m_L;			// get for convience

	// ready to get string
	const size_t StringMaxLength = 256;
	size_t StringLen = 0;					// length of string in the lua
	const char * TempString;				// obtain the string from lua

	// get material name
	char matName[StringMaxLength];
	StringLen = 0;
	lua_getfield(L, -1, "name");
	TempString = lua_tolstring(L, -1, &StringLen);
	ThrowIfFalse(StringLen > 0 && StringLen < StringMaxLength);
	strcpy_s(matName, StringMaxLength, TempString);
	lua_pop(L, 1);							// pop the name
	printf("Material Name; %s\n", matName);

	ShowStackSize(pLuaInter);
	// get diffuseAlbedo, contain 4 number;
	double dAlbe = 0;
	lua_getfield(L, -1, "diffuseAlbedo");
	// 4 diffuseAlbedo
	for (int i = 1; i <= 4; ++i)
	{
		lua_geti(L, -1, i);
		dAlbe = lua_tonumberx(L, -1, &checker);
		ASSERT(checker);
		printf("diffuseAlbedo[%d]: %lf\n", i, dAlbe);
		lua_pop(L, 1);						// pop the number immediately
	}
	lua_pop(L, 1);							// pop the diffuseAlbedo

	ShowStackSize(pLuaInter);
	// 3 FresnelR
	double fresnelR = 0;
	lua_getfield(L, -1, "fresnelR");
	for (int i = 1; i <= 3; ++i)
	{
		lua_geti(L, -1, i);
		fresnelR = lua_tonumberx(L, -1, &checker);
		ASSERT(checker);
		printf("fresnelR[%d]: %lf\n", i, fresnelR);
		lua_pop(L, 1);
	}
	lua_pop(L, 1);							// pop fresnelR

	// roughness is just one number
	double roughness = 0;
	lua_getfield(L, -1, "roughness");
	roughness = lua_tonumberx(L, -1, &checker);
	printf("roughness: %lf\n", roughness);
	lua_pop(L, 1);							// pop roughness


	//diffuseMap
	char diffuseMapName[StringMaxLength];
	TempString = nullptr;
	ShowStackSize(pLuaInter);
	lua_getfield(L, -1, "diffuseMap");
	ShowStackSize(pLuaInter);
	// does the material contain any diffuseMap?
	if (lua_isnil(L, -1)) // no
	{
		ShowStackSize(pLuaInter);
		printf("No diffuse map.\n");
	}
	else //yes
	{
		ShowStackSize(pLuaInter);
		TempString = lua_tolstring(L, -1, &StringLen);
		ASSERT(TempString);
		ThrowIfFalse(StringLen > 0 && StringLen < StringMaxLength);
		strcpy_s(diffuseMapName, StringMaxLength, TempString);
		printf("diffuseMap: %s\n", diffuseMapName);
	}
	lua_pop(L, 1);						// pop the diffuseMap(or the nil)

	//normalMap
	char normalMapName[256];
	TempString = nullptr;
	StringLen = 0;
	lua_getfield(L, -1, "normalMap");
	// does the material contain any normalMap?
	if (lua_isnil(L, -1)) // no
	{
		printf("No normal map.\n");
	}
	else //yes
	{
		TempString = lua_tolstring(L, -1, &StringLen);
		ASSERT(TempString);
		ThrowIfFalse(StringLen > 0 && StringLen < StringMaxLength);
		strcpy_s(normalMapName, StringMaxLength, TempString);
		printf("normalMap: %s\n", normalMapName);
	}
	lua_pop(L, 1);					// pop the normalMap(or the nil)
	return true;
}
