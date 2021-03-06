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
test_m_1.diffuseAlbedo[1] = 1.1
test_m_1.diffuseAlbedo[2] = 1.2
test_m_1.diffuseAlbedo[3] = 1.3
test_m_1.diffuseAlbedo[4] = 1.4
test_m_1.fresnelR[1] = 1.1
test_m_1.fresnelR[2] = 1.2
test_m_1.fresnelR[3] = 1.3
test_m_1.roughness = 1.11
test_m_1.normalMap = test_t_1.name

test_m_2 = Material.new("whiteMat")
test_m_2.diffuseAlbedo[1] = 2.1
test_m_2.diffuseAlbedo[2] = 2.2
test_m_2.diffuseAlbedo[3] = 2.3
test_m_2.diffuseAlbedo[4] = 2.4
test_m_2.fresnelR[1] = 2.1
test_m_2.fresnelR[2] = 2.2
test_m_2.fresnelR[3] = 2.3
test_m_2.roughness = 2.11
test_m_2.diffuseMap = test_t_2.name

test_m_3 = Material.new("bricks")
test_m_3.diffuseAlbedo[1] = 3.1
test_m_3.diffuseAlbedo[2] = 3.2
test_m_3.diffuseAlbedo[3] = 3.3
test_m_3.diffuseAlbedo[4] = 3.4
test_m_3.fresnelR[1] = 3.1
test_m_3.fresnelR[2] = 3.2
test_m_3.fresnelR[3] = 3.3
test_m_3.roughness = 3.11

test_m_4 = Material.new("tree")
test_m_4.diffuseAlbedo[1] = 4.1
test_m_4.diffuseAlbedo[2] = 4.2
test_m_4.diffuseAlbedo[3] = 4.3
test_m_4.diffuseAlbedo[4] = 4.4
test_m_4.fresnelR[1] = 4.1
test_m_4.fresnelR[2] = 4.2
test_m_4.fresnelR[3] = 4.3
test_m_4.roughness = 4.11
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
