# OpenSSL External Project
ExternalProject_Add(
    openssl_external
    PREFIX ${CMAKE_BINARY_DIR}/external/openssl
    URL https://github.com/openssl/openssl/archive/refs/tags/openssl-${OPENSSL_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS ON
    CONFIGURE_COMMAND <SOURCE_DIR>/config
        --prefix=${OPENSSL_PREFIX}
        --openssldir=${OPENSSL_PREFIX}
        no-shared
    BUILD_COMMAND sh -c "make depend && make -j${CMAKE_BUILD_PARALLEL_LEVEL}"
    INSTALL_COMMAND make install
    BUILD_IN_SOURCE 1
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
)
