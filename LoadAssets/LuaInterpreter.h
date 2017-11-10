#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include "lib/lua/lua.h"
#include "lib/lua/lualib.h"
#include "lib/lua/lauxlib.h"
#ifdef __cplusplus
}
#endif

#pragma comment (lib, "lua.lib")

class LuaInterpreter
{
public:
	LuaInterpreter();
	~LuaInterpreter();

public:
	// a flag that will stop the interpreter.
	static bool stop;

private:
	lua_State * m_L;
	// use to store the command line.
	char buffer[255];
	int error;
};

