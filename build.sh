#!/bin/bash
# Viper Project - build script thin wrapper (delegates to script/build.sh)
exec "$(dirname "$0")/script/build.sh" "$@"
