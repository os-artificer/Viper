# Zstd External Project
set(ZSTD_VERSION "1.5.6")
set(ZSTD_PREFIX "${DEPS_INSTALL_PREFIX}/zstd-${ZSTD_VERSION}")

ExternalProject_Add(
    zstd_external
    PREFIX ${CMAKE_BINARY_DIR}/external/zstd
    URL https://github.com/facebook/zstd/releases/download/v${ZSTD_VERSION}/zstd-${ZSTD_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS ON
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make -C <SOURCE_DIR>/lib lib-release -j${CMAKE_BUILD_PARALLEL_LEVEL}
    INSTALL_COMMAND make -C <SOURCE_DIR>/lib install PREFIX=${ZSTD_PREFIX}
    LOG_DOWNLOAD ON
    LOG_BUILD ON
    LOG_INSTALL ON
)
