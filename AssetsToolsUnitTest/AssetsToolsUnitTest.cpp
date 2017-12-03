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

static const char * 
	globalAssembleSetName	= "assembleSet";
static const char * 
	materialQueueName		= "MaterialQueue";
static const char * 
	geometryQueueName		= "GeometryQueue";


#define CHECK_XMFLOAT_4(xmfloat4, ex, ey, ez, ew)	\
		  (FT_EQ(xmfloat4.x, ex)					\
		&& FT_EQ(xmfloat4.y, ey)					\
		&& FT_EQ(xmfloat4.z, ez)					\
		&& FT_EQ(xmfloat4.w, ew))

#define CHECK_XMFLOAT_3(xmfloat3, ex, ey, ez)		\
		  (FT_EQ(xmfloat3.x, ex)					\
		&& FT_EQ(xmfloat3.y, ey)					\
		&& FT_EQ(xmfloat3.z, ez))

#define CHECK_XMFLOAT_2(xmfloat2, ex, ey)			\
		  (FT_EQ(xmfloat2.x, ex)					\
		&& FT_EQ(xmfloat2.y, ey))

// next we will declare some simple struct to 
// store the information to check certian test.
namespace temp
{

struct SubMesh
{
	std::string name;
	UINT startIndex;
	UINT endIndex;
	SubMesh(std::string n, UINT s, UINT e) 
		:name(n), startIndex(s), endIndex(e){}

	bool check(const std::string & n, UINT s, UINT e)
	{
		return name == n && startIndex == s && endIndex == e;
	}

	bool check(const char * n, UINT s, UINT e)
	{
		return check(std::string(n), s, e);
	}
};

struct Geometry
{
	std::string name;
	std::vector<SubMesh> subMeshs;
	int checkCount = 0;

	void checkSubMesh(const std::string& n, UINT s, UINT e)
	{
		int matchCount = 0;
		for (auto & sm : subMeshs)
		{
			matchCount += sm.check(n, s, e);
		}
		if (matchCount == 1)
			++checkCount;
	}
};
}


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

	TEST_UNIT_START("check load materials")
		TEST_TARGET->GetGlobal(globalAssembleSetName)
						->GetFieldOnTop(materialQueueName);
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

	TEST_UNIT_START("check the result of the loading geometries")
		TEST_TARGET
		->GetGlobal(globalAssembleSetName)
			->GetFieldOnTop(geometryQueueName);

		temp::Geometry boxGeo;
		boxGeo.name = "box";
		boxGeo.subMeshs.push_back(temp::SubMesh("Box006", 37, 72));
		boxGeo.subMeshs.push_back(temp::SubMesh("Box005", 1, 36));
		boxGeo.subMeshs.push_back(temp::SubMesh("Box004", 73, 108));
		temp::Geometry sphereGeo;
		sphereGeo.name = "sphere";
		sphereGeo.subMeshs.push_back(temp::SubMesh("group1 cube2", 25, 48));
		sphereGeo.subMeshs.push_back(temp::SubMesh("cube1 group1", 1, 24));

		std::unordered_map<std::string, temp::Geometry*> geos;

		geos[boxGeo.name]		= &boxGeo;
		geos[sphereGeo.name]	= &sphereGeo;
		

		UINT error = 0;
		auto converter =
			[&](const std::string & name, Lua::MeshData* pMesh)-> temp::Geometry* {
				return geos[name];
			};

		auto subMeshCollector = 
			[](temp::Geometry* pgeo, const std::string& name, UINT startIndex, UINT endIndex) {
				pgeo->checkSubMesh(name, startIndex, endIndex);
			};


		Lua::LoadAssets::LuaLoadGeometries<temp::Geometry>(TEST_TARGET.get(), converter, subMeshCollector);

		return geos["box"]->checkCount == 3 && geos["sphere"]->checkCount == 2;
	TEST_UNIT_END;

}


int main()
{
	TestUnit::testMain();
    return 0;
}

