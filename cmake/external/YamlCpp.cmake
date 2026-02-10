# yaml-cpp External Project
# When BUILD_DEPS_BOTH_STATIC_SHARED=ON: build static first, then shared
set(YAMLCPP_CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${YAML_CPP_PREFIX}
    -DCMAKE_BUILD_TYPE=Release
    -DBUILD_SHARED_LIBS=OFF
    -DYAML_CPP_BUILD_TESTS=OFF
    -DYAML_CPP_BUILD_TOOLS=OFF
    -DYAML_BUILD_SHARED_LIBS=OFF
)
if(BUILD_DEPS_BOTH_STATIC_SHARED)
    set(_yamlcpp_shared_dir ${CMAKE_BINARY_DIR}/external/yaml-cpp/build/yamlcpp_shared)
    list(JOIN YAMLCPP_CMAKE_ARGS " " _yamlcpp_cmake_args_str)
    set(_yamlcpp_install_str "${CMAKE_COMMAND} --install . && ${CMAKE_COMMAND} ${_yamlcpp_cmake_args_str} -DBUILD_SHARED_LIBS=ON -B ${_yamlcpp_shared_dir} -S <SOURCE_DIR> && ${CMAKE_COMMAND} --build ${_yamlcpp_shared_dir} --parallel ${CMAKE_BUILD_PARALLEL_LEVEL} && ${CMAKE_COMMAND} --install ${_yamlcpp_shared_dir}")
    set(YAMLCPP_INSTALL_CMD sh -c "${_yamlcpp_install_str}")
else()
    set(YAMLCPP_INSTALL_CMD ${CMAKE_COMMAND} --install .)
endif()
ExternalProject_Add(
    yaml-cpp_external
    PREFIX ${CMAKE_BINARY_DIR}/external/yaml-cpp
    URL https://github.com/jbeder/yaml-cpp/archive/refs/tags/${YAML_CPP_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS ON
    CMAKE_ARGS ${YAMLCPP_CMAKE_ARGS}
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
    INSTALL_COMMAND ${YAMLCPP_INSTALL_CMD}
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
)
