# RapidJSON External Project (header-only)
# Patch: remove GenericStringRef::operator= (assigns to const members; invalid in C++17)
ExternalProject_Add(
    rapidjson_external
    PREFIX ${CMAKE_BINARY_DIR}/external/rapidjson
    URL https://github.com/Tencent/rapidjson/archive/refs/tags/v${RAPIDJSON_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS ON
    PATCH_COMMAND sed -i "/GenericStringRef& operator=(const GenericStringRef& rhs)/d" <SOURCE_DIR>/include/rapidjson/document.h
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${RAPIDJSON_PREFIX}
        -DRAPIDJSON_BUILD_DOC=OFF
        -DRAPIDJSON_BUILD_EXAMPLES=OFF
        -DRAPIDJSON_BUILD_TESTS=OFF
    BUILD_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_INSTALL ON
)
