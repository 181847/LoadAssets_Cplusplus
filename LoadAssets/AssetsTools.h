#pragma once
#include "LuaInterpreter.h"
#include "lib\DirectX12\FrameResource.h"

// This is a header which define many tool function to load assets,
// the function whose name start with Lua mean the function need a LuaInterpreter,


// Load Material Data from the luaInterpreter, 
// the additional Material will be append into the matMap,
// if there no confliction of name(or the old Material will be replace).
bool LuaLoadMaterial(LuaInterpreter* pLuaInter,
	std::unordered_map<std::string, Material> matMap);
