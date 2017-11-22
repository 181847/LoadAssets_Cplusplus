#pragma once
#include "LuaInterpreter.h"
#include "Formater.h"
#include "lib\DirectX12\FrameResource.h"
#include <functional>
// This is a header which define many tool function to load assets,
// the function whose name start with Lua mean the function need a LuaInterpreter,


// ensure that the assembleSet is sitted at top of the stack.
// Load Material Data from the luaInterpreter, 
// the additional Material will be append into the matMap,
// if there no confliction of name(or the old Material will be replace).
bool LuaLoadMaterial(LuaInterpreter* pLuaInter,
	std::vector<Material> * matArr);



// Before called, ensure the stack top is a material,
// call the function to get one material into the matMap,
// then clear the stack to where the top material
// (the material also be popped).
bool LuaLoadSingleMaterial(LuaInterpreter* pLuaInter,
	std::vector<Material> * matArr);

void ShowDetail(Material & m);

// ensure that the assembleSet is on the top of the stack.
bool LuaLoadGeometrys(LuaInterpreter* pLuaInter,
	std::vector<MeshGeometry> geoArr, 
	std::function<std::unique_ptr<MeshGeometry>()>);
