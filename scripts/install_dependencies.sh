#!/bin/bash

# Exit if any command fails and if any unset variable is used
set -eu

DIR="$( cd "$( dirname "${BASH_SOURCE:-$0}" )" && pwd )"

. "${DIR}/functions.sh"
. "${DIR}/setup.sh"

if [ ! -f "$PREMAKE5" ]; then
	echo "Installing premake5..."
	PREMAKE_DIRECTORY="$DEPENDENCIES/$PROJECT_OS/premake-core"
	PREMAKE_TAR_PATH="$PREMAKE_DIRECTORY/premake-core.tar.gz"
	create_directory_forcefully "$PREMAKE_DIRECTORY"
	curl -s -L "$PREMAKE5_URL" -o "$PREMAKE_TAR_PATH"
	tar -xf "$PREMAKE_TAR_PATH" -C "$PREMAKE_DIRECTORY"
	rm -f "$PREMAKE_TAR_PATH"
fi

case "$(uname -s)" in
    Linux*)
		sudo apt update
		sudo apt install -y libavcodec-dev libavdevice-dev libavfilter-dev libavformat-dev libavutil-dev libpostproc-dev libswresample-dev libswscale-dev libopenal-dev libgl1-mesa-dev libglu1-mesa-dev
        ;;
    Darwin*)
		brew update
		brew install ffmpeg openal-soft xquartz
        ;;
    *)
        echo "Unknown operating system"
        exit 1
        ;;
esac
