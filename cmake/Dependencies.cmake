# Dependencies.cmake - Viper Project Dependency Management
# Automatically detect and install all dependencies

include(ExternalProject)

# Set default install prefix for dependencies
set(DEPS_INSTALL_PREFIX "/usr/local" CACHE PATH "Dependencies install prefix")

# Use static linking globally for built libs; when finding libs prefer static when both .a and .so exist
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
# Search .a first, then .so, so static is chosen when both exist; still find .so when .a is missing
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.so;.so.1;.so.1.0" CACHE STRING "Library suffixes: prefer static" FORCE)

# Dependency version definitions
set(OPENSSL_VERSION "3.2.1")
set(BOOST_VERSION "1.87.0")
set(LIBEVENT_VERSION "2.1.12")
set(HIREDIS_VERSION "1.2.0")
set(REDIS_PLUS_PLUS_VERSION "1.3.13")
set(CONCURRENTQUEUE_VERSION "1.0.4")
set(SPDLOG_VERSION "1.13.0")
set(CATCH2_VERSION "3.5.3")
set(CPPREST_VERSION "2.10.19")
set(ABSEIL_VERSION "20250127.1")
set(PROTOBUF_VERSION "30.2")
set(GRPC_VERSION "1.71.0")
set(CARES_VERSION "1.34.2")
set(RE2_VERSION "2024-06-01")
set(ETCD_CPP_VERSION "0.2.4")
set(MONGO_CXX_VERSION "r3.10.1")
set(RAPIDJSON_VERSION "1.1.0")
set(YAML_CPP_VERSION "0.8.0")
set(CURL_VERSION "8.11.1")
set(CURL_TAG "8_11_1")  # GitHub tag uses underscores
set(ZSTD_VERSION "1.5.6")
set(PROMETHEUS_CPP_VERSION "1.2.4")

# Dependency installation paths
set(OPENSSL_PREFIX "${DEPS_INSTALL_PREFIX}/openssl-${OPENSSL_VERSION}")
set(BOOST_PREFIX "${DEPS_INSTALL_PREFIX}/boost-${BOOST_VERSION}")
set(LIBEVENT_PREFIX "${DEPS_INSTALL_PREFIX}/libevent-${LIBEVENT_VERSION}")
set(HIREDIS_PREFIX "${DEPS_INSTALL_PREFIX}/hiredis-${HIREDIS_VERSION}")
set(REDIS_PLUS_PLUS_PREFIX "${DEPS_INSTALL_PREFIX}/redis-plus-plus-${REDIS_PLUS_PLUS_VERSION}")
set(CONCURRENTQUEUE_PREFIX "${DEPS_INSTALL_PREFIX}/concurrentqueue-${CONCURRENTQUEUE_VERSION}")
set(SPDLOG_PREFIX "${DEPS_INSTALL_PREFIX}/spdlog-${SPDLOG_VERSION}")
set(CATCH2_PREFIX "${DEPS_INSTALL_PREFIX}/catch2-${CATCH2_VERSION}")
set(CPPREST_PREFIX "${DEPS_INSTALL_PREFIX}/cpprest-${CPPREST_VERSION}")
set(ABSEIL_PREFIX "${DEPS_INSTALL_PREFIX}/abseil/${ABSEIL_VERSION}")
set(PROTOBUF_PREFIX "${DEPS_INSTALL_PREFIX}/protobuf-${PROTOBUF_VERSION}")
set(GRPC_PREFIX "${DEPS_INSTALL_PREFIX}/grpc-${GRPC_VERSION}")
set(CARES_PREFIX "${DEPS_INSTALL_PREFIX}/c-ares-${CARES_VERSION}")
set(RE2_PREFIX "${DEPS_INSTALL_PREFIX}/re2-${RE2_VERSION}")
set(ETCD_CPP_PREFIX "${DEPS_INSTALL_PREFIX}/etcd-cppapiv3")
set(MONGO_CXX_PREFIX "${DEPS_INSTALL_PREFIX}/mongo-cxx-driver-${MONGO_CXX_VERSION}")
set(RAPIDJSON_PREFIX "${DEPS_INSTALL_PREFIX}/rapidjson")
set(YAML_CPP_PREFIX "${DEPS_INSTALL_PREFIX}/yaml-cpp-${YAML_CPP_VERSION}")
set(CURL_PREFIX "${DEPS_INSTALL_PREFIX}/curl-${CURL_VERSION}")
set(ZSTD_PREFIX "${DEPS_INSTALL_PREFIX}/zstd-${ZSTD_VERSION}")
set(PROMETHEUS_CPP_PREFIX "${DEPS_INSTALL_PREFIX}/prometheus-cpp-${PROMETHEUS_CPP_VERSION}")

