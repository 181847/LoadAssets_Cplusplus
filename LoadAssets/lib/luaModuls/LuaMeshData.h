#pragma once

// my help tools
#include "..\MyTools\LuaTools.h"
#include "..\MyTools\MyTools.h"
#include "..\MyTools\Cleaner.h"

// necessary directX modules
#include "..\DirectX12\Common\GeometryGenerator.h"
#include "..\DirectX12\Common\d3dUtil.h"

// lua head files, in the C style
#ifdef __cplusplus
extern "C"
{
#endif
// includes
#include "..\lua\lua.h"
#include "..\lua\lauxlib.h"
#include "..\lua\lualib.h"
// includes
#ifdef __cplusplus
}
#endif

// compiled lua dll
#pragma comment(lib, "lua.lib")

// Here define the geometry container for store the mesh data in lua userData.
namespace Lua
{
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;
	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 TangentU;
		DirectX::XMFLOAT2 TexC;

		Vertex(
			DirectX::XMFLOAT3 pos,
			DirectX::XMFLOAT3 nml,
			DirectX::XMFLOAT3 tgu,
			DirectX::XMFLOAT2 tec)
			:Position(pos), Normal(nml), TangentU(tgu), TexC(tec) {}
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<uint32> Indices32;

		// for read object,
		// here we save the component seperately.
		std::vector<DirectX::XMFLOAT3> Positions;
		std::vector<DirectX::XMFLOAT3> Normals;
		std::vector<DirectX::XMFLOAT3> TangentUs;
		std::vector<DirectX::XMFLOAT2> Texcoords;

	private:
		std::vector<uint16> mIndices16;

	public:
		std::vector<uint16>& GetIndices16()
		{
			if (mIndices16.empty())
			{
				mIndices16.resize(Indices32.size());
				for (size_t i = 0; i < Indices32.size(); ++i)
				{
					mIndices16[i] = static_cast<uint16>(Indices32[i]);
				}
			}
		}
	};
}

// get pointer (in the userData) from the lua_State
#define checkMeshData(L) \
	reinterpret_cast<LuaPointerContainer<Lua::MeshData>*>\
	(luaL_checkudata(L, 1, "LoadAssets.MeshData"))->pointer

// get pointerContainer (in the userData) from the lua_State
#define checkConainer(L) \
	reinterpret_cast<LuaPointerContainer<Lua::MeshData>*>\
	(luaL_checkudata(L, 1, "LoadAssets.MeshData"))

#ifdef __cplusplus
extern "C"
{
#endif
	int DLL_API luaopen_MeshData(lua_State * L);
#ifdef __cplusplus
}
#endif

// create a new MeshData.
static int lua_newMeshData(lua_State * L);

// just show the data for test
static int lua_showMeshData(lua_State * L);

// just show the data for test
static int lua_getDetail(lua_State * L);

// add positioni to the buffer 
static int lua_addPosition(lua_State * L);

// add normal to the buffer
static int lua_addNormal(lua_State * L);

// add tangentU to the buffer
static int lua_addTangentU(lua_State * L);

// add textureCoord to the buffer
static int lua_addTexC(lua_State * L);

// add a vertex using the index of the previous added positions/normals.....
// here we must pass 4 integer as respect Compoment Index,
// 1 - position, 2 - normal, 3 - textureCoord, 4 - tangentU
static int lua_addVertex(lua_State * L);

// add a index to the indices, a index must be the 'index' of the vertices,
static int lua_addIndex(lua_State * L);

// show some help message.
static int lua_help(lua_State * L);

// called when the collect garbage.
static int lua_gc(lua_State * L);

// module lib, only a 'new' function.
static const struct luaL_Reg MeshDataLib[] =
{
	{"new", lua_newMeshData},
	{NULL, NULL}
};

// methods of the userData
static const struct luaL_Reg MeshDataLib_m[] =
{
	{ "show",				lua_showMeshData},
	{ "getDetail",			lua_getDetail },
	{ "addPosition",		lua_addPosition },
	{ "addNormal",			lua_addNormal },
	{ "addTangentU",		lua_addTangentU },
	{ "addTextureCoord",	lua_addTexC },
	{ "addVertex",			lua_addVertex},
	{ "addIndex",			lua_addIndex },
	{ "help",				lua_help },
	{ "__gc",				lua_gc },
	{NULL, NULL}
};
