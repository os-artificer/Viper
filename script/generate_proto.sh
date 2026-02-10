#!/bin/bash
# Generate protobuf C++ files from .proto in src/internal/

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
PROTO_DIR="${PROJECT_ROOT}/src/internal"
cd "${PROTO_DIR}"

protoc -I=. --cpp_out=. *.proto
