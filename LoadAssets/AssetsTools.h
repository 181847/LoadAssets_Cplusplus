#pragma once
#include "LuaInterpreter.h"
#include "KeyGenerator.h"
#include "lib\DirectX12\FrameResource.h"

// This is a header which define many tool function to load assets,
// the function whose name start with Lua mean the function need a LuaInterpreter,


// Load Material Data from the luaInterpreter, 
// the additional Material will be append into the matMap,
// if there no confliction of name(or the old Material will be replace).
bool LuaLoadMaterial(LuaInterpreter* pLuaInter,
	std::unordered_map<std::string, Material> * matMap);



// Before called, ensure the stack top is a material,
// call the function to get one material into the matMap,
// then clear the stack to where the top material
// (the material also be popped).
bool LuaLoadSingleMaterial(LuaInterpreter* pLuaInter,
	std::unordered_map<std::string, Material> * matMap);
