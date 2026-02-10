# c-ares External Project (static; required by gRPC when using package provider)
ExternalProject_Add(
    cares_external
    PREFIX ${CMAKE_BINARY_DIR}/external/c-ares
    URL https://github.com/c-ares/c-ares/releases/download/v${CARES_VERSION}/c-ares-${CARES_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS ON
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CARES_PREFIX}
        -DCMAKE_BUILD_TYPE=Release
        -DCARES_STATIC=ON
        -DCARES_SHARED=OFF
        -DCARES_INSTALL=ON
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
)
