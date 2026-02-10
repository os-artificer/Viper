# etcd-cpp-apiv3 External Project
ExternalProject_Add(
    etcd-cpp_external
    PREFIX ${CMAKE_BINARY_DIR}/external/etcd-cpp
    GIT_REPOSITORY https://github.com/etcd-cpp-apiv3/etcd-cpp-apiv3.git
    GIT_TAG master
    GIT_SHALLOW ON
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${ETCD_CPP_PREFIX}
        -DCMAKE_BUILD_TYPE=Release
        -DBUILD_SHARED_LIBS=OFF
        -DBUILD_ETCD_TESTS=OFF
        -DgRPC_DIR=${GRPC_PREFIX}/lib/cmake/grpc
        -DProtobuf_DIR=${PROTOBUF_PREFIX}/lib/cmake/protobuf
        -Dabsl_DIR=${ABSEIL_PREFIX}/lib/cmake/absl
        -Dutf8_range_DIR=${PROTOBUF_PREFIX}/lib/cmake/utf8_range
        -DOPENSSL_ROOT_DIR=${OPENSSL_PREFIX}
        -DCMAKE_CXX_STANDARD=17
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
)

if(gRPC_NEEDS_INSTALL)
    add_dependencies(etcd-cpp_external grpc_external)
endif()
if(Protobuf_NEEDS_INSTALL)
    add_dependencies(etcd-cpp_external protobuf_external)
endif()
if(Abseil_NEEDS_INSTALL)
    add_dependencies(etcd-cpp_external abseil_external)
endif()
if(OpenSSL_NEEDS_INSTALL)
    add_dependencies(etcd-cpp_external openssl_external)
endif()
