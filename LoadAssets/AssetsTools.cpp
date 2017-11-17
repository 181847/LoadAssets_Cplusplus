#include "AssetsTools.h"

#ifdef _DEBUG
#define	ShowStackSize(luaInterpreter) printf("StackSize: %d\n", luaInterpreter->GetStackSize())
#else
#define ShowStackSize(luaInterpreter)
#endif

bool LuaLoadMaterial(LuaInterpreter * pLuaInter,
	std::vector<Material> * matArr)
{
	ASSERT(pLuaInter);
	ASSERT(matArr);

	KeyGenerator<32> formater;

	pLuaInter->GetFieldOnTop("MaterialQueue");
	pLuaInter->GetFieldOnTop("n");

	int matCount = pLuaInter->ToIntegerAndClear();
	printf("material count: %d\n", matCount);
	// n has been popped

	// notice that the index start from 1.
	for (int i = 1; i <= matCount; ++i)
	{
		
		pLuaInter->GetIndexOnTop(i);

		// ensure the top is a material,
		// call the function to get one material.
		LuaLoadSingleMaterial(pLuaInter, matArr);

		pLuaInter->Pop();
	}

	for (auto & m : *matArr)
	{
		ShowDetail(m);
		putchar('\n');
	}

	return true;
}

bool LuaLoadSingleMaterial(LuaInterpreter * pLuaInter,
	std::vector<Material> * matArr)
{
	Material material;
	ASSERT(pLuaInter);
	ASSERT(matArr);
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
		
		switch (i)
		{
		case 1:
			material.DiffuseAlbedo.x = dAlbe;
			break;
		case 2:
			material.DiffuseAlbedo.y = dAlbe;
			break;
		case 3:
			material.DiffuseAlbedo.z = dAlbe;
			break;
		case 4:
			material.DiffuseAlbedo.w = dAlbe;
			break;
		default:
			ThrowIfFalse(false && "It is impossible.");
		}
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

		switch (i)
		{
		case 1:
			material.FresnelR0.x = dAlbe;
			break;
		case 2:
			material.FresnelR0.y = dAlbe;
			break;
		case 3:
			material.FresnelR0.z = dAlbe;
			break;
		default:
			ThrowIfFalse(false && "It is impossible.");
		}
	}
	lua_pop(L, 1);							// pop fresnelR

	// roughness is just one number
	double roughness = 0;
	lua_getfield(L, -1, "roughness");
	roughness = lua_tonumberx(L, -1, &checker);
	printf("roughness: %lf\n", roughness);
	lua_pop(L, 1);							// pop 
	material.Roughness = roughness;

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
		material.DiffuseSrvHeapIndex = 0;
	}
	else //yes
	{
		ShowStackSize(pLuaInter);
		TempString = lua_tolstring(L, -1, &StringLen);
		ASSERT(TempString);
		ThrowIfFalse(StringLen > 0 && StringLen < StringMaxLength);
		strcpy_s(diffuseMapName, StringMaxLength, TempString);
		printf("diffuseMap: %s\n", diffuseMapName);

		// get the map index
		lua_pop(L, 1);
		int index = 0;
		lua_getfield(L, -1, "diffuseMapIndex");
		index = lua_tointegerx(L, -1, &checker);
		ASSERT(checker);
		printf("DiffuseMapIndex: %d\n", index);
		material.DiffuseSrvHeapIndex = index;
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
		material.NormalSrvHeapIndex = 0;
	}
	else //yes
	{
		TempString = lua_tolstring(L, -1, &StringLen);
		ASSERT(TempString);
		ThrowIfFalse(StringLen > 0 && StringLen < StringMaxLength);
		strcpy_s(normalMapName, StringMaxLength, TempString);
		printf("normalMap: %s\n", normalMapName);


		// get the map index
		lua_pop(L, 1);
		int index = 0;
		lua_getfield(L, -1, "normalMapIndex");
		index = lua_tointegerx(L, -1, &checker);
		ASSERT(checker);
		printf("normalMapIndex: %d\n", index);

		material.NormalSrvHeapIndex = index;
	}
	lua_pop(L, 1);					// pop the normalMap(or the nil)

	// insert into the map
	(*matMap)[std::string(matName)] = material;
	return true;
}

void ShowDetail(Material & m)
{
	printf("diffAbledo: %lf, %lf, %lf, %lf\n",
		m.DiffuseAlbedo.x,
		m.DiffuseAlbedo.y,
		m.DiffuseAlbedo.z,
		m.DiffuseAlbedo.w);
	printf("Fresnelr: %lf, %lf, %lf",
		m.FresnelR0.x,
		m.FresnelR0.y,
		m.FresnelR0.z);
	printf("diffuseMapIndex: %d", m.DiffuseSrvHeapIndex);
	printf("normalMapIndex: %d", m.NormalSrvHeapIndex);
	
}
