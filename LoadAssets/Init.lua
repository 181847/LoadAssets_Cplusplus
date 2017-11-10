--[[
This script is used to config the interpretoer,
such as adding the search patch.
--]]
pathes = {}
pathes[#pathes + 1] = "D:\\GitHub\\Lua\\LoadAssets\\?.lua"


for i = 1, #pathes do
    package.path = package.path..";"..pathes[i]
end


-- store the original 'loadfile', replace it with loadfileInPath
old_loadfile = loadfile

-- modify the loadfile function,
-- enable the function can search through the package.path, 
-- not just in current folder.
-- notice that if the fileName end with ".lua", 
-- ".lua" will be removed.
loadfile = function(fileName)
    -- remove the ".lua" in the end.
    fileName = string.match(fileName, "(%w-).lua$") or fileName
    
    -- use the searchpath to find the actual path for the file
    fileName = package.searchpath(fileName, package.path)
    
    return old_loadfile(fileName)
end
