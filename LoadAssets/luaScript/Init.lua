--[[
This script is used to config the interpretoer,
such as adding the search patch.
--]]
pathes = {}
pathes[#pathes + 1] = "D:\\GitHub\\Lua\\LoadAssets\\?.lua"


for i = 1, #pathes do
    package.path = package.path..path1
end
