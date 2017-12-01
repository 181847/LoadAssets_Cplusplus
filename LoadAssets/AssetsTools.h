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
bool 
LuaLoadGeometrys(
	LuaInterpreter * pLuaInter,
	std::function<void(std::string&name, Lua::MeshData*)> 
		converter, 
	std::function<void(std::string&name, UINT startIndex, UINT endIndex)> 
		subMeshCollector);

}// namespace LoadAssets
}// Lua

