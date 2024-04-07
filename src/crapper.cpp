#include "curl_handle.hpp"
#include "get_request.hpp"
#include "post_request.hpp"

#include <crapper/crapper.hpp>


[[nodiscard]] std::unique_ptr<Request> Crapper::State::create_request(CurlHandle& curl_handle) const {
    switch (method) {
        case Method::Get:
            return std::make_unique<GetRequest>(url, body, curl_handle);
        case Method::Post:
            return std::make_unique<PostRequest>(url, body, curl_handle);
    }
    std::unreachable();
}

Crapper::Crapper() : m_curl_handle{ std::make_unique<CurlHandle>() } { }

Crapper::~Crapper() = default;

Crapper& Crapper::get(std::string url) & {
    m_state.method = Method::Get;
    m_state.url = std::move(url);
    return *this;
}

[[nodiscard]] Crapper Crapper::get(std::string url) && {
    this->get(std::move(url));
    return std::move(*this);
}

Crapper& Crapper::post(std::string url) & {
    m_state.method = Method::Post;
    m_state.url = std::move(url);
    return *this;
}

[[nodiscard]] Crapper Crapper::post(std::string url) && {
    this->post(std::move(url));
    return std::move(*this);
}

[[nodiscard]] Response Crapper::send() {
    auto const request = m_state.create_request(*m_curl_handle);
    return request->send(*m_curl_handle);
}
