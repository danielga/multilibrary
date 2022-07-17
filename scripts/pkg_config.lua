local function is_non_empty_string(value)
	return type(value) == "string" and #value ~= 0
end

if os.istarget("windows") then
	function pkg_config() end
elseif is_non_empty_string(os.outputof("pkg-config --version")) then
	local pkg_config_parsers = {
		l = function(lib)
			links(lib)
		end,
		L = function(libsfolder)
			syslibdirs(libsfolder)
		end,
		I = function(incfolder)
			sysincludedirs(incfolder)
		end,
		D = function(define)
			defines(define)
		end
	}

	function pkg_config(cmds)
		local output = os.outputof("pkg-config --silence-errors " .. table.concat(cmds, " "))
		if not output or output == "" then
			return
		end

		for w in output:gmatch("%S+") do
			local l = w:sub(2, 2)
			if w:sub(1, 1) == "-" and pkg_config_parsers[l] then
				pkg_config_parsers[l](w:sub(3))
			else
				print("unrecognized pkg-config output '" .. w .. "'")
			end
		end
	end
else
	print("pkg-config isn't installed, going with the default behavior of premake links")

	function pkg_config(cmds)
		local shouldlink = false
		local libs = {}
		for i = 1, #cmds do
			if cmds[i] == "--libs" then
				shouldlink = true
			elseif cmds[i]:sub(1, 1) ~= "-" then
				table.insert(libs, cmds[i]:match("^lib(.+)$") or cmds[i])
			end
		end

		if shouldlink then
			links(libs)
		end
	end
end
