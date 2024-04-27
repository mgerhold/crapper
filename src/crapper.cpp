#include "curl_handle.hpp"
#include "delete_request.hpp"
#include "get_request.hpp"
#include "post_request.hpp"
#include <crapper/crapper.hpp>

[[nodiscard]] std::unique_ptr<Request> Crapper::State::create_request(CurlHandle& curl_handle) const {
    switch (method) {
        case Method::Get:
            return std::make_unique<GetRequest>(url, body, Headers{ header_fields }, curl_handle);
        case Method::Post:
            return std::make_unique<PostRequest>(url, body, Headers{ header_fields }, curl_handle);
        case Method::Delete:
            return std::make_unique<DeleteRequest>(url, body, Headers{ header_fields }, curl_handle);
    }
    std::unreachable();
}

Crapper::Crapper() : m_curl_handle{ std::make_unique<CurlHandle>() } { }

Crapper::Crapper(Crapper&&) noexcept = default;

Crapper& Crapper::operator=(Crapper&&) noexcept = default;

Crapper::~Crapper() = default;

Crapper& Crapper::get(std::string url) & {
    m_state.reset();
    m_state.method = Method::Get;
    m_state.url = std::move(url);
    return *this;
}

[[nodiscard]] Crapper Crapper::get(std::string url) && {
    this->get(std::move(url));
    return std::move(*this);
}

Crapper& Crapper::post(std::string url) & {
    m_state.reset();
    m_state.method = Method::Post;
    m_state.url = std::move(url);
    return *this;
}

[[nodiscard]] Crapper Crapper::post(std::string url) && {
    this->post(std::move(url));
    return std::move(*this);
}

Crapper& Crapper::delete_(std::string url) & {
    m_state.reset();
    m_state.method = Method::Delete;
    m_state.url = std::move(url);
    return *this;
}

[[nodiscard]] Crapper Crapper::delete_(std::string url) && {
    this->delete_(std::move(url));
    return std::move(*this);
}

Crapper& Crapper::header(HeaderKey const key, std::string value) & {
    m_state.header_fields.insert({ to_string(key), std::move(value) });
    return *this;
}

[[nodiscard]] Crapper Crapper::header(HeaderKey const key, std::string value) && {
    this->header(key, std::move(value));
    return std::move(*this);
}

Crapper& Crapper::body(std::string body) & {
    m_state.body = std::move(body);
    m_state.header_fields.erase(to_string(HeaderKey::ContentType));
    m_state.header_fields.emplace(to_string(HeaderKey::ContentType), to_string(ContentType::TextPlain));
    return *this;
}

[[nodiscard]] Crapper Crapper::body(std::string body) && {
    this->body(std::move(body));
    return std::move(*this);
}

[[nodiscard]] Response Crapper::send() {
    auto const request = m_state.create_request(*m_curl_handle);
    return request->send(*m_curl_handle);
}
