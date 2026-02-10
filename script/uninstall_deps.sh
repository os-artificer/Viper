#!/bin/bash
# Uninstall Viper project's third-party dependencies from /usr/local (or DEPS_INSTALL_PREFIX).
# Run: ./script/uninstall_deps.sh [prefix]
# Default prefix: /usr/local

set -e
PREFIX="${1:-/usr/local}"

echo "Removing Viper deps from ${PREFIX}..."

# Version suffixes must match cmake/Dependencies.cmake
dirs=(
    "openssl-3.2.1"
    "boost-1.87.0"
    "libevent-2.1.12"
    "curl-8.11.1"
    "yaml-cpp-0.8.0"
    "spdlog-1.13.0"
    "rapidjson"
    "concurrentqueue-1.0.4"
    "protobuf-30.2"
    "grpc-1.71.0"
    "c-ares-1.34.2"
    "re2-2024-06-01"
    "abseil/20250127.1"
    "etcd-cppapiv3"
    "cpprest-2.10.19"
    "zstd-1.5.6"
)

for d in "${dirs[@]}"; do
    path="${PREFIX}/${d}"
    if [[ -d "$path" ]]; then
        echo "  rm -rf $path"
        rm -rf "$path"
    fi
done

echo "Done."
