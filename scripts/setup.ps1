# Fail if any unset variable is used
Set-StrictMode -Version Latest

Import-Module "$PSScriptRoot/functions.psm1"

ValidateVariableOrSetDefault "REPOSITORY_DIR" -Default (Split-Path $PSScriptRoot)
ValidateVariableOrSetDefault "DEPENDENCIES" -Default "$REPOSITORY_DIR/thirdparty"
ValidateVariableOrSetDefault "COMPILER_PLATFORM" -Default "vs2022"
ValidateVariableOrSetDefault "PREMAKE5_URL" -Default "https://github.com/premake/premake-core/releases/download/v5.0.0-beta1/premake-5.0.0-beta1-windows.zip"
ValidateVariableOrSetDefault "PREMAKE5" -Default "$DEPENDENCIES/windows/premake-core/premake5.exe"
ValidateVariableOrSetDefault "PROJECT_OS" -Default "windows"

CreateDirectoryForcefully $DEPENDENCIES
