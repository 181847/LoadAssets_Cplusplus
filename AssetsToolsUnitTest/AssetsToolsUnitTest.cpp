// AssetsToolsUnitTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../../Library/Lua/LuaInterpreter/LuaInterpreter.h"
#include "../LoadAssets/AssetsTools.h"
#include "../../Library/MyTools/UnitTestModules.h"

// this variable is externed in 'Library/DirectX12/d3dUt'
const int gNumFrameResources = 3;

DECLARE_TEST_UNITS;

static std::unique_ptr<Lua::LuaInterpreter> pLuaInter = std::make_unique<Lua::LuaInterpreter>();
#define TEST_TARGET pLuaInter


void TestUnit::GetReady()
{
	TEST_TARGET->DoFile("Init.lua");
}

void TestUnit::AfterTest()
{
	TEST_TARGET->Run();
}

void TestUnit::AddTestUnit()
{
	TEST_UNIT_START("start up, can you see me?")
		return true;
	TEST_UNIT_END;

	TEST_UNIT_START("")

	TEST_UNIT_END;
}


int main()
{
	TestUnit::testMain();
    return 0;
}

