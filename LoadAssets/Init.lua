--[[
This script is used to config the interpretoer,
such as adding the search patch.
--]]

-- AddPath to add a path to the search for loading file
local function AddPath(path)
    package.path = package.path..";"..path
end

AddPath("D:\\GitHub\\Lua\\LoadAssets\\?.lua")
AddPath(".\\luaScript\\?.lua")

-- store the original 'loadfile', replace it with loadfileInPath
old_loadfile = loadfile

-- modify the loadfile function,
-- enable the function the ability to search through the package.path, 
-- not just in current folder.
-- notice that if the fileName end with ".lua", 
-- ".lua" will be removed.
loadfile = function(filePath)
    -- remove the ".lua" in the end.
    fileName = string.match(filePath, "(%w-).lua$") or filePath
    
    -- use the searchpath to find the actual path for the file
    fileName = package.searchpath(fileName, package.path)
    
    if fileName then
        return old_loadfile(fileName)
    else
        error("no such file:\n\t"..filePath)
    end
end


-- this is the function to check if all the assets linked corrected.
Assemble = require("Assemble")

-- Call Assemble to load the necessary classes,
-- such as Texture/RenderItem.....
Assemble()

-- from the file adding our file
startLoadAssets = loadfile("startLoadAssets.lua")

startLoadAssets()