# Options: Allow users to selectively install dependencies
option(AUTO_INSTALL_DEPENDENCIES "Automatically install missing dependencies" ON)
option(BUILD_DEPS_BOTH_STATIC_SHARED "Build both static and shared libs when auto-installing deps" ON)
option(INSTALL_OPENSSL "Install OpenSSL if not found" ON)
option(INSTALL_BOOST "Install Boost if not found" ON)
option(INSTALL_LIBEVENT "Install libevent if not found" ON)
option(INSTALL_CURL "Install CURL if not found" ON)
option(INSTALL_YAML_CPP "Install yaml-cpp if not found" ON)
option(INSTALL_SPDLOG "Install spdlog if not found" ON)
option(INSTALL_RAPIDJSON "Install RapidJSON if not found" ON)
option(INSTALL_CONCURRENTQUEUE "Install concurrentqueue if not found" ON)
option(INSTALL_PROTOBUF "Install Protobuf if not found" ON)
option(INSTALL_GRPC "Install gRPC if not found" ON)
option(INSTALL_ABSEIL "Install Abseil if not found" ON)
option(INSTALL_ETCD_CPP "Install etcd-cpp-apiv3 if not found" ON)
option(INSTALL_CARES "Install c-ares if not found (for gRPC)" ON)
option(INSTALL_RE2 "Install re2 if not found (for gRPC)" ON)
option(INSTALL_CPPREST "Install cpprestsdk if not found" OFF)  # cpprest build has issues; use system
option(INSTALL_ZSTD "Install zstd if not found" ON)

# Dependency detection function
function(check_and_install_dependency DEP_NAME CHECK_FILE INSTALL_OPTION PREFIX_VAR)
    if(EXISTS "${${PREFIX_VAR}}/${CHECK_FILE}")
        message(STATUS "Found ${DEP_NAME} at ${${PREFIX_VAR}}")
        set(${DEP_NAME}_FOUND TRUE PARENT_SCOPE)
        set(${DEP_NAME}_PREFIX "${${PREFIX_VAR}}" PARENT_SCOPE)
    else()
        message(STATUS "${DEP_NAME} not found at ${${PREFIX_VAR}}")
        if(AUTO_INSTALL_DEPENDENCIES AND ${INSTALL_OPTION})
            message(STATUS "Will install ${DEP_NAME} during build")
            set(${DEP_NAME}_NEEDS_INSTALL TRUE PARENT_SCOPE)
        else()
            message(WARNING "${DEP_NAME} not found and auto-install is disabled")
        endif()
    endif()
endfunction()

# Detect all dependencies
message(STATUS "Checking dependencies...")

