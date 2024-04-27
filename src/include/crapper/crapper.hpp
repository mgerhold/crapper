#pragma once

#include "method.hpp"
#include "response.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include <string>


class Request;
class CurlHandle;

namespace detail {
    template<typename T>
    concept Serializable = requires(T t) { nlohmann::json(t); };
} // namespace detail

class Crapper final {
public:
    using Buffer = std::string;

private:
    struct State final {
        std::string url;
        Method method = Method::Default;
        std::string body;
        std::unordered_multimap<std::string, std::string> header_fields;

        [[nodiscard]] std::unique_ptr<Request> create_request(CurlHandle& curl_handle) const;

        void reset() {
            *this = {};
        }
    };

    std::unique_ptr<CurlHandle> m_curl_handle; // pointer indirection to avoid exposing CurlHandle's definition
    State m_state;

public:
    Crapper();
    // Crapper(Crapper const&) = delete;
    Crapper(Crapper&&) noexcept;
    // Crapper& operator=(Crapper const&) = delete;
    Crapper& operator=(Crapper&&) noexcept;
    ~Crapper();

    Crapper& get(std::string url) &;
    [[nodiscard]] Crapper get(std::string url) &&;
    Crapper& post(std::string url) &;
    [[nodiscard]] Crapper post(std::string url) &&;
    Crapper& delete_(std::string url) &;
    [[nodiscard]] Crapper delete_(std::string url) &&;

    Crapper& header(HeaderKey key, std::string value) &;
    [[nodiscard]] Crapper header(HeaderKey key, std::string value) &&;

    Crapper& body(std::string body) &;
    [[nodiscard]] Crapper body(std::string body) &&;

    Crapper& json(detail::Serializable auto const& data) & {
        m_state.body = nlohmann::json(data).dump();
        m_state.header_fields.erase(to_string(HeaderKey::ContentType));
        m_state.header_fields.emplace(to_string(HeaderKey::ContentType), to_string(ContentType::ApplicationJson));
        return *this;
    }

    [[nodiscard]] Crapper json(detail::Serializable auto const& data) && {
        this->json(data);
        return std::move(*this);
    }

    [[nodiscard]] Response send();
};
