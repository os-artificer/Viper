# spdlog External Project
ExternalProject_Add(
    spdlog_external
    PREFIX ${CMAKE_BINARY_DIR}/external/spdlog
    URL https://github.com/gabime/spdlog/archive/refs/tags/v${SPDLOG_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS ON
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${SPDLOG_PREFIX}
        -DCMAKE_BUILD_TYPE=Release
        -DBUILD_SHARED_LIBS=OFF
        -DSPDLOG_BUILD_EXAMPLE=OFF
        -DSPDLOG_BUILD_TESTS=OFF
        -DSPDLOG_BUILD_BENCH=OFF
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
)
