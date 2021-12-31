#!/bin/bash

# Exit if any command fails and if any unset variable is used
set -eu

function validate_variable_or_set_default {
	local NAME="$1"
	local DEFAULT="${2:-}"

	local VALUE="$DEFAULT"
	if [ "${!NAME:-}" ]; then
		local VALUE="${!NAME}"
	fi

	if [ -z "$VALUE" ] && [ -z "$DEFAULT" ]; then
		echo "'$NAME' was not set"
		exit 1
	fi

	export "$NAME"="$VALUE"
}

function create_directory_forcefully {
	local DIRECTORY="$1"

	if [ -d "$DIRECTORY" ]; then
		return
	elif [ -f "$DIRECTORY" ]; then
		rm -f "$DIRECTORY"
	fi

	mkdir -p "$DIRECTORY"
}
