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

	// get material name
	char matName[StringMaxLength];
	pLuaInter->GetFieldOnTop("name");
	pLuaInter->ToStringAndClear<StringMaxLength>(matName);
	printf("Material Name; %s\n", matName);

	// get diffuseAlbedo, contain 4 number;
	double dAlbe = 0;
	pLuaInter->GetFieldOnTop("diffuseAlbedo");
	// 4 diffuseAlbedo
	for (int i = 1; i <= 4; ++i)
	{
		pLuaInter->GetIndexOnTop(i);
		dAlbe = pLuaInter->ToNumberAndClear();
		printf("diffuseAlbedo[%d]: %lf\n", i, dAlbe);
		
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
	pLuaInter->Pop();
	// diffuseAlbedo has been popped

	// 3 FresnelR
	double fresnelR = 0;
	pLuaInter->GetFieldOnTop("fresnelR");
	for (int i = 1; i <= 3; ++i)
	{
		pLuaInter->GetIndexOnTop(i);
		fresnelR = pLuaInter->ToNumberAndClear();
		printf("fresnelR[%d]: %lf\n", i, fresnelR);

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
	pLuaInter->Pop();
	// fresnelR has been popped

	// roughness is just one number
	pLuaInter->GetFieldOnTop("roughness");
	double roughness = pLuaInter->ToNumberAndClear();
	printf("roughness: %lf\n", roughness);
	material.Roughness = roughness;

	//diffuseMap
	char diffuseMapName[StringMaxLength];
	pLuaInter->GetFieldOnTop("diffuseMap");
	// does the material contain any diffuseMap?
	if (pLuaInter->IsNil()) // no
	{
		printf("No diffuse map.\n");
		material.DiffuseSrvHeapIndex = 0;
		pLuaInter->Pop();
	}
	else //yes
	{
		pLuaInter->ToStringAndClear<StringMaxLength>(diffuseMapName);
		printf("diffuseMapName: %s\n", diffuseMapName);

		pLuaInter->GetFieldOnTop("diffuseMapIndex");
		int diffuseMapIndex = pLuaInter->ToIntegerAndClear();
		printf("DiffuseMapIndex: %d\n", diffuseMapIndex);
		material.DiffuseSrvHeapIndex = diffuseMapIndex;
	}
	// do not need to pop any thing, material is on the top

	//diffuseMap
	char normalMapName[StringMaxLength];
	pLuaInter->GetFieldOnTop("normalMap");
	// does the material contain any normalMap?
	if (pLuaInter->IsNil()) // no
	{
		printf("No normal map.\n");
		material.NormalSrvHeapIndex = 0;
		pLuaInter->Pop();
	}
	else //yes
	{
		pLuaInter->ToStringAndClear<StringMaxLength>(normalMapName);
		printf("normalMapName: %s\n", normalMapName);

		pLuaInter->GetFieldOnTop("normalMapIndex");
		int normalMapIndex = pLuaInter->ToIntegerAndClear();
		printf("NormalMapIndex: %d\n", normalMapIndex);
		material.NormalSrvHeapIndex = normalMapIndex;
	}
	// do not need to pop any thing, material is on the top


	// finally , pop the material
	//pLuaInter->Pop();
	// insert into the map
	matArr->push_back(material);
	return true;
}

void ShowDetail(Material & m)
{
	printf("diffAbledo: %lf, %lf, %lf, %lf\n",
		m.DiffuseAlbedo.x,
		m.DiffuseAlbedo.y,
		m.DiffuseAlbedo.z,
		m.DiffuseAlbedo.w);
	printf("Fresnelr: %lf, %lf, %lf\n",
		m.FresnelR0.x,
		m.FresnelR0.y,
		m.FresnelR0.z);
	printf("diffuseMapIndex: %d\n", m.DiffuseSrvHeapIndex);
	printf("normalMapIndex: %d\n", m.NormalSrvHeapIndex);
	
}
