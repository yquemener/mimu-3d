--Minetest
--Copyright (C) 2014 sapier
--
--This program is free software; you can redistribute it and/or modify
--it under the terms of the GNU Lesser General Public License as published by
--the Free Software Foundation; either version 2.1 of the License, or
--(at your option) any later version.
--
--This program is distributed in the hope that it will be useful,
--but WITHOUT ANY WARRANTY; without even the implied warranty of
--MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--GNU Lesser General Public License for more details.
--
--You should have received a copy of the GNU Lesser General Public License along
--with this program; if not, write to the Free Software Foundation, Inc.,
--51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

mt_color_grey  = "#AAAAAA"
mt_color_blue  = "#6389FF"
mt_color_green = "#72FF63"
mt_color_dark_green = "#25C191"

--for all other colors ask sfan5 to complete his work!

local menupath = core.get_mainmenu_path()
local basepath = core.get_builtin_path()
local menustyle = core.settings:get("main_menu_style")
defaulttexturedir = core.get_texturepath_share() .. DIR_DELIM .. "base" ..
					DIR_DELIM .. "pack" .. DIR_DELIM

dofile(basepath .. "common" .. DIR_DELIM .. "async_event.lua")
dofile(basepath .. "common" .. DIR_DELIM .. "filterlist.lua")
dofile(basepath .. "fstk" .. DIR_DELIM .. "buttonbar.lua")
dofile(basepath .. "fstk" .. DIR_DELIM .. "dialog.lua")
dofile(basepath .. "fstk" .. DIR_DELIM .. "tabview.lua")
dofile(basepath .. "fstk" .. DIR_DELIM .. "ui.lua")
dofile(menupath .. DIR_DELIM .. "common.lua")
dofile(menupath .. DIR_DELIM .. "pkgmgr.lua")
dofile(menupath .. DIR_DELIM .. "textures.lua")

dofile(menupath .. DIR_DELIM .. "dlg_config_world.lua")
dofile(menupath .. DIR_DELIM .. "dlg_settings_advanced.lua")
dofile(menupath .. DIR_DELIM .. "dlg_contentstore.lua")
if menustyle ~= "simple" then
	dofile(menupath .. DIR_DELIM .. "dlg_create_world.lua")
	dofile(menupath .. DIR_DELIM .. "dlg_delete_content.lua")
	dofile(menupath .. DIR_DELIM .. "dlg_delete_world.lua")
	dofile(menupath .. DIR_DELIM .. "dlg_rename_modpack.lua")
end

dofile(menupath .. DIR_DELIM .. "lalamenu.lua")

--------------------------------------------------------------------------------
local function main_event_handler(tabview, event)
	if event == "MenuQuit" then
		core.close()
	end
	return true
end

--------------------------------------------------------------------------------
local function init_globals()
	-- Init gamedata
	gamedata.worldindex = 0

	if menustyle == "simple" then
		local world_list = core.get_worlds()
		local world_index

		local found_singleplayerworld = false
		for i, world in ipairs(world_list) do
			if world.name == "singleplayerworld" then
				found_singleplayerworld = true
				world_index = i
				break
			end
		end

		if not found_singleplayerworld then
			core.create_world("singleplayerworld", 1)

			world_list = core.get_worlds()

			for i, world in ipairs(world_list) do
				if world.name == "singleplayerworld" then
					world_index = i
					break
				end
			end
		end

		gamedata.worldindex = world_index
	else
		menudata.worldlist = filterlist.create(
			core.get_worlds,
			compare_worlds,
			-- Unique id comparison function
			function(element, uid)
				return element.name == uid
			end,
			-- Filter function
			function(element, gameid)
				return element.gameid == gameid
			end
		)

		menudata.worldlist:add_sort_mechanism("alphabetic", sort_worlds_alphabetic)
		menudata.worldlist:set_sortmode("alphabetic")

		if not core.settings:get("menu_last_game") then
			local default_game = core.settings:get("default_game") or "minetest"
			core.settings:set("menu_last_game", default_game)
		end

		mm_texture.init()
	end

	-- Create main tabview
	-- local tv_main = tabview_create("maintab", {x = 12, y = 5.4}, {x = 0, y = 0})
	local tv_main = lala_create("mainwidget", {x = 12, y = 5.4}, {x = 0, y = 0})

	tv_main:set_global_event_handler(main_event_handler)
	tv_main:set_fixed_size(false)

	ui.set_default("mainwidget")
	tv_main:show()

	ui.update()

	core.sound_play("main_menu", true)
end

init_globals()
