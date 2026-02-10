# Boost External Project
ExternalProject_Add(
    boost_external
    PREFIX ${CMAKE_BINARY_DIR}/external/boost
    URL https://github.com/boostorg/boost/releases/download/boost-${BOOST_VERSION}/boost-${BOOST_VERSION}-b2-nodocs.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS ON
    CONFIGURE_COMMAND <SOURCE_DIR>/bootstrap.sh
        --with-libraries=all
        --prefix=${BOOST_PREFIX}
    BUILD_COMMAND <SOURCE_DIR>/b2
        --address-model=64
        link=static
        runtime-link=static
        variant=release
        threading=multi
        -j${CMAKE_BUILD_PARALLEL_LEVEL}
    INSTALL_COMMAND <SOURCE_DIR>/b2 install
    BUILD_IN_SOURCE 1
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
)
