# gRPC External Project (release tarball has no third_party submodules; use package for cares/re2/zlib)
ExternalProject_Add(
    grpc_external
    PREFIX ${CMAKE_BINARY_DIR}/external/grpc
    URL https://github.com/grpc/grpc/archive/refs/tags/v${GRPC_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS ON
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${GRPC_PREFIX}
        -DCMAKE_BUILD_TYPE=Release
        -DBUILD_SHARED_LIBS=OFF
        -DgRPC_INSTALL=ON
        -DgRPC_BUILD_TESTS=OFF
        -DgRPC_BUILD_CSHARP_EXT=OFF
        -DgRPC_BUILD_GRPC_CSHARP_PLUGIN=OFF
        -DgRPC_ABSL_PROVIDER=package
        -DgRPC_PROTOBUF_PROVIDER=package
        -DgRPC_SSL_PROVIDER=package
        -DgRPC_CARES_PROVIDER=package
        -DgRPC_RE2_PROVIDER=package
        -DgRPC_ZLIB_PROVIDER=package
        -Dabsl_DIR=${ABSEIL_PREFIX}/lib/cmake/absl
        -DProtobuf_DIR=${PROTOBUF_PREFIX}/lib/cmake/protobuf
        -Dutf8_range_DIR=${PROTOBUF_PREFIX}/lib/cmake/utf8_range
        -Dc-ares_DIR=${CARES_PREFIX}/lib/cmake/c-ares
        -Dre2_DIR=${RE2_PREFIX}/lib/cmake/re2
        -DOPENSSL_ROOT_DIR=${OPENSSL_PREFIX}
        -DOPENSSL_USE_STATIC_LIBS=ON
        -DCMAKE_CXX_STANDARD=17
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
)

if(Cares_NEEDS_INSTALL)
    add_dependencies(grpc_external cares_external)
endif()
if(Re2_NEEDS_INSTALL)
    add_dependencies(grpc_external re2_external)
endif()
if(Abseil_NEEDS_INSTALL)
    add_dependencies(grpc_external abseil_external)
endif()
if(Protobuf_NEEDS_INSTALL)
    add_dependencies(grpc_external protobuf_external)
endif()
if(OpenSSL_NEEDS_INSTALL)
    add_dependencies(grpc_external openssl_external)
endif()
