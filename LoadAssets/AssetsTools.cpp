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

	Formater<MaxNameLength> formater;

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
	Formater<32> formater;				// to generatoer the key string

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

bool 
LuaLoadGeometrys(
	LuaInterpreter * pLuaInter, 
	std::vector<std::unique_ptr<MeshGeometry>>* geoArr, 
	Microsoft::WRL::ComPtr<ID3D12Device> mDevice, 
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList)
{
	// first define a lambda function to convert Lua::MeshData
	// to the MeshGeometry
	std::function<
		std::unique_ptr<MeshGeometry>
		(std::string& name, Lua::MeshData *)>
	converter = 

		// lambda
	[mDevice, mCmdList]
	(std::string& name, Lua::MeshData* pMd) 
		-> std::unique_ptr<MeshGeometry>
	{
		auto &Vertices	= pMd->Vertices;
		auto &Indices	= pMd->GetIndices16();
		const UINT VertexCount		= Vertices.size();
		const UINT IndexCount		= Indices.size();

		// using the MeshData to fill the indices
		std::vector<uint16_t> totalIndices;

		totalIndices.insert(totalIndices.end(),
			std::begin(pMd->GetIndices16()), std::end(pMd->GetIndices16()));

		// by default, we will use the Lua::Vertex as the
		// vertex struct, which contain the:
		// position(3)/normal(3)/textureCoord(2)/tangentU(3)
		const UINT vbByteSize = VertexCount * sizeof(Lua::Vertex);
		const UINT ibByteSize = IndexCount * sizeof(std::uint16_t);

		auto geo = std::make_unique<MeshGeometry>();
		geo->Name					= name;
		geo->VertexBufferByteSize	= vbByteSize;
		geo->IndexBufferByteSize	= ibByteSize;
		geo->VertexByteStride		= (UINT)sizeof(Vertex);
		geo->IndexFormat			= DXGI_FORMAT_R16_UINT;

		ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
		ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));

		CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), Vertices.data(), vbByteSize);
		CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), Indices.data(), ibByteSize);

		//geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(mDevice.Get(), mCmdList.Get(),
		//	Vertices.data(), vbByteSize, geo->VertexBufferUploader);
		//geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(mDevice.Get(), mCmdList.Get(),
		//	Indices.data(), ibByteSize, geo->IndexBufferUploader);

		return std::move(geo);
	};

	return LuaLoadGeometrys(pLuaInter, geoArr, converter);
}

template<typename GEOMETRY>
bool 
LuaLoadGeometrys(
	LuaInterpreter * pLuaInter, 
	std::vector<std::unique_ptr<GEOMETRY>>* geoArr, 
	std::function<std::unique_ptr<GEOMETRY>(std::string&name, Lua::MeshData*)> converter)
{
	ASSERT(pLuaInter);
	ASSERT(geoArr);

	// this is a funciton to get the subMesh name from the LuaInterpreter.
	// Before call the function, ensure that the subMeshes is on the top of the stack,
	// stack top -> subMeshes
	//						box:
	//							startIndex	= 1
	//							endIndex	= 32
	//						sphere:
	//							startIndex	= 33
	//							endIndex	= 75
	std::function<std::unique_ptr<GEOMETRY>(std::unique_ptr<GEOMETRY> geo> 
		DecorateWithSubMeshName =
	[&pLuaInter]
	(std::unique_ptr<GEOMETRY> geo) 
	{

		return std::move(geo);
	};

	// this is the function to go into the pLuaInterpreter get the userData,
	// then convert it to the expected pointer,
	// here we converte the userdata(void*) to the Lua::MeshData*
	std::function<Lua::MeshData*(void *)> 
		userData_to_meshData = 
		[](void * pointer)
	{
		auto pMeshData = reinterpret_cast<LuaPointerContainer<Lua::MeshData> *>(pointer);
		return pMeshData->pointer;
	};

	// help get the name
	Formater<MaxNameLength> name;

	int geoCount = 0;
	// open GeometryQueue
	pLuaInter->GetFieldOnTop("GeometryQueue");
	// Geometry count
	pLuaInter->GetFieldOnTop("n");
	geoCount = pLuaInter->ToNumberAndClear();
	
	for (int i = 1; i <= geoCount; ++i)
	{
		// one Geometry instance
		pLuaInter->GetIndexOnTop(i);

		// get geometry name 
		pLuaInter->GetFieldOnTop("name");
		pLuaInter->ToStringAndClear<MaxNameLength>(name.bufferPointer());
		std::string geoName = name.bufferPointer();

		// get meshData
		pLuaInter->GetFieldOnTop("meshData");
		auto * pMeshData = pLuaInter->ToUserDataAndClear("Lua.MeshData", userData_to_meshData);

		auto geo = converter(geoName, pMeshData);
		
		geoArr->push_back(std::move(geo));
		
		// pop Geometry instance
		pLuaInter->Pop();
	}


	// pop GeometryQueue
	pLuaInter->Pop();
	return false;
}
