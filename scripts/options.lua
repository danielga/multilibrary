newoption({
	trigger = "static-runtime",
	description = "Force the use of the static C runtime (only works with static builds)"
})

newoption({
	trigger = "glew-linking",
	description = "Linking strategy with GLEW",
	value = "VALUE",
	default = "compile",
	allowed = {
		{"static", "Statically links against GLEW"},
		{"dynamic", "Dynamically links with GLEW"},
		{"compile", "Compiles GLEW from source code"}
	}
})

newoption({
	trigger = "thirdparty-directory",
	description = "Path to third-party libraries, useful mostly for Windows",
	value = "PATH",
	default = "thirdparty"
})

newoption({
	trigger = "fix",
	description = "Automatically attempt to fix instances reported by the 'format' or 'lint' commands"
})

newoption({
	trigger = "format",
	description = "Reformat files on the 'lint' command (requires the '--fix' flag)"
})

newoption({
	trigger = "clang-tidy-binary",
	description = "clang-tidy binary to use on the 'lint' command",
	value = "PATH",
	default = "clang-tidy"
})

newoption({
	trigger = "clang-apply-replacements-binary",
	description = "clang-apply-replacements binary to use on the 'lint' command",
	value = "PATH",
	default = "clang-apply-replacements"
})

newoption({
	trigger = "clang-format-binary",
	description = "clang-format binary to use on the 'format' command",
	value = "PATH",
	default = "clang-format"
})

newoption({
	trigger = "jobs",
	description = "Number of parallel jobs to run on the 'lint' command",
	value = "VALUE",
	default = "4"
})
