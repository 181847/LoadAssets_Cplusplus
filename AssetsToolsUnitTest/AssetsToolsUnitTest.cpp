// AssetsToolsUnitTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../../Library/Lua/LuaInterpreter/LuaInterpreter.h"
#include "../LoadAssets/AssetsTools.h"
#include "../../Library/MyTools/UnitTestModules.h"
#include "../../Library/MyTools/Formater.h"

// this variable is externed in 'Library/DirectX12/d3dUt'
const int gNumFrameResources = 3;

DECLARE_TEST_UNITS;

static std::unique_ptr<Lua::LuaInterpreter> pLuaInter = std::make_unique<Lua::LuaInterpreter>();
#define TEST_TARGET pLuaInter

static const char * globalAssembleSetName = "assembleSet";


#define CHECK_XMFLOAT_4(xmfloat4, ex, ey, ez, ew)\
		  (FT_EQ(xmfloat4.x, ex)	\
		&& FT_EQ(xmfloat4.y, ey)	\
		&& FT_EQ(xmfloat4.z, ez)	\
		&& FT_EQ(xmfloat4.w, ew))

#define CHECK_XMFLOAT_3(xmfloat3, ex, ey, ez)\
		  (FT_EQ(xmfloat3.x, ex)	\
		&& FT_EQ(xmfloat3.y, ey)	\
		&& FT_EQ(xmfloat3.z, ez))

#define CHECK_XMFLOAT_2(xmfloat2, ex, ey)\\
		  (FT_EQ(xmfloat2.x, ex)	\
		&& FT_EQ(xmfloat2.y, ey))


void TestUnit::GetReady()
{
	TEST_TARGET->DoFile("Init.lua");
}

void TestUnit::AfterTest()
{
	TEST_TARGET->Run();
}

void TestUnit::AddTestUnit()
{
	TEST_UNIT_START("start up, can you see me?")
		return true;
	TEST_UNIT_END;

	TEST_UNIT_START("get materials")
		TEST_TARGET->GetGlobal(globalAssembleSetName)
						->GetFieldOnTop("MaterialQueue");
		std::vector<Material> gmaterials;

		Lua::LoadAssets::LuaLoadMaterial(TEST_TARGET.get(), &gmaterials);

		bool equal = true;
		for (Material & m : gmaterials)
		{
			// check each material's name
			if (m.Name == "baseMat")
			{
				equal = equal && CHECK_XMFLOAT_4(m.DiffuseAlbedo, 1.1f, 1.2f, 1.3f, 1.4f);
				equal = equal && CHECK_XMFLOAT_3(m.FresnelR0, 1.1f, 1.2f, 1.3f);
				equal = equal && FT_EQ(m.Roughness, 1.11f);
				equal = equal && EQ(m.DiffuseSrvHeapIndex, 0);
				equal = equal && NOT_EQ(m.NormalSrvHeapIndex, 0);
			}
			else if (m.Name == "whiteMat")
			{
				equal = equal && CHECK_XMFLOAT_4(m.DiffuseAlbedo, 2.1f, 2.2f, 2.3f, 2.4f);
				equal = equal && CHECK_XMFLOAT_3(m.FresnelR0, 2.1f, 2.2f, 2.3f);
				equal = equal && FT_EQ(m.Roughness, 2.11f);
				equal = equal && NOT_EQ(m.DiffuseSrvHeapIndex, 0);
				equal = equal && EQ(m.NormalSrvHeapIndex, 0);
			}
			else if (m.Name == "bricks")
			{
				equal = equal && CHECK_XMFLOAT_4(m.DiffuseAlbedo, 3.1f, 3.2f, 3.3f, 3.4f);
				equal = equal && CHECK_XMFLOAT_3(m.FresnelR0, 3.1f, 3.2f, 3.3f);
				equal = equal && FT_EQ(m.Roughness, 3.11f);
				equal = equal && EQ(m.DiffuseSrvHeapIndex, 0);
				equal = equal && EQ(m.NormalSrvHeapIndex, 0);
			}
			else if (m.Name == "tree")
			{
				equal = equal && CHECK_XMFLOAT_4(m.DiffuseAlbedo, 4.1f, 4.2f, 4.3f, 4.4f);
				equal = equal && CHECK_XMFLOAT_3(m.FresnelR0, 4.1f, 4.2f, 4.3f);
				equal = equal && FT_EQ(m.Roughness, 4.11f);
				equal = equal && NOT_EQ(m.DiffuseSrvHeapIndex, 0);
				equal = equal && NOT_EQ(m.NormalSrvHeapIndex, 0);
			}
			else
			{
				return false;
			}
		}
		return equal;
	TEST_UNIT_END;
}


int main()
{
	TestUnit::testMain();
    return 0;
}