check_and_install_dependency("OpenSSL" "include/openssl/ssl.h" INSTALL_OPENSSL OPENSSL_PREFIX)
check_and_install_dependency("Boost" "include/boost/version.hpp" INSTALL_BOOST BOOST_PREFIX)
check_and_install_dependency("Libevent" "include/event2/event.h" INSTALL_LIBEVENT LIBEVENT_PREFIX)
check_and_install_dependency("CURL" "include/curl/curl.h" INSTALL_CURL CURL_PREFIX)
check_and_install_dependency("YamlCpp" "include/yaml-cpp/yaml.h" INSTALL_YAML_CPP YAML_CPP_PREFIX)
check_and_install_dependency("Spdlog" "include/spdlog/spdlog.h" INSTALL_SPDLOG SPDLOG_PREFIX)
check_and_install_dependency("RapidJSON" "include/rapidjson/document.h" INSTALL_RAPIDJSON RAPIDJSON_PREFIX)
check_and_install_dependency("Concurrentqueue" "include/concurrentqueue/concurrentqueue.h" INSTALL_CONCURRENTQUEUE CONCURRENTQUEUE_PREFIX)
check_and_install_dependency("Protobuf" "include/google/protobuf/message.h" INSTALL_PROTOBUF PROTOBUF_PREFIX)
check_and_install_dependency("gRPC" "include/grpcpp/grpcpp.h" INSTALL_GRPC GRPC_PREFIX)
check_and_install_dependency("Cares" "include/ares.h" INSTALL_CARES CARES_PREFIX)
check_and_install_dependency("Re2" "include/re2/re2.h" INSTALL_RE2 RE2_PREFIX)
check_and_install_dependency("Abseil" "include/absl/base/config.h" INSTALL_ABSEIL ABSEIL_PREFIX)
check_and_install_dependency("EtcdCpp" "include/etcd/Client.hpp" INSTALL_ETCD_CPP ETCD_CPP_PREFIX)
check_and_install_dependency("Cpprest" "include/cpprest/http_client.h" INSTALL_CPPREST CPPREST_PREFIX)
check_and_install_dependency("Zstd" "include/zstd.h" INSTALL_ZSTD ZSTD_PREFIX)

# Load build configurations for each dependency
if(OpenSSL_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/OpenSSL.cmake)
endif()

if(Boost_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/Boost.cmake)
endif()

if(Libevent_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/Libevent.cmake)
endif()

if(CURL_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/CURL.cmake)
endif()

if(YamlCpp_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/YamlCpp.cmake)
endif()

if(Spdlog_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/Spdlog.cmake)
endif()

if(RapidJSON_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/RapidJSON.cmake)
endif()

if(Concurrentqueue_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/Concurrentqueue.cmake)
endif()

if(Protobuf_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/Protobuf.cmake)
endif()

if(Abseil_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/Abseil.cmake)
endif()

if(Cares_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/Cares.cmake)
endif()
if(Re2_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/Re2.cmake)
endif()
if(gRPC_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/gRPC.cmake)
endif()

if(EtcdCpp_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/EtcdCpp.cmake)
endif()

if(Cpprest_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/Cpprest.cmake)
endif()

if(Zstd_NEEDS_INSTALL)
    include(${CMAKE_CURRENT_LIST_DIR}/external/Zstd.cmake)
endif()

# When AUTO_INSTALL_DEPENDENCIES=OFF and required deps are missing, FATAL_ERROR with install instructions
set(REQUIRED_DEPS_FOR_BUILD OpenSSL CURL Libevent YamlCpp Protobuf Abseil gRPC)
set(MISSING_DEPS "")
foreach(_dep ${REQUIRED_DEPS_FOR_BUILD})
    if(NOT ${_dep}_FOUND)
        list(APPEND MISSING_DEPS ${_dep})
    endif()
endforeach()
if(MISSING_DEPS AND NOT AUTO_INSTALL_DEPENDENCIES)
    list(JOIN MISSING_DEPS ", " MISSING_DEPS_STR)
    message(FATAL_ERROR
        "Required dependencies are missing and AUTO_INSTALL_DEPENDENCIES is OFF.\n"
        "Missing: ${MISSING_DEPS_STR}\n"
        "Enable auto-install (recommended when deps are missing):\n"
        "  cmake .. -DAUTO_INSTALL_DEPENDENCIES=ON\n"
        "  cmake --build . --target install_dependencies\n"
        "  cmake .."
    )
endif()
