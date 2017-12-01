#include "AssetsTools.h"

#ifdef _DEBUG
#define	ShowStackSize(luaInterpreter) printf("StackSize: %d\n", luaInterpreter->GetStackSize())
#else
#define ShowStackSize(luaInterpreter)
#endif

namespace Lua
{
namespace LoadAssets
{


bool LuaLoadMaterial(LuaInterpreter * pLuaInter,
	std::vector<Material> * matArr)
{
	pLuaInter->Foreach(
		FOREACH_START
			Formater<MaxNameLength> nameBuffer;
			if (keyIsNumber)
			{
				Material mat;
				EACH	->GetFieldOnTop("name")
							->ToStringAndClear<nameBuffer.Size>(nameBuffer.bufferPointer())
						->GetFieldOnTop("diffuseAlbedo")
							->Foreach(
								FOREACH_START
									if (keyIsNumber)
									{
										auto diffuse = EACH->ToNumberAndPop<float>();
										// be care that the keyItg in lua start from 1
										// here we should substract one.
										MAP_INDEX_TO_XYZW(diffuse, keyItg - 1, mat.DiffuseAlbedo);
									}
								FOREACH_END
							)// end foreach 
							->Pop() // pop the diffuseAlbedo table
						->GetFieldOnTop("fresnelR")
							->Foreach(
								FOREACH_START
									if (keyIsNumber)
									{
										auto fresnelR = EACH->ToNumberAndPop<float>();
										// be care that the keyItg in lua start from 1
										// here we should substract one.
										MAP_INDEX_TO_XYZ(fresnelR, keyItg - 1, mat.FresnelR0);
									}
								FOREACH_END
							)// end foreach fresnelR
							->Pop() // pop the fresnelR table
						->GetFieldOnTop("roughness")
							->ToNumberAndPop(&mat.Roughness)
						->GetFieldOnTop("diffuseMapIndex")
							->If(
								LUA_INTERPRETER_IF
									return CONDITION_TARGET->IsNil();
								LUA_INTERPRETER_THEN
									// pop the nil
									THEN_TARGET->Pop();
									mat.DiffuseSrvHeapIndex = 0;
								LUA_INTERPRETER_ELSE
									ELSE_TARGET->ToIntegerAndPop(&mat.DiffuseSrvHeapIndex);
								LUA_INTERPRETER_ENDIF
							)
						->GetFieldOnTop("normalMapIndex")
							->If(
								LUA_INTERPRETER_IF
									return CONDITION_TARGET->IsNil();
								LUA_INTERPRETER_THEN
									// pop the nil
									THEN_TARGET->Pop();
									mat.NormalSrvHeapIndex = 0;
								LUA_INTERPRETER_ELSE
									ELSE_TARGET->ToIntegerAndPop(&mat.NormalSrvHeapIndex);
								LUA_INTERPRETER_ENDIF
							);
				EACH->Pop();// pop the material
			}// end if keyIsNumber
		FOREACH_END
	);// end foreach material
	return false;
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
	
}// ShowDetail


template<typename GEOMETRY>
bool 
LuaLoadGeometrys(
	LuaInterpreter * pLuaInter, 
	std::vector<std::unique_ptr<GEOMETRY>>* geoArr, 
	std::function<std::unique_ptr<GEOMETRY>(std::string&name, Lua::MeshData*)> converter)
{
	ASSERT(pLuaInter);
	ASSERT(geoArr);

	// this is the function to go into the pLuaInterpreter get the userData,
	// then convert it to the expected pointer,
	// here we converte the userdata(void*) to the Lua::MeshData*
	std::function<Lua::MeshData*(void *)> 
		userData_to_meshData = 
		[](void * pointer)
	{
		DEBUG_MESSAGE("converte a userdata to the meshdata\n");

		auto pMeshData = reinterpret_cast<LuaPointerContainer<Lua::MeshData> *>(pointer);
		return pMeshData->pointer;
	};

	// help get the name
	Formater<MaxNameLength> name;

	int geoCount = 0;
	// Geometry count
	pLuaInter->GetFieldOnTop("n");
	geoCount = pLuaInter->ToNumberAndPop<int>();
	
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
}// LuaLoadGeometrys


 //
 //bool 
 //LuaLoadGeometrys(
 //	LuaInterpreter* pLuaInter,
 //	std::vector<std::unique_ptr<MeshGeometry>> *geoArr,	// store all the geometry
 //	ID3D12Device * mDevice,
 //	ID3D12GraphicsCommandList * mCmdList)
 //{
 //	// first define a lambda function to convert Lua::MeshData
 //	// to the MeshGeometry
 //	std::function<
 //		std::unique_ptr<MeshGeometry>
 //		(std::string& name, Lua::MeshData *)>
 //	converter = 
 //
 //		// lambda
 //	[mDevice, mCmdList]
 //	(std::string& name, Lua::MeshData* pMd) 
 //		-> std::unique_ptr<MeshGeometry>
 //	{
 //		auto &Vertices	= pMd->Vertices;
 //		auto &Indices	= pMd->GetIndices16();
 //		const UINT VertexCount		= Vertices.size();
 //		const UINT IndexCount		= Indices.size();
 //
 //		// using the MeshData to fill the indices
 //		std::vector<uint16_t> totalIndices;
 //
 //		totalIndices.insert(totalIndices.end(),
 //			std::begin(pMd->GetIndices16()), std::end(pMd->GetIndices16()));
 //
 //		// by default, we will use the Lua::Vertex as the
 //		// vertex struct, which contain the:
 //		// position(3)/normal(3)/textureCoord(2)/tangentU(3)
 //		const UINT vbByteSize = VertexCount * sizeof(Lua::Vertex);
 //		const UINT ibByteSize = IndexCount * sizeof(std::uint16_t);
 //
 //		auto geo = std::make_unique<MeshGeometry>();
 //		geo->Name					= name;
 //		geo->VertexBufferByteSize	= vbByteSize;
 //		geo->IndexBufferByteSize	= ibByteSize;
 //		geo->VertexByteStride		= (UINT)sizeof(Vertex);
 //		geo->IndexFormat			= DXGI_FORMAT_R16_UINT;
 //
 //		ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
 //		ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
 //
 //		CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), Vertices.data(), vbByteSize);
 //		CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), Indices.data(), ibByteSize);
 //
 //		//geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(mDevice.Get(), mCmdList.Get(),
 //		//	Vertices.data(), vbByteSize, geo->VertexBufferUploader);
 //		//geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(mDevice.Get(), mCmdList.Get(),
 //		//	Indices.data(), ibByteSize, geo->IndexBufferUploader);
 //		DEBUG_MESSAGE("convert meshdata\n");
 //		return std::move(geo);
 //	};
 //
 //	return LuaLoadGeometrys(pLuaInter, geoArr, converter);
 //}// LuaLoadGeometrys

}// namespace LoadAssets
}// namespace Lua
