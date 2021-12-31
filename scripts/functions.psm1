# Fail if any unset variable is used
Set-StrictMode -Version Latest

Write-Host "Installing and importing VSSetup module..."
Install-Module VSSetup -Scope CurrentUser -Force -ErrorAction Stop
Import-Module VSSetup -ErrorAction Stop

function ValidateVariableOrSetDefault([string]$Name, $Default = $null) {
	$value = $Default
	if (Test-Path env:$Name) {
		$value = (Get-Item env:$Name).Value
	}

	if (Test-Path variable:$Name) {
		$value = Get-Variable -ValueOnly $Name
	}

	if (($null -eq $value) -and ($null -eq $Default)) {
		throw "'$Name' was not set"
	}

	Set-Variable $Name -Value $value -Scope Global -ErrorAction Stop -Confirm:$false
}

function Invoke-Call([scriptblock]$ScriptBlock, [string]$ErrorAction = $ErrorActionPreference) {
	& @ScriptBlock
	if (($LastExitCode -ne 0) -and ($ErrorAction -eq "Stop")) {
		exit $LastExitCode
	}
}

function CreateDirectoryForcefully([string]$Path) {
	$item = Get-Item $Path -ErrorAction SilentlyContinue
	if ($item -is [System.IO.DirectoryInfo]) {
		return
	}

	if ($item) {
		Remove-Item $Path -Force -ErrorAction Stop -Confirm:$false
	}

	New-Item $Path -ItemType Directory -ErrorAction Stop -Confirm:$false | Out-Null
}

function GetMSBuildPath() {
	$Instance = Get-VSSetupInstance -All -ErrorAction Stop |
		Select-VSSetupInstance -Require 'Microsoft.VisualStudio.Workload.NativeDesktop' -Latest -ErrorAction Stop |
		Select-Object -First 1 -ErrorAction Stop
	if (!$Instance) {
		throw "Unable to retrieve one instance of Visual Studio"
	}

	$Path = $Instance.InstallationPath
	$MSBuild = "$Path/MSBuild/Current/Bin/MSBuild.exe"
	if (![System.IO.File]::Exists($MSBuild)) {
		$MSBuild = "$Path/MSBuild/15.0/Bin/MSBuild.exe"
		if (![System.IO.File]::Exists($MSBuild)) {
			throw "Unable to retrieve path to MSBuild"
		}
	}

	return $MSBuild
}

Set-Variable MSBuild (GetMSBuildPath) -ErrorAction Stop -Confirm:$false

Export-ModuleMember -Function ValidateVariableOrSetDefault, CreateDirectoryForcefully, Invoke-Call, GetMSBuildPath -Variable MSBuild
