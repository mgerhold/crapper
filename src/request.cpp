#include "request.hpp"
#include "headers.hpp"
#include <iostream>
#include <unordered_map>

#include <curl/curl.h>

// clang-format off
static std::size_t write_callback(
    char* const ptr,
    std::size_t const size,
    std::size_t const nmemb,
    void* const userdata
) { // clang-format on
    auto buffer = static_cast<std::string*>(userdata);
    auto const num_bytes = size * nmemb;
    auto const view = std::string_view{ ptr, ptr + num_bytes };
    *buffer += view;
    return num_bytes;
}

Request::Request(std::string url, std::string body, CurlHandle& curl_handle)
    : m_url{ std::move(url) },
      m_body{ std::move(body) } {
    curl_handle.writefunction(write_callback);
}

void Request::prepare_curl(CurlHandle& curl_handle) const {
    curl_handle.url(m_url.c_str());
}

[[nodiscard]] Response Request::send(CurlHandle& curl_handle) const {
    auto body_buffer = std::string{};
    curl_handle.writedata(&body_buffer);

    prepare_curl(curl_handle);
    curl_handle.perform();

    return Response{
        curl_handle.get_http_status_code(),
        Headers{ curl_handle.get_headers() },
        std::move(body_buffer),
    };
}
