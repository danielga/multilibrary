newoption({
	trigger = "static-runtime",
	description = "Force the use of the static C runtime (only works with static builds)"
})

newoption({
	trigger = "compile-glew",
	description = "Compile GLEW ourselves instead of linking"
})

newoption({
	trigger = "thirdparty-folder",
	value = "thirdparty (default)",
	description = "Sets the path of third-party libraries relative to the current system projects folder (\"thirdparty\" would use \"Projects/windows/thirdparty\", for example, on Windows)."
})

if not _ACTION then
	error("no action defined")
end

SOURCE_FOLDER = "../source"
INCLUDE_FOLDER = "../include"
THIRDPARTY_FOLDER = os.target() .. "/" .. (_OPTIONS["thirdparty-folder"] or "thirdparty")
PROJECT_FOLDER = os.target() .. "/" .. _ACTION

if _ACTION == "clean" then
	local rmfmt = "rmdir /s /q \"" .. _MAIN_SCRIPT_DIR .. "/%s\""
	if not os.is("windows") then
		rmfmt = "rm -rf \"" .. _MAIN_SCRIPT_DIR .. "/%s\""
	end

	local folders = os.matchdirs(os.target() .. "/*")
	for _, folder in pairs(folders) do
		if folder ~= THIRDPARTY_FOLDER then
			os.outputof(rmfmt:format(folder))
		end
	end

	return
end

local pkg_config_parsers = {
	l = function(lib)
		links(lib)
	end,
	L = function(libsfolder)
		libdirs(libsfolder)
	end,
	I = function(incfolder)
		includedirs(incfolder)
	end,
	D = function(define)
		defines(define)
	end
}

local function pkg_config(cmds)
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

local version, code = os.outputof("pkg-config --version")
if version ~= "" and version ~= "'pkg-config' is not recognized as an internal or external command,\noperable program or batch file." then
	pkg_config = function(cmds)
		local output = os.outputof("pkg-config --silence-errors " .. table.concat(cmds, " "))
		if output ~= "" then
			for w in output:gmatch("%S+") do
				local l = w:sub(2, 2)
				if w:sub(1, 1) == "-" and pkg_config_parsers[l] then
					pkg_config_parsers[l](w:sub(3))
				else
					print("unrecognized pkg-config output '" .. w .. "'")
				end
			end
		end
	end
else
	print("pkg-config doesn't exist, going with the default behavior of premake links")
end

