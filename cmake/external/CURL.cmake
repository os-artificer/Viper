# CURL External Project
# When BUILD_DEPS_BOTH_STATIC_SHARED=ON: build static first, then shared
set(CURL_CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${CURL_PREFIX}
    -DCMAKE_BUILD_TYPE=Release
    -DBUILD_SHARED_LIBS=OFF
    -DBUILD_CURL_EXE=OFF
    -DBUILD_TESTING=OFF
    -DCURL_USE_OPENSSL=ON
    -DOPENSSL_ROOT_DIR=${OPENSSL_PREFIX}
)
if(BUILD_DEPS_BOTH_STATIC_SHARED)
    set(_curl_shared_dir ${CMAKE_BINARY_DIR}/external/curl/build/curl_shared)
    list(JOIN CURL_CMAKE_ARGS " " _curl_cmake_args_str)
    set(_curl_install_str "${CMAKE_COMMAND} --install . && ${CMAKE_COMMAND} ${_curl_cmake_args_str} -DBUILD_SHARED_LIBS=ON -B ${_curl_shared_dir} -S <SOURCE_DIR> && ${CMAKE_COMMAND} --build ${_curl_shared_dir} --parallel ${CMAKE_BUILD_PARALLEL_LEVEL} && ${CMAKE_COMMAND} --install ${_curl_shared_dir}")
    set(CURL_INSTALL_CMD sh -c "${_curl_install_str}")
else()
    set(CURL_INSTALL_CMD ${CMAKE_COMMAND} --install .)
endif()
ExternalProject_Add(
    curl_external
    PREFIX ${CMAKE_BINARY_DIR}/external/curl
    URL https://github.com/curl/curl/archive/refs/tags/curl-${CURL_TAG}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS ON
    CMAKE_ARGS ${CURL_CMAKE_ARGS}
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
    INSTALL_COMMAND ${CURL_INSTALL_CMD}
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
)

if(OpenSSL_NEEDS_INSTALL)
    add_dependencies(curl_external openssl_external)
endif()
