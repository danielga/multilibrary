if not _ACTION then
	error("no action defined")
end

newoption({
	trigger = "static-runtime",
	description = "Force the use of the static C runtime (only works with static builds)"
})

newoption({
	trigger = "glew-linking",
	description = "How to link with GLEW",
	value = "Should be 'static', 'dynamic' or 'compile'",
	default = "static",
	allowed = {
		{"static", "Statically links against GLEW"},
		{"dynamic", "Dynamically links with GLEW"},
		{"compile", "Compiles GLEW from source code"}
	}
})

newoption({
	trigger = "thirdparty-directory",
	value = "thirdparty (default)",
	description = "Sets the path of third-party libraries relative to the current system projects directory (\"thirdparty\" would use \"projects/windows/thirdparty\", for example, on Windows)."
})

SOURCE_DIRECTORY = "source"
INCLUDE_DIRECTORY = "include"
THIRDPARTY_DIRECTORY = "projects/" .. os.target() .. "/" .. (_OPTIONS["thirdparty-directory"] or "thirdparty")
PROJECT_DIRECTORY = "projects/" .. os.target() .. "/" .. _ACTION

if _ACTION == "clean" then
	local rmfmt = "rmdir /s /q \"" .. _MAIN_SCRIPT_DIR .. "/%s\""
	if not os.ishost("windows") then
		rmfmt = "rm -rf \"" .. _MAIN_SCRIPT_DIR .. "/%s\""
	end

	local dirs = os.matchdirs("projects/" .. os.target() .. "/*")
	for _, dir in pairs(dirs) do
		if dir ~= THIRDPARTY_DIRECTORY then
			os.outputof(rmfmt:format(dir))
		end
	end

	return
end