solution("MultiLibrary")
	uuid("8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942")
	language("C++")
	location(PROJECT_FOLDER)
	warnings("Extra")
	flags("NoPCH")
	characterset("Unicode")
	platforms({"x86", "x64"})
	configurations({"Release", "Debug", "StaticRelease", "StaticDebug"})
	startproject("Testing")

	filter("platforms:x86")
		architecture("x32")

	filter("platforms:x64")
		architecture("x64")

	filter("options:static-runtime")
		flags("StaticRuntime")

	filter("action:gmake")
		buildoptions("-std=c++11")

	filter("system:windows")
		defines({"UNICODE", "_UNICODE", "WIN32_LEAN_AND_MEAN", "WINVER=0x0601", "_WIN32_WINNT=0x0601", "_CRT_SECURE_NO_DEPRECATE"})
		includedirs(THIRDPARTY_FOLDER .. "/include")

		filter({"system:windows", "platforms:x86"})
			libdirs(THIRDPARTY_FOLDER .. "/lib/x86")

		filter({"system:windows", "platforms:x64"})
			libdirs(THIRDPARTY_FOLDER .. "/lib/x64")

	filter("system:linux")
		includedirs(THIRDPARTY_FOLDER .. "/include")

	filter("system:macosx")
		includedirs(THIRDPARTY_FOLDER .. "/include")

	filter("configurations:Release")
		optimize("On")
		vectorextensions("SSE2")
		kind("SharedLib")
		objdir(PROJECT_FOLDER .. "/intermediate")

		filter({"configurations:Release", "platforms:x86"})
			targetdir(PROJECT_FOLDER .. "/x86/release")
			debugdir(PROJECT_FOLDER .. "/x86/release")

		filter({"configurations:Release", "platforms:x64"})
			targetdir(PROJECT_FOLDER .. "/x64/release")
			debugdir(PROJECT_FOLDER .. "/x64/release")

	filter("configurations:Debug")
		defines("MULTILIBRARY_DEBUG")
		symbols("On")
		kind("SharedLib")
		objdir(PROJECT_FOLDER .. "/intermediate")

		filter({"configurations:Debug", "platforms:x86"})
			targetdir(PROJECT_FOLDER .. "/x86/debug")
			debugdir(PROJECT_FOLDER .. "/x86/debug")

		filter({"configurations:Debug", "platforms:x64"})
			targetdir(PROJECT_FOLDER .. "/x64/debug")
			debugdir(PROJECT_FOLDER .. "/x64/debug")

	filter("configurations:StaticRelease")
		defines("MULTILIBRARY_STATIC")
		optimize("On")
		vectorextensions("SSE2")
		kind("StaticLib")
		objdir(PROJECT_FOLDER .. "/intermediate")

		filter({"configurations:StaticRelease", "options:static-runtime"})
			flags("StaticRuntime")

		filter({"configurations:StaticRelease", "platforms:x86"})
			targetdir(PROJECT_FOLDER .. "/x86/staticrelease")
			debugdir(PROJECT_FOLDER .. "/x86/staticrelease")

		filter({"configurations:StaticRelease", "platforms:x64"})
			targetdir(PROJECT_FOLDER .. "/x64/staticrelease")
			debugdir(PROJECT_FOLDER .. "/x64/staticrelease")

	filter("configurations:StaticDebug")
		defines({"MULTILIBRARY_DEBUG", "MULTILIBRARY_STATIC"})
		symbols("On")
		kind("StaticLib")
		objdir(PROJECT_FOLDER .. "/intermediate")

		filter({"configurations:StaticDebug", "options:static-runtime"})
			flags("StaticRuntime")

		filter({"configurations:StaticDebug", "platforms:x86"})
			targetdir(PROJECT_FOLDER .. "/x86/staticdebug")
			debugdir(PROJECT_FOLDER .. "/x86/staticdebug")

		filter({"configurations:StaticDebug", "platforms:x64"})
			targetdir(PROJECT_FOLDER .. "/x64/staticdebug")
			debugdir(PROJECT_FOLDER .. "/x64/staticdebug")

	project("Testing")
		uuid("A9FBF5DC-08A5-1840-9169-FA049E25EBA7")
		kind("ConsoleApp")
		targetname("testing")
		includedirs(INCLUDE_FOLDER)
		vpaths({["Sources"] = SOURCE_FOLDER .. "/Testing/**.cpp"})
		files(SOURCE_FOLDER .. "/Testing/main.cpp")
		links({"Media", "Filesystem", "Network", "Window", "Visual", "Common"})

		filter("options:compile-glew")
			defines("GLEW_STATIC")
			vpaths({["Sources"] = THIRDPARTY_FOLDER .. "/src/**.c"})
			files(THIRDPARTY_FOLDER .. "/src/glew.c")

		filter({"system:windows", "configurations:StaticDebug or StaticRelease"})
			links({"avcodec", "avformat", "avutil", "swscale", "swresample", "openal32", "ws2_32", "opengl32", "gdi32"})

			filter({"system:windows", "configurations:StaticDebug or StaticRelease", "options:not compile-glew"})
				links("glew32")

		filter({"system:windows", "configurations:Debug or Release"})
			links("opengl32")

		filter({"system:linux", "configurations:StaticDebug or StaticRelease"})
			pkg_config({"--cflags", "--libs", "x11", "gl", "openal", "libavcodec", "libavformat", "libavutil", "libswscale", "libswresample"})
			links("pthread")

			filter({"system:linux", "configurations:StaticDebug or StaticRelease", "options:not compile-glew"})
				pkg_config({"--cflags", "--libs", "glew"})

		filter({"system:linux", "configurations:Debug or Release"})
			pkg_config({"--cflags", "--libs", "gl"})
			links("pthread")

		filter({"system:macosx", "configurations:StaticDebug or StaticRelease"})
			links({"pthread", "avcodec", "avformat", "avutil", "swscale", "swresample", "OpenAL.framework", "OpenGL.framework"})

			filter({"system:macosx", "configurations:StaticDebug or StaticRelease", "options:not compile-glew"})
				links("GLEW")

		filter({"system:macosx", "configurations:Debug or Release"})
			links("OpenGL.framework")

	project("Child")
		uuid("CF9AA3B0-6410-11E4-9803-0800200C9A66")
		kind("ConsoleApp")
		targetname("child")
		includedirs(INCLUDE_FOLDER)
		vpaths({["Sources"] = SOURCE_FOLDER .. "/Testing/**.cpp"})
		files(SOURCE_FOLDER .. "/Testing/child.cpp")
		links({"Filesystem", "Common"})

	group("MultiLibrary")
		project("Common")
			uuid("B172660C-0AB8-B24F-8BED-F729A0DE3CBB")
			targetname("ml-common")
			defines("MULTILIBRARY_COMMON_EXPORT")
			includedirs({INCLUDE_FOLDER, SOURCE_FOLDER})
			vpaths({
				["Headers"] = {
					SOURCE_FOLDER .. "/MultiLibrary/**.hpp",
					INCLUDE_FOLDER .. "/MultiLibrary/**.hpp",
					INCLUDE_FOLDER .. "/MultiLibrary/**.inl"
				},
				["Sources"] = SOURCE_FOLDER .. "/MultiLibrary/**.cpp"
			})
			files({
				SOURCE_FOLDER .. "/MultiLibrary/Common/*.cpp",
				SOURCE_FOLDER .. "/MultiLibrary/Common/*.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Common/*.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Common/*.inl",
				INCLUDE_FOLDER .. "/MultiLibrary/Common.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Config.hpp"
			})

			filter("system:windows")
				files({
					SOURCE_FOLDER .. "/MultiLibrary/Common/Windows/*.cpp",
					SOURCE_FOLDER .. "/MultiLibrary/Common/Windows/*.hpp"
				})

			filter("system:linux")
				files({
					SOURCE_FOLDER .. "/MultiLibrary/Common/Linux/*.cpp",
					SOURCE_FOLDER .. "/MultiLibrary/Common/Linux/*.hpp"
				})

			filter("system:macosx")
				files({
					SOURCE_FOLDER .. "/MultiLibrary/Common/MacOSX/*.cpp",
					SOURCE_FOLDER .. "/MultiLibrary/Common/MacOSX/*.hpp"
				})

		project("Media")
			uuid("C07CB0BB-E779-B64A-A91C-F7E31A0992C3")
			targetname("ml-media")
			defines({"MULTILIBRARY_MEDIA_EXPORT", "__STDC_CONSTANT_MACROS"})
			includedirs({INCLUDE_FOLDER, SOURCE_FOLDER})
			vpaths({
				["Headers"] = {
					SOURCE_FOLDER .. "/MultiLibrary/**.hpp",
					INCLUDE_FOLDER .. "/MultiLibrary/**.hpp"
				},
				["Sources"] = SOURCE_FOLDER .. "/MultiLibrary/**.cpp"
			})
			files({
				SOURCE_FOLDER .. "/MultiLibrary/Media/*.cpp",
				SOURCE_FOLDER .. "/MultiLibrary/Media/*.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Media/*.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Media.hpp"
			})

			filter({"system:windows", "configurations:Debug or Release"})
				links({"Common", "avcodec", "avformat", "avutil", "swscale", "swresample", "openal32"})

			filter("system:linux")
				pkg_config({"--cflags", "openal", "libavcodec", "libavformat", "libavutil", "libswscale", "libswresample"})

				filter({"system:linux", "configurations:Debug or Release"})
					links("Common")
					pkg_config({"--libs", "openal", "libavcodec", "libavformat", "libavutil", "libswscale", "libswresample"})

			filter({"system:macosx", "configurations:Debug or Release"})
				links({"Common", "avcodec", "avformat", "avutil", "swscale", "swresample", "OpenAL.framework"})

		project("Filesystem")
			uuid("19FB0DFA-1EC6-9C48-A24A-375F1A17B432")
			targetname("ml-filesystem")
			defines("MULTILIBRARY_FILESYSTEM_EXPORT")
			includedirs({INCLUDE_FOLDER, SOURCE_FOLDER})
			vpaths({
				["Headers"] = {
					SOURCE_FOLDER .. "/MultiLibrary/**.hpp",
					INCLUDE_FOLDER .. "/MultiLibrary/**.hpp"
				},
				["Sources"] = SOURCE_FOLDER .. "/MultiLibrary/**.cpp"
			})
			files({
				SOURCE_FOLDER .. "/MultiLibrary/Filesystem/*.cpp",
				SOURCE_FOLDER .. "/MultiLibrary/Filesystem/*.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Filesystem/*.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Filesystem.hpp"
			})

			filter("system:windows")
				files({
					SOURCE_FOLDER .. "/MultiLibrary/Filesystem/Windows/*.cpp",
					SOURCE_FOLDER .. "/MultiLibrary/Filesystem/Windows/*.hpp"
				})

				filter({"system:windows", "configurations:Debug or Release"})
					links("Common")

			filter("system:linux")
				files({
					SOURCE_FOLDER .. "/MultiLibrary/Filesystem/Linux/*.cpp",
					SOURCE_FOLDER .. "/MultiLibrary/Filesystem/Linux/*.hpp"
				})

				filter({"system:linux", "configurations:Debug or Release"})
					links("Common")

			filter("system:macosx")
				files({
					SOURCE_FOLDER .. "/MultiLibrary/Filesystem/MacOSX/*.cpp",
					SOURCE_FOLDER .. "/MultiLibrary/Filesystem/MacOSX/*.hpp"
				})

				filter({"system:macosx", "configurations:Debug or Release"})
					links("Common")

		project("Network")
			uuid("5734F344-4AC5-B34A-8C8E-A614CFD25A91")
			targetname("ml-network")
			defines("MULTILIBRARY_NETWORK_EXPORT")
			includedirs({INCLUDE_FOLDER, SOURCE_FOLDER})
			vpaths({
				["Headers"] = {
					SOURCE_FOLDER .. "/MultiLibrary/**.hpp",
					INCLUDE_FOLDER .. "/MultiLibrary/**.hpp"
				},
				["Sources"] = SOURCE_FOLDER .. "/MultiLibrary/**.cpp"
			})
			files({
				SOURCE_FOLDER .. "/MultiLibrary/Network/*.cpp",
				SOURCE_FOLDER .. "/MultiLibrary/Network/*.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Network/*.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Network.hpp"
			})

			filter({"system:windows", "configurations:Debug or Release"})
				links({"Common", "ws2_32"})

			filter({"system:linux", "configurations:Debug or Release"})
				links("Common")

			filter({"system:macosx", "configurations:Debug or Release"})
				links("Common")

		project("Window")
			uuid("B4B2441D-77C4-7A4E-9A89-319CE6A0F2E3")
			targetname("ml-window")
			defines("MULTILIBRARY_WINDOW_EXPORT")
			includedirs({INCLUDE_FOLDER, SOURCE_FOLDER})
			vpaths({
				["Headers"] = {
					SOURCE_FOLDER .. "/MultiLibrary/**.hpp",
					INCLUDE_FOLDER .. "/MultiLibrary/**.hpp"
				},
				["Sources"] = SOURCE_FOLDER .. "/MultiLibrary/**.cpp"
			})
			files({
				SOURCE_FOLDER .. "/MultiLibrary/Window/*.cpp",
				SOURCE_FOLDER .. "/MultiLibrary/Window/*.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Window/*.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Window.hpp"
			})

			filter("options:compile-glew")
				defines("GLEW_STATIC")
				vpaths({["Sources"] = THIRDPARTY_FOLDER .. "/src/**.c"})
				files(THIRDPARTY_FOLDER .. "/src/glew.c")

			filter("system:windows")
				files({
					SOURCE_FOLDER .. "/MultiLibrary/Window/Windows/*.cpp",
					SOURCE_FOLDER .. "/MultiLibrary/Window/Windows/*.hpp"
				})

				filter({"system:windows", "configurations:Debug or Release"})
					links({"Common", "gdi32", "opengl32"})

					filter({"system:windows", "configurations:Debug or Release", "options:not compile-glew"})
						links("glew32")

			filter("system:linux")
				files({
					SOURCE_FOLDER .. "/MultiLibrary/Window/Linux/*.cpp",
					SOURCE_FOLDER .. "/MultiLibrary/Window/Linux/*.hpp"
				})

				filter({"system:linux", "configurations:Debug or Release"})
					links("Common")
					pkg_config({"--cflags", "--libs", "x11", "gl"})

					filter({"system:linux", "configurations:Debug or Release", "options:not compile-glew"})
						pkg_config({"--cflags", "--libs", "glew"})

			filter("system:macosx")
				files({
					SOURCE_FOLDER .. "/MultiLibrary/Window/MacOSX/*.cpp",
					SOURCE_FOLDER .. "/MultiLibrary/Window/MacOSX/*.hpp"
				})

				filter({"system:macosx", "configurations:Debug or Release"})
					links({"Common", "OpenGL.framework"})

					filter({"system:macosx", "configurations:Debug or Release", "options:not compile-glew"})
						links("GLEW")

		project("Visual")
			uuid("64CE3AA5-430D-9548-8D34-58F982E583EF")
			targetname("ml-visual")
			defines("MULTILIBRARY_VISUAL_EXPORT")
			includedirs({INCLUDE_FOLDER, SOURCE_FOLDER})
			vpaths({
				["Headers"] = {
					SOURCE_FOLDER .. "/MultiLibrary/**.hpp",
					INCLUDE_FOLDER .. "/MultiLibrary/**.hpp"
				},
				["Sources"] = {
					SOURCE_FOLDER .. "/MultiLibrary/**.cpp"
				}
			})
			files({
				SOURCE_FOLDER .. "/MultiLibrary/Visual/*.cpp",
				SOURCE_FOLDER .. "/MultiLibrary/Visual/*.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Visual/*.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Visual.hpp"
			})

			filter("options:compile-glew")
				defines("GLEW_STATIC")
				vpaths({["Sources"] = THIRDPARTY_FOLDER .. "/src/**.c"})
				files(THIRDPARTY_FOLDER .. "/src/glew.c")

			filter({"system:windows", "configurations:Debug or Release"})
				links({"Window", "Common", "opengl32"})

				filter({"system:windows", "configurations:Debug or Release", "options:not compile-glew"})
						links("glew32")

			filter({"system:linux", "configurations:Debug or Release"})
				links({"Window", "Common"})
				pkg_config({"--cflags", "--libs", "gl"})

				filter({"system:linux", "configurations:Debug or Release", "options:not compile-glew"})
						pkg_config({"--cflags", "--libs", "glew"})

			filter({"system:macosx", "configurations:Debug or Release"})
				links({"Window", "Common", "OpenGL.framework"})

				filter({"system:macosx", "configurations:Debug or Release", "options:not compile-glew"})
						links("GLEW")
