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
		break;						\
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



// Before called, ensure the stack top is a material,
// call the function to get one material into the matMap,
// then clear the stack to where the top material
// (the material also be popped).
bool LuaLoadSingleMaterial(LuaInterpreter* pLuaInter,
	std::vector<Material> * matArr);

// ensure that the assembleSet is on the top of the stack.
//bool
//LuaLoadGeometrys(LuaInterpreter* pLuaInter,
//	std::vector<std::unique_ptr<MeshGeometry>> *geoArr,	// store all the geometry
//	ID3D12Device * mDevice,
//	ID3D12GraphicsCommandList * mCmdList);

// ensure that the assembleSet is on the top of the stack,
// here will use a function to convert the MeshData to MeshGeoemtry
// the user can modify the function to do different
template<typename GEOMETRY>
bool
LuaLoadGeometrys(LuaInterpreter* pLuaInter,
	std::vector<std::unique_ptr<GEOMETRY>> *geoArr,	// store all the geometry
													// a function to convert the Lua::MeshData to MeshGeometry
	std::function<std::unique_ptr<GEOMETRY>(std::string& name, Lua::MeshData *)> converter,
	std::function<void(std::string, UINT startIndex, UINT endIndex)> subMeshCollector);

// A debug function to show the Material data.
void ShowDetail(Material & m);


template<typename GEOMETRY>
bool 
LuaLoadGeometrys(
	LuaInterpreter * pLuaInter, 
	std::vector<std::unique_ptr<GEOMETRY>>* geoArr, 
	std::function<std::unique_ptr<GEOMETRY>(std::string&name, Lua::MeshData*)> converter, 
	std::function<void(std::string, UINT startIndex, UINT endIndex)> subMeshCollector)
{
	// @ pLuaInter
	pLuaInter	
		->GetFieldOnTop("GeometryQueue")
			// ---> foreach each geometry
			->Foreach([&geoArr](LUA_INTERPRETER_FOEEACH_LAMBDA_ARGS) {
				if (keyIsNumber)
				{
					DEBUG_MESSAGE("iterate a geometry index:%d\n", keyItg);
					MeshData* pmd;

					// @ pLuaInter
					pLuaInter
						->GetFieldOnTop("meshData")
							->ToUserDataAndClear<Lua::MeshData>("LoadAssets.MeshData", pmd)
						->GetFieldOnTop("SubMeshes")
							// ---> for each subMesh there are two integer that we should get.
							->Foreach([](LUA_INTERPRETER_FOEEACH_LAMBDA_ARGS) {
								if (Not(keyIsNumber))
								{
									int startIndex = 0, endIndex = 0;
									// @ pLuaInter
									pLuaInter
										->GetFieldOnTop("startIndex")
											->ToIntegerAndPop(&startIndex)
										->GetFieldOnTop("endIndex")
											->ToIntegerAndPop(&endIndex);
									subMeshCollector(std:;string(keyStr), startIndex, endIndex);
								}// <==> end if keyIsNumber
							});
							// ---> end Foreach
					geoArr->push_back(converter(pmd));
					DEBUG_MESSAGE("geometry end");
				}// <==> end if keyIsNumber
			})
			// ---> end Foreach
		->Pop();// pop GeometryQueue
	return true;
}

}// namespace LoadAssets
}// Lua

