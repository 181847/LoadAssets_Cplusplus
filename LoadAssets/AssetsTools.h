#pragma once

#include "../../Library/Lua/LuaInterpreter/LuaInterpreter.h"
#include "../../Library/Lua/LuaModuls/LuaMeshData/LuaMeshDataStruct.h"
#include "../../Library/MyTools/Formater.h"
#include "../../Library/MyTools/LuaTools.h"
#include "../../Library/DirectX12/FrameResource.h"
#include "../../Library/DirectX12/Common/d3dUtil.h"

#pragma comment(lib, "LuaInterpreter.lib")

// some struct use x/y/z/w to store data,
// and no index of array, this marco is to map the data of some index
// to specific x/y/z/w,
// receiver.{x, y, z, w}[index] = data,
// the index start from zero.
#define MAP_INDEX_TO_XYZW(data, index, receiver)\
	switch(index)								\
	{											\
	case 0:										\
		receiver.x = data;						\
		break;									\
	case 1:										\
		receiver.y = data;						\
		break;									\
	case 2:										\
		receiver.z = data;						\
		break;									\
	case 3:										\
		receiver.w = data;						\
		break;									\
	default:									\
		ASSERT(false && "map data to x//y//z//w failed ");	\
	}											\
// marco MAP_INDEX_TO_XYZW end

// some struct use x/y/z to store data,
// and no index of array, this marco is to map the data of some index
// to specific x/y/z,
// receiver.{x, y, z, w}[index] = data,
// the index start from zero.
#define MAP_INDEX_TO_XYZ(data, index, receiver)\
	switch(index)								\
	{											\
	case 0:										\
		receiver.x = data;						\
		break;									\
	case 1:										\
		receiver.y = data;						\
		break;									\
	case 2:										\
		receiver.z = data;						\
		break;									\
	default:									\
		ASSERT(false && "map data to x//y//z failed ");	\
	}											\
// marco MAP_INDEX_TO_XYZ end

namespace Lua
{
namespace LoadAssets
{

static const unsigned int MaxNameLength = 256;
// This is a header which define many tool function to load assets,
// the function whose name start with Lua mean the function need a LuaInterpreter,


// ensure the MaterialQueue is on the top of the stack
// the function will access to it,
// but the function will not pop the MaterialQueue out.
// the caller should take response for the queue.
bool LuaLoadMaterial(LuaInterpreter* pLuaInter,
	std::vector<Material> * matArr);

// A debug function to show the Material data.
void ShowDetail(Material & m);

// ensure the geometryQueue is on the top of the stack,
// 'converter' is the function to get the raw MeshData,
// the user can pass any lambda function to convert the MeshData
// to the desired data struct.
// the subMeshCollector is the function to get submesh informations.
// On each meshdata finded, the meshdata will be pass into the converter,
// and return some kind pointer the user defined,
// then when he subMesh finded below the geomety, 
// the the subMeshCollector will be called, and pass the 
// GEOMETRY* pointer , subMesh's name, two index to the subMeshCollector.
template<typename GEOMETRY>
bool 
LuaLoadGeometries(
	LuaInterpreter * pLuaInter,
	std::function<GEOMETRY*(const std::string &, Lua::MeshData*)> 
		converter, 
	std::function<void(GEOMETRY*, const std::string &, UINT, UINT)> 
		subMeshCollector);

template<typename GEOMETRY>
bool 
LuaLoadGeometries(
	LuaInterpreter * pLuaInter,
	std::function<GEOMETRY*(const std::string &, Lua::MeshData*)>
	converter,
	std::function<void(GEOMETRY*, const std::string &, UINT, UINT)>
	subMeshCollector)
{
	pLuaInter->Foreach(
		FOREACH_START
			// GeometryQueue have one 'n' -> 'integer' ,
			// we should pass it.
			if (Not(keyIsNumber))
				return;
			// store ther name of geometry
			Formater<256> nameBuffer;
			// store the meshDataPointer
			Lua::MeshData * pMeshData;
			// have submeshes?
			bool subMeshesIsNil = true;

			EACH
				->GetFieldOnTop("name")
					->ToStringAndClear<nameBuffer.Size>(nameBuffer.bufferPointer())
				->GetFieldOnTop("meshData")
				->ToUserDataAndClear<Lua::MeshData>(
					LuaMeshDataMetatableName,
					&pMeshData,
					[](void* pointer){
						return reinterpret_cast<LuaPointerContainer<Lua::MeshData>*>(pointer)->pointer; })
				->GetFieldOnTop("subMeshes")
					->IsNil(&subMeshesIsNil); // later we will pop it

			// convert the geometry
			GEOMETRY * pUserGeo = converter(nameBuffer.bufferPointer(), pMeshData);

			if (Not(subMeshesIsNil))
			{
				EACH // Geometry
				->Foreach( // subMeshes table
					FOREACH_START
						if (Not(keyIsNumber))
						{
							UINT startIndex(0), endIndex(0);
							EACH // submesh
							->GetFieldOnTop("startIndex")
								->ToIntegerAndPop(&startIndex)
							->GetFieldOnTop("endIndex")
								->ToIntegerAndPop(&endIndex);
							
							subMeshCollector(pUserGeo, keyStr, startIndex, endIndex);
						}
					FOREACH_END
				);// end foreach subMesh
			} // end if (haveSubMeshes)
			else
			{
				EACH // Geometry
					->Pop();		// pop the nil which is expected to the subMeshes table.
			} // end else (haveSubMeshes)
		FOREACH_END					// end foreach Geometry
	);
	return false;
}// end LoadGeometries

}// namespace LoadAssets
}// Lua

