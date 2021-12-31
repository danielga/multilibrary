# Fail if any unset variable is used
Set-StrictMode -Version Latest

Import-Module "$PSScriptRoot/functions.psm1"

& "$PSScriptRoot/setup.ps1"

Add-Type -AssemblyName System.IO.Compression.FileSystem

if (!((Get-Item $PREMAKE5 -ErrorAction SilentlyContinue) -is [System.IO.FileInfo])) {
	Write-Output "Installing premake5..."
	$PremakeDirectory = "$DEPENDENCIES/$PROJECT_OS/premake-core"
	$PremakeZipPath = "$PremakeDirectory/premake-core.zip"
	CreateDirectoryForcefully $PremakeDirectory
	(New-Object System.Net.WebClient).DownloadFile("$PREMAKE5_URL", $PremakeZipPath)
	[System.IO.Compression.ZipFile]::ExtractToDirectory($PremakeZipPath, $PremakeDirectory)
	Remove-Item $PremakeZipPath -Force -ErrorAction Stop -Confirm:$false
}

CreateDirectoryForcefully "$DEPENDENCIES/$PROJECT_OS/bin/x86"
CreateDirectoryForcefully "$DEPENDENCIES/$PROJECT_OS/bin/x64"
CreateDirectoryForcefully "$DEPENDENCIES/$PROJECT_OS/include"
CreateDirectoryForcefully "$DEPENDENCIES/$PROJECT_OS/lib/x86"
CreateDirectoryForcefully "$DEPENDENCIES/$PROJECT_OS/lib/x64"

if (!((Get-Item "$DEPENDENCIES/$PROJECT_OS/bin/x64/avcodec-58.dll" -ErrorAction SilentlyContinue) -is [System.IO.FileInfo])) {
	Write-Output "Installing ffmpeg..."
	$FFMPEGDirectory = "$DEPENDENCIES/$PROJECT_OS/ffmpeg"
	$FFMPEGZipPath = "$FFMPEGDirectory/ffmpeg-4.4.1-full_build-shared.7z"
	CreateDirectoryForcefully $FFMPEGDirectory
	(New-Object System.Net.WebClient).DownloadFile("https://www.gyan.dev/ffmpeg/builds/packages/ffmpeg-4.4.1-full_build-shared.7z", $FFMPEGZipPath)
	Invoke-Call { & "C:/Program Files/7-Zip/7z.exe" x -o"$FFMPEGDirectory" "$FFMPEGZipPath" } -ErrorAction Stop
	Remove-Item $FFMPEGZipPath -Force -ErrorAction Stop -Confirm:$false

	Copy-Item -Path "$FFMPEGDirectory/ffmpeg-4.4.1-full_build-shared/bin/*" -Destination "$DEPENDENCIES/$PROJECT_OS/bin/x64/" -Force -ErrorAction Stop -Confirm:$false
	Copy-Item -Path "$FFMPEGDirectory/ffmpeg-4.4.1-full_build-shared/include/*" -Destination "$DEPENDENCIES/$PROJECT_OS/include/" -Recurse -Force -ErrorAction Stop -Confirm:$false
	Copy-Item -Path "$FFMPEGDirectory/ffmpeg-4.4.1-full_build-shared/lib/*" -Destination "$DEPENDENCIES/$PROJECT_OS/lib/x64/" -Force -ErrorAction Stop -Confirm:$false

	Remove-Item $FFMPEGDirectory -Force -Recurse -ErrorAction Stop -Confirm:$false
}

if (!((Get-Item "$DEPENDENCIES/$PROJECT_OS/bin/x64/soft_oal.dll" -ErrorAction SilentlyContinue) -is [System.IO.FileInfo])) {
	Write-Output "Installing openal-soft..."
	$OpenALSoftDirectory = "$DEPENDENCIES/$PROJECT_OS/openal-soft"
	$OpenALSoftZipPath = "$OpenALSoftDirectory/openal-soft-1.21.1-bin.zip"
	CreateDirectoryForcefully $OpenALSoftDirectory
	(New-Object System.Net.WebClient).DownloadFile("https://openal-soft.org/openal-binaries/openal-soft-1.21.1-bin.zip", $OpenALSoftZipPath)
	[System.IO.Compression.ZipFile]::ExtractToDirectory($OpenALSoftZipPath, $OpenALSoftDirectory)
	Remove-Item $OpenALSoftZipPath -Force -ErrorAction Stop -Confirm:$false

	Copy-Item -Path "$OpenALSoftDirectory/openal-soft-1.21.1-bin/bin/Win32/*" -Destination "$DEPENDENCIES/$PROJECT_OS/bin/x86/" -Force -ErrorAction Stop -Confirm:$false
	Copy-Item -Path "$OpenALSoftDirectory/openal-soft-1.21.1-bin/bin/Win64/*" -Destination "$DEPENDENCIES/$PROJECT_OS/bin/x64/" -Force -ErrorAction Stop -Confirm:$false
	Copy-Item -Path "$OpenALSoftDirectory/openal-soft-1.21.1-bin/include/*" -Destination "$DEPENDENCIES/$PROJECT_OS/include/" -Recurse -Force -ErrorAction Stop -Confirm:$false
	Copy-Item -Path "$OpenALSoftDirectory/openal-soft-1.21.1-bin/libs/Win32/*" -Destination "$DEPENDENCIES/$PROJECT_OS/lib/x86/" -Force -ErrorAction Stop -Confirm:$false
	Copy-Item -Path "$OpenALSoftDirectory/openal-soft-1.21.1-bin/libs/Win64/*" -Destination "$DEPENDENCIES/$PROJECT_OS/lib/x64/" -Force -ErrorAction Stop -Confirm:$false

	Remove-Item $OpenALSoftDirectory -Force -Recurse -ErrorAction Stop -Confirm:$false
}
