if not _ACTION then
	error("no action defined")
end

if _ACTION == "clean" then
	local folders = os.matchdirs("*")
	for _, folder in pairs(folders) do
		local subfolders = os.matchdirs(folder .. "/*")
		for _, subfolder in pairs(subfolders) do
			local subsubfolders = os.matchdirs(subfolder .. "/*")
			for _, subsubfolder in pairs(subsubfolders) do
				os.rmdir(subsubfolder)
			end
		end
	end

	return
end

newoption({
	trigger = "static-runtime",
	description = "Force the use of the static C runtime (only works with static builds)"
})

newoption({
	trigger = "thirdparty-folder",
	value = "thirdparty (default)",
	description = "Sets the path of third-party libraries relative to the current system projects folder (\"thirdparty\" would use \"Projects/windows/thirdparty\", for example, on Windows)."
})

SOURCE_FOLDER = "../Source"
INCLUDE_FOLDER = "../Include"
THIRDPARTY_FOLDER = os.get() .. "/" .. (_OPTIONS["thirdparty-folder"] or "thirdparty")
PROJECT_FOLDER = os.get() .. "/" .. _ACTION

solution("MultiLibrary")
	uuid("8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942")
	language("C++")
	location(PROJECT_FOLDER)
	warnings("Extra")
	flags({"NoPCH"})
	platforms({"x86", "x64"})
	configurations({"Release", "Debug", "Static Release", "Static Debug"})

	filter("platforms:x86")
		architecture("x32")

	filter("platforms:x64")
		architecture("x64")

	filter("system:windows")
		defines({"WIN32_LEAN_AND_MEAN", "WINVER=0x0601", "_WIN32_WINNT=0x0601", "UNICODE", "_UNICODE", "_CRT_SECURE_NO_DEPRECATE"})
		includedirs({THIRDPARTY_FOLDER .. "/include"})

		filter({"system:windows", "platforms:x86"})
			libdirs({THIRDPARTY_FOLDER .. "/lib/x86"})

		filter({"system:windows", "platforms:x64"})
			libdirs({THIRDPARTY_FOLDER .. "/lib/x64"})

	filter("system:macosx")
		includedirs({THIRDPARTY_FOLDER .. "/include"})

		filter({"system:macosx", "platforms:x86"})
			libdirs({THIRDPARTY_FOLDER .. "/lib/x86"})

		filter({"system:macosx", "platforms:x64"})
			libdirs({THIRDPARTY_FOLDER .. "/lib/x64"})

	filter("configurations:Release")
		optimize("On")
		vectorextensions("SSE2")
		kind("SharedLib")
		objdir(PROJECT_FOLDER .. "/Intermediate")

		filter({"configurations:Release", "platforms:x86"})
			targetdir(PROJECT_FOLDER .. "/Release x86")

		filter({"configurations:Release", "platforms:x64"})
			targetdir(PROJECT_FOLDER .. "/Release x64")

	filter({"configurations:Debug"})
		defines({"MULTILIBRARY_DEBUG"})
		flags({"Symbols"})
		kind("SharedLib")
		objdir(PROJECT_FOLDER .. "/Intermediate")

		filter({"configurations:Debug", "platforms:x86"})
			targetdir(PROJECT_FOLDER .. "/Debug x86")

		filter({"configurations:Debug", "platforms:x64"})
			targetdir(PROJECT_FOLDER .. "/Debug x64")

	filter({"configurations:Static Release"})
		defines({"MULTILIBRARY_STATIC"})
		optimize("On")
		vectorextensions("SSE2")
		kind("StaticLib")
		objdir(PROJECT_FOLDER .. "/Intermediate")

		filter({"configurations:Static Release", "options:static-runtime"})
			flags({"StaticRuntime"})

		filter({"configurations:Static Release", "platforms:x86"})
			targetdir(PROJECT_FOLDER .. "/Static Release x86")

		filter({"configurations:Static Release", "platforms:x64"})
			targetdir(PROJECT_FOLDER .. "/Static Release x64")

	filter({"configurations:Static Debug"})
		defines({"MULTILIBRARY_DEBUG", "MULTILIBRARY_STATIC"})
		flags({"Symbols"})
		kind("StaticLib")
		objdir(PROJECT_FOLDER .. "/Intermediate")

		filter({"configurations:Static Debug", "options:static-runtime"})
			flags({"StaticRuntime"})

		filter({"configurations:Static Debug", "platforms:x86"})
			targetdir(PROJECT_FOLDER .. "/Static Debug x86")

		filter({"configurations:Static Debug", "platforms:x64"})
			targetdir(PROJECT_FOLDER .. "/Static Debug x64")

	project("Testing")
		uuid("A9FBF5DC-08A5-1840-9169-FA049E25EBA7")
		kind("ConsoleApp")
		includedirs({INCLUDE_FOLDER})
		vpaths({["Headers"] = SOURCE_FOLDER .. "/Testing/**.hpp", ["Sources"] = SOURCE_FOLDER .. "/Testing/**.cpp"})
		files({SOURCE_FOLDER .. "/Testing/*.cpp", SOURCE_FOLDER .. "/Testing/*.hpp"})
		links({"Media", "Filesystem", "Network", "Window", "Visual", "Common"})

		filter({"system:windows", "configurations:Static Debug or Static Release"})
			links({"avcodec", "avformat", "avutil", "swscale", "swresample", "openal32", "ws2_32", "glew32", "opengl32", "gdi32"})

		filter({"system:windows", "configurations:Debug or Release"})
			links({"opengl32"})

		--filter("system:linux")
			--linkoptions({"-Wl,-rpath=./"})

		filter({"system:linux", "configurations:Static Debug or Static Release"})
			links({"pthread", "avcodec", "avformat", "avutil", "swscale", "swresample", "openal", "GLEW", "GL"})

		filter({"system:linux", "configurations:Debug or Release"})
			links({"GL"})

		filter({"system:macosx", "configurations:Static Debug or Static Release"})
			links({"pthread", "avcodec", "avformat", "avutil", "swscale", "swresample", "GLEW"})
			linkoptions({"-framework OpenAL", "-framework OpenGL"})

		filter({"system:macosx", "configurations:Debug or Release"})
			linkoptions({"-framework OpenGL"})

	group("MultiLibrary")
		project("Common")
			uuid("B172660C-0AB8-B24F-8BED-F729A0DE3CBB")
			targetname("MultiLibraryCommon")
			defines({"MULTILIBRARY_COMMON_EXPORT"})
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
			targetname("MultiLibraryMedia")
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

			filter({"system:linux", "configurations:Debug or Release"})
				links({"Common", "avcodec", "avformat", "avutil", "swscale", "swresample", "openal"})

			filter({"system:macosx", "configurations:Debug or Release"})
				links({"Common", "avcodec", "avformat", "avutil", "swscale", "swresample"})
				linkoptions({"-framework OpenAL"})

		project("Filesystem")
			uuid("19FB0DFA-1EC6-9C48-A24A-375F1A17B432")
			targetname("MultiLibraryFilesystem")
			defines({"MULTILIBRARY_FILESYSTEM_EXPORT"})
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
					links({"Common"})

			filter("system:linux")
				files({
					SOURCE_FOLDER .. "/MultiLibrary/Filesystem/Linux/*.cpp",
					SOURCE_FOLDER .. "/MultiLibrary/Filesystem/Linux/*.hpp"
				})

				filter({"system:linux", "configurations:Debug or Release"})
					links({"Common"})

			filter("system:macosx")
				files({
					SOURCE_FOLDER .. "/MultiLibrary/Filesystem/MacOSX/*.cpp",
					SOURCE_FOLDER .. "/MultiLibrary/Filesystem/MacOSX/*.hpp"
				})

				filter({"system:macosx", "configurations:Debug or Release"})
					links({"Common"})

		project("Network")
			uuid("5734F344-4AC5-B34A-8C8E-A614CFD25A91")
			targetname("MultiLibraryNetwork")
			defines({"MULTILIBRARY_NETWORK_EXPORT"})
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
				links({"Common"})

			filter({"system:macosx", "configurations:Debug or Release"})
				links({"Common"})

		project("Window")
			uuid("B4B2441D-77C4-7A4E-9A89-319CE6A0F2E3")
			targetname("MultiLibraryWindow")
			defines({"MULTILIBRARY_WINDOW_EXPORT", "GLEW_STATIC"})
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

			filter("system:windows")
				files({
					SOURCE_FOLDER .. "/MultiLibrary/Window/Windows/*.cpp",
					SOURCE_FOLDER .. "/MultiLibrary/Window/Windows/*.hpp"
				})

				filter({"system:windows", "configurations:Debug or Release"})
					links({"Common", "opengl32", "gdi32", "glew32"})

			filter("system:linux")
				files({
					SOURCE_FOLDER .. "/MultiLibrary/Window/Linux/*.cpp",
					SOURCE_FOLDER .. "/MultiLibrary/Window/Linux/*.hpp"
				})

				filter({"system:linux", "configurations:Debug or Release"})
					links({"Common", "GL", "glew"})

			filter("system:macosx")
				files({
					SOURCE_FOLDER .. "/MultiLibrary/Window/MacOSX/*.cpp",
					SOURCE_FOLDER .. "/MultiLibrary/Window/MacOSX/*.hpp"
				})

				filter({"system:macosx", "configurations:Debug or Release"})
					links({"Common", "glew"})
					linkoptions({"-framework OpenGL"})

		project("Visual")
			uuid("64CE3AA5-430D-9548-8D34-58F982E583EF")
			targetname("MultiLibraryVisual")
			defines({"MULTILIBRARY_VISUAL_EXPORT"})
			includedirs({INCLUDE_FOLDER, SOURCE_FOLDER})
			vpaths({
				["Headers"] = {
					SOURCE_FOLDER .. "/MultiLibrary/**.hpp",
					INCLUDE_FOLDER .. "/MultiLibrary/**.hpp"
				},
				["Sources"] = SOURCE_FOLDER .. "/MultiLibrary/**.cpp"
			})
			files({
				SOURCE_FOLDER .. "/MultiLibrary/Visual/*.cpp",
				SOURCE_FOLDER .. "/MultiLibrary/Visual/*.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Visual/*.hpp",
				INCLUDE_FOLDER .. "/MultiLibrary/Visual.hpp"
			})

			filter({"system:windows", "configurations:Debug or Release"})
				links({"Window", "Common", "glew32", "opengl32"})

			filter({"system:linux", "configurations:Debug or Release"})
				links({"Window", "Common", "GLEW", "GL"})

			filter({"system:macosx", "configurations:Debug or Release"})
				links({"Window", "Common", "GLEW"})
				linkoptions("-framework OpenGL")