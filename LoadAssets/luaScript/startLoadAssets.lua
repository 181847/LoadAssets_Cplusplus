-- This file is used as the add real assets part, 
-- the script will be automatically loaded and 
-- called by '../Init.lua'

_ENV = {_G = _G}
-- set the metatable, find the missing var and function
-- in the _G enviroment.
_G.setmetatable(_ENV, {__index = _G})

test_t_1 = Texture.new("brick", "d:/texture/brick.dds")
test_t_2 = Texture.new("tree", "d:/texture/tree.dds")

test_m_1 = Material.new("baseMat")
test_m_2 = Material.new("whiteMat")

test_g_1 = Geometry.new("box", "../base/box.obj")
test_g_2 = Geometry.new("sphere", "../base/geoSphere.obj")

test_ritem_1 = RenderItem.new(test_g_1.name, test_m_1.name, "opaque")
test_ritem_2 = RenderItem.new(test_g_2.name, test_m_2.name, "water")

test_t_1:addToGlobalSet()
test_t_2:addToGlobalSet()
--
test_m_1:addToGlobalSet()
test_m_2:addToGlobalSet()
--
test_g_1:addToGlobalSet()
test_g_2:addToGlobalSet()
--
test_ritem_1:addToGlobalSet()
test_ritem_2:addToGlobalSet()
