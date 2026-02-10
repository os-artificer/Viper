# concurrentqueue External Project (header-only)
ExternalProject_Add(
    concurrentqueue_external
    PREFIX ${CMAKE_BINARY_DIR}/external/concurrentqueue
    URL https://github.com/cameron314/concurrentqueue/archive/refs/tags/v${CONCURRENTQUEUE_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS ON
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND
        ${CMAKE_COMMAND} -E make_directory ${CONCURRENTQUEUE_PREFIX}/include/concurrentqueue
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR> ${CONCURRENTQUEUE_PREFIX}/include/concurrentqueue
    LOG_DOWNLOAD ON
    LOG_INSTALL ON
)
