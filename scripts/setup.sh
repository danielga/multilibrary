#!/bin/bash

# Exit if any command fails and if any unset variable is used
set -eu

DIR="$( cd "$( dirname "${BASH_SOURCE:-$0}" )" && pwd )"

. "${DIR}/functions.sh"

validate_variable_or_set_default "REPOSITORY_DIR" "$(cd "${DIR}/.." && pwd)"
validate_variable_or_set_default "DEPENDENCIES" "$REPOSITORY_DIR/thirdparty"
validate_variable_or_set_default "COMPILER_PLATFORM" "gmake"

case "$(uname -s)" in
    Linux*)
        TARGET="linux"
        validate_variable_or_set_default "PREMAKE5_URL" "https://github.com/premake/premake-core/releases/download/v5.0.0-beta1/premake-5.0.0-beta1-linux.tar.gz"
        validate_variable_or_set_default "PROJECT_OS" "linux"
        ;;
    Darwin*)
        TARGET="osx"
        validate_variable_or_set_default "PREMAKE5_URL" "https://github.com/premake/premake-core/releases/download/v5.0.0-beta1/premake-5.0.0-beta1-macosx.tar.gz"
        validate_variable_or_set_default "PROJECT_OS" "macosx"
        ;;
    *)
        echo "Unknown operating system"
        exit 1
        ;;
esac

validate_variable_or_set_default "PREMAKE5" "$DEPENDENCIES/$PROJECT_OS/premake-core/premake5"

create_directory_forcefully "$DEPENDENCIES"
