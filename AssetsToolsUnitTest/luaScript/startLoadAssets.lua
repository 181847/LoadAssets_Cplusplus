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
test_m_3 = Material.new("bricks")
test_m_3.diffuseAlbedo[1] = 0.3
test_m_3.diffuseAlbedo[2] = 0
test_m_3.diffuseMap = test_t_1.name

test_m_4 = Material.new("tree")
test_m_4.diffuseAlbedo[3] = 0.222223333
test_m_4.normalMap = test_t_1.name
test_m_4.diffuseMap = test_t_2.name

test_g_1 = Geometry.new("box", "Tank.obj")
test_g_2 = Geometry.new("sphere", "shapeG.obj")

test_ritem_1 = RenderItem.new(test_g_1.name, 'Box006', test_m_1.name, "opaque")
test_ritem_2 = RenderItem.new(test_g_2.name, 'cube1 group1', test_m_2.name, "water")

test_t_1:addToGlobalSet()
test_t_2:addToGlobalSet()
--
test_m_1:addToGlobalSet()
test_m_2:addToGlobalSet()
test_m_3:addToGlobalSet()
test_m_4:addToGlobalSet()
--
test_g_1:addToGlobalSet()
test_g_2:addToGlobalSet()
--
test_ritem_1:addToGlobalSet()
test_ritem_2:addToGlobalSet()
