

AddPath("D:\\GitHub\\Lua\\LoadAssets\\?.lua")
--AddCPath("D:\\GitHub\\Lua\\LoadAssets\\?.dll")
AddPath(".\\luaScript\\?.lua")
AssembleModule = require("Assemble")
DebugLogger("Assemble Module loaded")

Assemble = AssembleModule.Assemble

DebugLogger("AssembleFunction Module loaded")

-- from the file adding our file
startLoadAssets = loadfile("startLoadAssets.lua")
startLoadAssets()
DebugLogger("Assets load end")

DebugLogger("assemble all assets into the table assembleSet")

success, assembleSet = Assemble()

print(success)

assert(success, "initial assembling failed, cannot process further testing")
