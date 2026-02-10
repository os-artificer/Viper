#include "core/core.h"
#include "error/error.h"

#include <curl/curl.h>

namespace viper {

std::error_code Init()
{
    curl_global_init(CURL_GLOBAL_ALL);
    return error::ErrorCode::SUCCESS;
}

void Uninit()
{
    curl_global_cleanup();
}

} // namespace viper

