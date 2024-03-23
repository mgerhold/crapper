#include "curl_handle.hpp"
#include <crapper/crapper.hpp>

// clang-format off
static std::size_t write_callback(
    char* const ptr,
    std::size_t const size,
    std::size_t const nmemb,
    void* const userdata
) { // clang-format on
    auto buffer = static_cast<Crapper::Buffer*>(userdata);
    auto const num_bytes = size * nmemb;
    auto const view = std::string_view{ ptr, ptr + num_bytes };
    *buffer += view;
    return num_bytes;
}

Crapper::Crapper() : m_curl_handle{ std::make_unique<CurlHandle>() } { }

Crapper::~Crapper() = default;

[[nodiscard]] Crapper::Buffer Crapper::get(char const* const url) {
    auto buffer = Buffer{};
    m_curl_handle->writefunction(write_callback).writedata(&buffer).url(url).perform();
    return buffer;
}
