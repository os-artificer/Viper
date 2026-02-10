# re2 External Project (static; required by gRPC when using package provider)
ExternalProject_Add(
    re2_external
    PREFIX ${CMAKE_BINARY_DIR}/external/re2
    URL https://github.com/google/re2/archive/refs/tags/${RE2_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS ON
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${RE2_PREFIX}
        -DCMAKE_BUILD_TYPE=Release
        -DBUILD_SHARED_LIBS=OFF
        -DRE2_BUILD_TESTING=OFF
        -Dabsl_DIR=${ABSEIL_PREFIX}/lib/cmake/absl
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
)
if(Abseil_NEEDS_INSTALL)
    add_dependencies(re2_external abseil_external)
endif()
