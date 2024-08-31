#pragma once

#include "types.hpp"
#include <string>
#include <string_view>

[[nodiscard]] std::string curl_error_string(std::string_view message, CurlErrorCode error_code);
