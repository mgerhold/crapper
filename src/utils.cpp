#include "utils.hpp"
#include <curl/curl.h>
#include <sstream>

static_assert(std::same_as<std::underlying_type_t<CURLcode>, CurlErrorCode>);

[[nodiscard]] std::string curl_error_string(std::string_view const message, CurlErrorCode const error_code) {
    auto stream = std::ostringstream{};
    stream << message << ": " << curl_easy_strerror(static_cast<CURLcode>(error_code));
    return std::move(stream).str();
}
