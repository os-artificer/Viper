# BuildBothStaticShared.cmake
# Helper to build both static and shared libs for CMake-based ExternalProjects.
# Usage: include() and call build_both_static_shared() with appropriate args.
#
# build_both_static_shared(
#   SOURCE_DIR <dir>
#   BINARY_DIR <dir>
#   INSTALL_PREFIX <prefix>
#   CMAKE_ARGS_BASE <args...>  # Base CMAKE_ARGS (without BUILD_SHARED_LIBS)
# )

function(build_both_static_shared)
    cmake_parse_arguments(ARG "" "SOURCE_DIR;BINARY_DIR;INSTALL_PREFIX" "CMAKE_ARGS_BASE" ${ARGN})
    if(NOT ARG_SOURCE_DIR OR NOT ARG_BINARY_DIR OR NOT ARG_INSTALL_PREFIX)
        message(FATAL_ERROR "build_both_static_shared requires SOURCE_DIR, BINARY_DIR, INSTALL_PREFIX")
    endif()

    set(_build_dir "${ARG_BINARY_DIR}/build_both")
    file(MAKE_DIRECTORY "${_build_dir}")

    # First pass: static libs (BUILD_SHARED_LIBS=OFF)
    message(STATUS "Configuring for static build...")
    execute_process(
        COMMAND ${CMAKE_COMMAND} ${ARG_CMAKE_ARGS_BASE}
            -DCMAKE_INSTALL_PREFIX=${ARG_INSTALL_PREFIX}
            -DBUILD_SHARED_LIBS=OFF
            -B "${_build_dir}" -S "${ARG_SOURCE_DIR}"
        RESULT_VARIABLE _result
    )
    if(_result)
        message(FATAL_ERROR "Static configure failed: ${_result}")
    endif()

    message(STATUS "Building static libs...")
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build "${_build_dir}" --parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
        RESULT_VARIABLE _result
    )
    if(_result)
        message(FATAL_ERROR "Static build failed: ${_result}")
    endif()

    message(STATUS "Installing static libs...")
    execute_process(
        COMMAND ${CMAKE_COMMAND} --install "${_build_dir}"
        RESULT_VARIABLE _result
    )
    if(_result)
        message(FATAL_ERROR "Static install failed: ${_result}")
    endif()

    # Second pass: shared libs (BUILD_SHARED_LIBS=ON), reconfig in same dir
    message(STATUS "Reconfiguring for shared build...")
    execute_process(
        COMMAND ${CMAKE_COMMAND} ${ARG_CMAKE_ARGS_BASE}
            -DCMAKE_INSTALL_PREFIX=${ARG_INSTALL_PREFIX}
            -DBUILD_SHARED_LIBS=ON
            -B "${_build_dir}" -S "${ARG_SOURCE_DIR}"
        RESULT_VARIABLE _result
    )
    if(_result)
        message(FATAL_ERROR "Shared configure failed: ${_result}")
    endif()

    message(STATUS "Building shared libs...")
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build "${_build_dir}" --parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
        RESULT_VARIABLE _result
    )
    if(_result)
        message(FATAL_ERROR "Shared build failed: ${_result}")
    endif()

    message(STATUS "Installing shared libs...")
    execute_process(
        COMMAND ${CMAKE_COMMAND} --install "${_build_dir}"
        RESULT_VARIABLE _result
    )
    if(_result)
        message(FATAL_ERROR "Shared install failed: ${_result}")
    endif()
endfunction()
