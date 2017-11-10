#include "LuaInterpreter.h"


bool LuaInterpreter::stop = false;

static int ExitInterpreter(lua_State * L)
{

}

LuaInterpreter::LuaInterpreter()
{
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
	stop = false;
}


LuaInterpreter::~LuaInterpreter()
{
}
