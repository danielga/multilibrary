function include_vcpkg()
	local vcpkg_root = os.getenv("VCPKG_ROOT")
	if not vcpkg_root then
		vcpkg_root = assert(
			os.pathsearch(".vcpkg-root", os.getenv("PATH")),
			"Unable to find vcpkg root directory"
		)
	end

	assert(
		os.isfile(path.join(vcpkg_root, ".vcpkg-root")),
		"Possible vcpkg root directory \"" .. vcpkg_root .. "\" is not valid (does not contain .vcpkg-root file)"
	)

	-- Windows: 'windows', 'x86' or 'x64'
	-- Linux: 'linux', 'x64'
	-- MacOSX: 'osx', 'x64'
	sysincludedirs(path.join(vcpkg_root, "installed/%{cfg.platform}-%{cfg.system:gsub('mac', '')}/include"))
	syslibdirs(path.join(vcpkg_root, "installed/%{cfg.platform}-%{cfg.system:gsub('mac', '')}/%{cfg.buildcfg:find('[Dd]ebug') and 'debug/lib' or 'lib'}"))
end
