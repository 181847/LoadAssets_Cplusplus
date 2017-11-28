#pragma once

#include "../../Library/Lua/LuaInterpreter/LuaInterpreter.h"
#include "../../Library/Lua/LuaModuls/LuaMeshData/LuaMeshDataStruct.h"
#include "../../Library/MyTools/Formater.h"
#include "../../Library/MyTools/LuaTools.h"
#include "../../Library/DirectX12/FrameResource.h"
#include "../../Library/DirectX12/Common/d3dUtil.h"

#pragma comment(lib, "LuaInterpreter.lib")

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
	pLuaInter->GetFieldOnTop("GeometryQueue");
	return false;
}

}// namespace LoadAssets
}// Lua

