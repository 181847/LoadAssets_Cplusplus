#pragma once
#include "LuaInterpreter.h"
#include "lib\MyTools\Formater.h"
#include "lib\luaModuls\LuaMeshData.h"
#include "lib\MyTools\LuaTools.h"
#include "lib\DirectX12\FrameResource.h"
#include <functional>

static const unsigned int MaxNameLength = 256;

// This is a header which define many tool function to load assets,
// the function whose name start with Lua mean the function need a LuaInterpreter,


// ensure that the assembleSet is sitted at top of the stack.
// Load Material Data from the luaInterpreter, 
// the additional Material will be append into the matMap.
bool LuaLoadMaterial(LuaInterpreter* pLuaInter,
	std::vector<Material> * matArr);



// Before called, ensure the stack top is a material,
// call the function to get one material into the matMap,
// then clear the stack to where the top material
// (the material also be popped).
bool LuaLoadSingleMaterial(LuaInterpreter* pLuaInter,
	std::vector<Material> * matArr);

// A debug function to show the Material data.
void ShowDetail(Material & m);

// ensure that the assembleSet is on the top of the stack.
template<typename GEOMETRY>
bool 
LuaLoadGeometrys(LuaInterpreter* pLuaInter,
	std::vector<std::unique_ptr<GEOMETRY>> *geoArr,	// store all the geometry
	Microsoft::WRL::ComPtr<ID3D12Device> mDevice,
	Microsoft::WRL::ComPtr<ID3D12CommandList> mCmdList);

// ensure that the assembleSet is on the top of the stack,
// here will use a function to convert the MeshData to MeshGeoemtry
// the user can modify the function to do different
template<typename GEOMETRY>
bool
LuaLoadGeometrys(LuaInterpreter* pLuaInter,
	std::vector<std::unique_ptr<GEOMETRY>> *geoArr,	// store all the geometry
	// a function to convert the Lua::MeshData to MeshGeometry
	std::function<std::unique_ptr<GEOMETRY>(std::string& name, Lua::MeshData *)> converter);

