# Libevent External Project
# When BUILD_DEPS_BOTH_STATIC_SHARED=ON: build static first (default), then shared and install both
set(LIBEVENT_CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${LIBEVENT_PREFIX}
    -DCMAKE_BUILD_TYPE=Release
    -DBUILD_SHARED_LIBS=OFF
    -DEVENT__DISABLE_TESTS=ON
    -DEVENT__DISABLE_SAMPLES=ON
    -DEVENT__DISABLE_BENCHMARK=ON
)
if(BUILD_DEPS_BOTH_STATIC_SHARED)
    set(_libevent_shared_dir ${CMAKE_BINARY_DIR}/external/libevent/build/libevent_shared)
    list(JOIN LIBEVENT_CMAKE_ARGS " " _libevent_cmake_args_str)
    set(_libevent_install_str "${CMAKE_COMMAND} --install . && ${CMAKE_COMMAND} ${_libevent_cmake_args_str} -DBUILD_SHARED_LIBS=ON -B ${_libevent_shared_dir} -S <SOURCE_DIR> && ${CMAKE_COMMAND} --build ${_libevent_shared_dir} --parallel ${CMAKE_BUILD_PARALLEL_LEVEL} && ${CMAKE_COMMAND} --install ${_libevent_shared_dir}")
    set(LIBEVENT_INSTALL_CMD sh -c "${_libevent_install_str}")
else()
    set(LIBEVENT_INSTALL_CMD ${CMAKE_COMMAND} --install .)
endif()
ExternalProject_Add(
    libevent_external
    PREFIX ${CMAKE_BINARY_DIR}/external/libevent
    URL https://github.com/libevent/libevent/archive/refs/tags/release-${LIBEVENT_VERSION}-stable.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS ON
    CMAKE_ARGS ${LIBEVENT_CMAKE_ARGS}
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
    INSTALL_COMMAND ${LIBEVENT_INSTALL_CMD}
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
)
