#pragma once

#include "curl_handle.hpp"
#include "include/crapper/response.hpp"
#include <string>

class Request {
private:
    std::string m_url;
    std::string m_body;
    Headers m_headers;

protected:
    Request(std::string url, std::string body, Headers headers, CurlHandle& curl_handle);
    virtual void prepare_curl(CurlHandle& curl_handle) const;

    [[nodiscard]] std::string const& body() const {
        return m_body;
    }

public:
    Request(Request const&) = default;
    Request(Request&&) noexcept = default;
    Request& operator=(Request const&) = default;
    Request& operator=(Request&&) noexcept = default;
    virtual ~Request() = default;

    [[nodiscard]] Response send(CurlHandle& curl_handle) const;
};