local pkg_config_parsers = {
	l = function(lib)
		links(lib)
	end,
	L = function(libsdir)
		libdirs(libsdir)
	end,
	I = function(incdir)
		includedirs(incdir)
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

local version = os.outputof("pkg-config --version")
if version and version ~= "" and version ~= "'pkg-config' is not recognized as an internal or external command,\noperable program or batch file." then
	pkg_config = function(cmds)
		local output = os.outputof("pkg-config --silence-errors " .. table.concat(cmds, " "))
		if output and output ~= "" then
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
	cppdialect("C++11")
	location(PROJECT_DIRECTORY)
	warnings("Extra")
	flags("NoPCH")
	characterset("Unicode")
	platforms({"x86", "x64"})
	configurations({"Release", "Debug", "Release (static)", "Debug (static)"})
	startproject("Testing")

	filter("platforms:x86")
		architecture("x32")

	filter("platforms:x64")
		architecture("x64")

	filter("options:static-runtime")
		staticruntime("On")

	filter("system:windows")
		defines({"UNICODE", "_UNICODE", "WIN32_LEAN_AND_MEAN", "WINVER=0x0601", "_WIN32_WINNT=0x0601", "_CRT_SECURE_NO_DEPRECATE"})
		includedirs(THIRDPARTY_DIRECTORY .. "/include")

		filter({"system:windows", "platforms:x86"})
			libdirs(THIRDPARTY_DIRECTORY .. "/lib/x86")

		filter({"system:windows", "platforms:x64"})
			libdirs(THIRDPARTY_DIRECTORY .. "/lib/x64")

	filter("system:linux")
		includedirs(THIRDPARTY_DIRECTORY .. "/include")

	filter("system:macosx")
		includedirs(THIRDPARTY_DIRECTORY .. "/include")

	filter("configurations:Release")
		optimize("On")
		vectorextensions("SSE2")
		kind("SharedLib")

		filter({"configurations:Release", "platforms:x86"})
			targetdir(PROJECT_DIRECTORY .. "/x86/release")
			debugdir(PROJECT_DIRECTORY .. "/x86/release")
			objdir("!" .. PROJECT_DIRECTORY .. "/x86/release/intermediate/%{prj.name}")

		filter({"configurations:Release", "platforms:x64"})
			targetdir(PROJECT_DIRECTORY .. "/x64/release")
			debugdir(PROJECT_DIRECTORY .. "/x64/release")
			objdir("!" .. PROJECT_DIRECTORY .. "/x64/release/intermediate/%{prj.name}")

	filter("configurations:Debug")
		defines("MULTILIBRARY_DEBUG")
		symbols("On")
		kind("SharedLib")

		filter({"configurations:Debug", "platforms:x86"})
			targetdir(PROJECT_DIRECTORY .. "/x86/debug")
			debugdir(PROJECT_DIRECTORY .. "/x86/debug")
			objdir("!" .. PROJECT_DIRECTORY .. "/x86/debug/intermediate/%{prj.name}")

		filter({"configurations:Debug", "platforms:x64"})
			targetdir(PROJECT_DIRECTORY .. "/x64/debug")
			debugdir(PROJECT_DIRECTORY .. "/x64/debug")
			objdir("!" .. PROJECT_DIRECTORY .. "/x64/debug/intermediate/%{prj.name}")

	filter("configurations:Release (static)")
		defines("MULTILIBRARY_STATIC")
		optimize("On")
		vectorextensions("SSE2")
		kind("StaticLib")

		filter({"configurations:Release (static)", "options:static-runtime"})
			staticruntime("On")

		filter({"configurations:Release (static)", "platforms:x86"})
			targetdir(PROJECT_DIRECTORY .. "/x86/release-static")
			debugdir(PROJECT_DIRECTORY .. "/x86/release-static")
			objdir("!" .. PROJECT_DIRECTORY .. "/x86/release-static/intermediate/%{prj.name}")

		filter({"configurations:Release (static)", "platforms:x64"})
			targetdir(PROJECT_DIRECTORY .. "/x64/release-static")
			debugdir(PROJECT_DIRECTORY .. "/x64/release-static")
			objdir("!" .. PROJECT_DIRECTORY .. "/x64/release-static/intermediate/%{prj.name}")

	filter("configurations:Debug (static)")
		defines({"MULTILIBRARY_DEBUG", "MULTILIBRARY_STATIC"})
		symbols("On")
		kind("StaticLib")

		filter({"configurations:Debug (static)", "options:static-runtime"})
			staticruntime("On")

		filter({"configurations:Debug (static)", "platforms:x86"})
			targetdir(PROJECT_DIRECTORY .. "/x86/debug-static")
			debugdir(PROJECT_DIRECTORY .. "/x86/debug-static")
			objdir("!" .. PROJECT_DIRECTORY .. "/x86/debug-static/intermediate/%{prj.name}")

		filter({"configurations:Debug (static)", "platforms:x64"})
			targetdir(PROJECT_DIRECTORY .. "/x64/debug-static")
			debugdir(PROJECT_DIRECTORY .. "/x64/debug-static")
			objdir("!" .. PROJECT_DIRECTORY .. "/x64/debug-static/intermediate/%{prj.name}")

	project("Testing")
		uuid("A9FBF5DC-08A5-1840-9169-FA049E25EBA7")
		kind("ConsoleApp")
		targetname("testing")
		includedirs(INCLUDE_DIRECTORY)
		vpaths({["Source files"] = SOURCE_DIRECTORY .. "/Testing/**.cpp"})
		files(SOURCE_DIRECTORY .. "/Testing/main.cpp")
		links({"Media", "Filesystem", "Network", "Window", "Visual", "Common"})

		filter("options:not glew-linking=dynamic")
			defines("GLEW_STATIC")

		filter("options:glew-linking=compile")
			vpaths({["Source files"] = THIRDPARTY_DIRECTORY .. "/src/**.c"})
			files(THIRDPARTY_DIRECTORY .. "/src/glew.c")

		filter({"system:windows", "configurations:Debug (static) or Release (static)"})
			links({"avcodec", "avformat", "avutil", "swscale", "swresample", "openal32", "ws2_32", "opengl32", "gdi32"})

			filter({"system:windows", "configurations:Debug (static)", "options:not glew-linking=compile"})
				links("glew32d")

			filter({"system:windows", "configurations:Release (static)", "options:not glew-linking=compile"})
				links("glew32")

		filter({"system:windows", "configurations:Debug or Release"})
			links("opengl32")

		filter({"system:linux", "configurations:Debug (static) or Release (static)"})
			pkg_config({"--cflags", "--libs", "x11", "gl", "openal", "libavcodec", "libavformat", "libavutil", "libswscale", "libswresample"})
			links("pthread")

			filter({"system:linux", "configurations:Debug (static) or Release (static)", "options:not glew-linking=compile"})
				pkg_config({"--cflags", "--libs", "glew"})

		filter({"system:linux", "configurations:Debug or Release"})
			pkg_config({"--cflags", "--libs", "gl"})
			links("pthread")

		filter({"system:macosx", "configurations:Debug (static) or Release (static)"})
			links({"pthread", "avcodec", "avformat", "avutil", "swscale", "swresample", "OpenAL.framework", "OpenGL.framework"})

			filter({"system:macosx", "configurations:Debug (static) or Release (static)", "options:not glew-linking=compile"})
				links("GLEW")

		filter({"system:macosx", "configurations:Debug or Release"})
			links("OpenGL.framework")

	project("Child")
		uuid("CF9AA3B0-6410-11E4-9803-0800200C9A66")
		kind("ConsoleApp")
		targetname("child")
		includedirs(INCLUDE_DIRECTORY)
		vpaths({["Source files"] = SOURCE_DIRECTORY .. "/Testing/**.cpp"})
		files(SOURCE_DIRECTORY .. "/Testing/child.cpp")
		links({"Filesystem", "Common"})

	group("MultiLibrary")
		project("Common")
			uuid("B172660C-0AB8-B24F-8BED-F729A0DE3CBB")
			targetname("ml-common")
			defines("MULTILIBRARY_COMMON_EXPORT")
			includedirs({INCLUDE_DIRECTORY, SOURCE_DIRECTORY})
			vpaths({
				["Header files"] = {
					INCLUDE_DIRECTORY .. "/MultiLibrary/**.hpp",
					INCLUDE_DIRECTORY .. "/MultiLibrary/**.inl"
				},
				["Source files"] = {
					SOURCE_DIRECTORY .. "/MultiLibrary/**.hpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/**.cpp"
				}
			})
			files({
				SOURCE_DIRECTORY .. "/MultiLibrary/Common/*.cpp",
				SOURCE_DIRECTORY .. "/MultiLibrary/Common/*.hpp",
				INCLUDE_DIRECTORY .. "/MultiLibrary/Common/*.hpp",
				INCLUDE_DIRECTORY .. "/MultiLibrary/Common/*.inl",
				INCLUDE_DIRECTORY .. "/MultiLibrary/Common.hpp",
				INCLUDE_DIRECTORY .. "/MultiLibrary/Config.hpp"
			})

			filter("system:windows")
				files({
					SOURCE_DIRECTORY .. "/MultiLibrary/Common/Windows/*.cpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/Common/Windows/*.hpp"
				})

			filter("system:linux")
				files({
					SOURCE_DIRECTORY .. "/MultiLibrary/Common/Linux/*.cpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/Common/Linux/*.hpp"
				})

			filter("system:macosx")
				files({
					SOURCE_DIRECTORY .. "/MultiLibrary/Common/MacOSX/*.cpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/Common/MacOSX/*.hpp"
				})

		project("Media")
			uuid("C07CB0BB-E779-B64A-A91C-F7E31A0992C3")
			targetname("ml-media")
			defines("MULTILIBRARY_MEDIA_EXPORT")
			includedirs({INCLUDE_DIRECTORY, SOURCE_DIRECTORY})
			vpaths({
				["Header files"] = INCLUDE_DIRECTORY .. "/MultiLibrary/**.hpp",
				["Source files"] = {
					SOURCE_DIRECTORY .. "/MultiLibrary/**.hpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/**.cpp"
				}
			})
			files({
				SOURCE_DIRECTORY .. "/MultiLibrary/Media/*.cpp",
				SOURCE_DIRECTORY .. "/MultiLibrary/Media/*.hpp",
				INCLUDE_DIRECTORY .. "/MultiLibrary/Media/*.hpp",
				INCLUDE_DIRECTORY .. "/MultiLibrary/Media.hpp"
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
			includedirs({INCLUDE_DIRECTORY, SOURCE_DIRECTORY})
			vpaths({
				["Header files"] = INCLUDE_DIRECTORY .. "/MultiLibrary/**.hpp",
				["Source files"] = {
					SOURCE_DIRECTORY .. "/MultiLibrary/**.hpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/**.cpp"
				}
			})
			files({
				SOURCE_DIRECTORY .. "/MultiLibrary/Filesystem/*.cpp",
				SOURCE_DIRECTORY .. "/MultiLibrary/Filesystem/*.hpp",
				INCLUDE_DIRECTORY .. "/MultiLibrary/Filesystem/*.hpp",
				INCLUDE_DIRECTORY .. "/MultiLibrary/Filesystem.hpp"
			})

			filter("configurations:Debug or Release")
				links("Common")

			filter("system:windows")
				files({
					SOURCE_DIRECTORY .. "/MultiLibrary/Filesystem/Windows/*.cpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/Filesystem/Windows/*.hpp"
				})

			filter("system:linux")
				files({
					SOURCE_DIRECTORY .. "/MultiLibrary/Filesystem/Linux/*.cpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/Filesystem/Linux/*.hpp"
				})

			filter("system:macosx")
				files({
					SOURCE_DIRECTORY .. "/MultiLibrary/Filesystem/MacOSX/*.cpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/Filesystem/MacOSX/*.hpp"
				})

		project("Network")
			uuid("5734F344-4AC5-B34A-8C8E-A614CFD25A91")
			targetname("ml-network")
			defines("MULTILIBRARY_NETWORK_EXPORT")
			includedirs({INCLUDE_DIRECTORY, SOURCE_DIRECTORY})
			vpaths({
				["Header files"] = INCLUDE_DIRECTORY .. "/MultiLibrary/**.hpp",
				["Source files"] = {
					SOURCE_DIRECTORY .. "/MultiLibrary/**.hpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/**.cpp"
				}
			})
			files({
				SOURCE_DIRECTORY .. "/MultiLibrary/Network/*.cpp",
				SOURCE_DIRECTORY .. "/MultiLibrary/Network/*.hpp",
				INCLUDE_DIRECTORY .. "/MultiLibrary/Network/*.hpp",
				INCLUDE_DIRECTORY .. "/MultiLibrary/Network.hpp"
			})

			filter({"system:windows", "configurations:Debug or Release"})
				links({"Common", "ws2_32"})

			filter({"system:linux or macosx", "configurations:Debug or Release"})
				links("Common")

		project("Window")
			uuid("B4B2441D-77C4-7A4E-9A89-319CE6A0F2E3")
			targetname("ml-window")
			defines("MULTILIBRARY_WINDOW_EXPORT")
			includedirs({INCLUDE_DIRECTORY, SOURCE_DIRECTORY})
			vpaths({
				["Header files"] = INCLUDE_DIRECTORY .. "/MultiLibrary/**.hpp",
				["Source files"] = {
					SOURCE_DIRECTORY .. "/MultiLibrary/**.hpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/**.cpp"
				}
			})
			files({
				SOURCE_DIRECTORY .. "/MultiLibrary/Window/*.cpp",
				SOURCE_DIRECTORY .. "/MultiLibrary/Window/*.hpp",
				INCLUDE_DIRECTORY .. "/MultiLibrary/Window/*.hpp",
				INCLUDE_DIRECTORY .. "/MultiLibrary/Window.hpp"
			})

			filter("options:not glew-linking=dynamic")
				defines("GLEW_STATIC")

			filter("options:glew-linking=compile")
				vpaths({["Source files"] = THIRDPARTY_DIRECTORY .. "/src/**.c"})
				files(THIRDPARTY_DIRECTORY .. "/src/glew.c")

			filter("system:windows")
				files({
					SOURCE_DIRECTORY .. "/MultiLibrary/Window/Windows/*.cpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/Window/Windows/*.hpp"
				})

				filter({"system:windows", "configurations:Debug or Release"})
					links({"Common", "gdi32", "opengl32"})

				filter({"system:windows", "configurations:Debug", "options:not glew-linking=compile"})
					links("glew32d")

				filter({"system:windows", "configurations:Release", "options:not glew-linking=compile"})
					links("glew32")

			filter("system:linux")
				files({
					SOURCE_DIRECTORY .. "/MultiLibrary/Window/Linux/*.cpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/Window/Linux/*.hpp"
				})

				filter({"system:linux", "configurations:Debug or Release"})
					links("Common")
					pkg_config({"--cflags", "--libs", "x11", "gl"})

					filter({"system:linux", "configurations:Debug or Release", "options:not glew-linking=compile"})
						pkg_config({"--cflags", "--libs", "glew"})

			filter("system:macosx")
				files({
					SOURCE_DIRECTORY .. "/MultiLibrary/Window/MacOSX/*.cpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/Window/MacOSX/*.hpp"
				})

				filter({"system:macosx", "configurations:Debug or Release"})
					links({"Common", "OpenGL.framework"})

					filter({"system:macosx", "configurations:Debug or Release", "options:not glew-linking=compile"})
						links("GLEW")

		project("Visual")
			uuid("64CE3AA5-430D-9548-8D34-58F982E583EF")
			targetname("ml-visual")
			defines("MULTILIBRARY_VISUAL_EXPORT")
			includedirs({INCLUDE_DIRECTORY, SOURCE_DIRECTORY})
			vpaths({
				["Header files"] = INCLUDE_DIRECTORY .. "/MultiLibrary/**.hpp",
				["Source files"] = {
					SOURCE_DIRECTORY .. "/MultiLibrary/**.hpp",
					SOURCE_DIRECTORY .. "/MultiLibrary/**.cpp"
				}
			})
			files({
				SOURCE_DIRECTORY .. "/MultiLibrary/Visual/*.cpp",
				SOURCE_DIRECTORY .. "/MultiLibrary/Visual/*.hpp",
				INCLUDE_DIRECTORY .. "/MultiLibrary/Visual/*.hpp",
				INCLUDE_DIRECTORY .. "/MultiLibrary/Visual.hpp"
			})

			filter("options:not glew-linking=dynamic")
				defines("GLEW_STATIC")

			filter("options:glew-linking=compile")
				vpaths({["Source files"] = THIRDPARTY_DIRECTORY .. "/src/**.c"})
				files(THIRDPARTY_DIRECTORY .. "/src/glew.c")

			filter({"system:windows", "configurations:Debug or Release"})
				links({"Window", "Common", "opengl32"})

				filter({"system:windows", "configurations:Debug", "options:not glew-linking=compile"})
					links("glew32d")

				filter({"system:windows", "configurations:Release", "options:not glew-linking=compile"})
					links("glew32")

			filter({"system:linux", "configurations:Debug or Release"})
				links({"Window", "Common"})
				pkg_config({"--cflags", "--libs", "gl"})

				filter({"system:linux", "configurations:Debug or Release", "options:not glew-linking=compile"})
					pkg_config({"--cflags", "--libs", "glew"})

			filter({"system:macosx", "configurations:Debug or Release"})
				links({"Window", "Common", "OpenGL.framework"})

				filter({"system:macosx", "configurations:Debug or Release", "options:not glew-linking=compile"})
					links("GLEW")
