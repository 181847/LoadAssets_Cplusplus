#include "AssetsTools.h"

bool LuaLoadMaterial(LuaInterpreter * pLuaInter, std::unordered_map<std::string, Material> * matMap)
{
	ASSERT(pLuaInter);
	ASSERT(matMap);

	auto * L = pLuaInter->m_L;

	// the MatPart is the MaterialSet which contain information 
	// about all the material.
	lua_getglobal(L, "MatPart");

	return false;
}
