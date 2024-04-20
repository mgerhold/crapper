#pragma once

#include "headers.hpp"
#include "status_codes.hpp"
#include <string>

class Response final {
private:
    HttpStatusCode m_status;
    Headers m_headers;
    std::string m_body;

public:
    Response(HttpStatusCode const status, Headers headers, std::string body)
        : m_status{ status },
          m_headers{ std::move(headers) },
          m_body{ std::move(body) } { }

    [[nodiscard]] HttpStatusCode status() const {
        return m_status;
    }

    [[nodiscard]] Headers const& headers() const {
        return m_headers;
    }

    [[nodiscard]] std::string const& body() const {
        return m_body;
    }
};
