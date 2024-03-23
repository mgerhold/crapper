#pragma once

#include <string_view>
#include "types.hpp"

[[nodiscard]] std::string curl_error_string(std::string_view message, CurlErrorCode error_code);
