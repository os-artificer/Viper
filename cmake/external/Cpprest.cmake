# cpprestsdk External Project
ExternalProject_Add(
    cpprest_external
    PREFIX ${CMAKE_BINARY_DIR}/external/cpprest
    URL https://github.com/microsoft/cpprestsdk/archive/refs/tags/v${CPPREST_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS ON
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CPPREST_PREFIX}
        -DCMAKE_BUILD_TYPE=Release
        -DBUILD_SHARED_LIBS=OFF
        -DCPPREST_EXCLUDE_WEBSOCKETS=OFF
        -DCPPREST_EXCLUDE_COMPRESSION=OFF
        -DCPPREST_EXCLUDE_BROTLI=ON
        -DBUILD_TESTS=OFF
        -DBUILD_SAMPLES=OFF
        -DCMAKE_CXX_STANDARD=17
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        "-DCMAKE_CXX_FLAGS=-Wno-error=format-truncation -Wno-error"
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
)

# cpprestsdk depends on OpenSSL and Boost
if(OpenSSL_NEEDS_INSTALL)
    add_dependencies(cpprest_external openssl_external)
endif()
if(Boost_NEEDS_INSTALL)
    add_dependencies(cpprest_external boost_external)
endif()
