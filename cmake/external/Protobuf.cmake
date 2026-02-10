# Protobuf External Project
ExternalProject_Add(
    protobuf_external
    PREFIX ${CMAKE_BINARY_DIR}/external/protobuf
    URL https://github.com/protocolbuffers/protobuf/archive/refs/tags/v${PROTOBUF_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS ON
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${PROTOBUF_PREFIX}
        -DCMAKE_BUILD_TYPE=Release
        -DBUILD_SHARED_LIBS=OFF
        -Dprotobuf_BUILD_TESTS=OFF
        -Dprotobuf_BUILD_EXAMPLES=OFF
        -Dprotobuf_WITH_ZLIB=OFF
        -Dprotobuf_ABSL_PROVIDER=package
        -Dabsl_DIR=${ABSEIL_PREFIX}/lib/cmake/absl
        -DCMAKE_CXX_STANDARD=17
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
)

if(Abseil_NEEDS_INSTALL)
    add_dependencies(protobuf_external abseil_external)
endif()
