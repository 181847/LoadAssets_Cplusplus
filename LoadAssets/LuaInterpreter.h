#pragma once

#include "lib/MyTools/MyTools.h"

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

#define Not(x) (!x)

#pragma comment (lib, "lua.lib")

class LuaInterpreter
{
public:
	LuaInterpreter();
	~LuaInterpreter();

	DELETE_COPY_CONSTRUCTOR(LuaInterpreter)

	// start running 
	void Run();

public:
	bool stop = false;

private:
	lua_State * m_L;
	// use to store the command line.
	char buffer[255];
	int error;
};

