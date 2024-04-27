#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>

enum class HeaderKey {
    ContentType,
    Authorization,
};

[[nodiscard]] inline char const* to_string(HeaderKey const key) {
    switch (key) {
        case HeaderKey::ContentType:
            return "Content-Type";
        case HeaderKey::Authorization:
            return "Authorization";
    }
    std::unreachable();
}

enum class ContentType {
    TextPlain,
    ApplicationJson,
};

[[nodiscard]] inline char const* to_string(ContentType const key) {
    switch (key) {
        case ContentType::TextPlain:
            return "text/plain";
        case ContentType::ApplicationJson:
            return "application/json";
    }
    std::unreachable();
}

struct curl_slist;

struct CurlStringListDeleter final {
    void operator()(curl_slist* pointer) const;
};

using CurlStringList = std::unique_ptr<curl_slist, CurlStringListDeleter>;

class Headers final {
private:
    std::unordered_multimap<std::string, std::string> m_fields;

public:
    explicit Headers(std::unordered_multimap<std::string, std::string> fields) : m_fields{ std::move(fields) } { }

    [[nodiscard]] std::unordered_multimap<std::string, std::string> const& fields() const {
        return m_fields;
    }

    [[nodiscard]] std::size_t size() const {
        return m_fields.size();
    }

    [[nodiscard]] std::optional<std::string> get(std::string const& key) const {
        auto const iterator = m_fields.find(key);
        if (iterator == m_fields.cend()) {
            return std::nullopt;
        }
        return iterator->second;
    }

    [[nodiscard]] std::optional<std::string> operator[](std::string const& key) const {
        return get(key);
    }

    [[nodiscard]] CurlStringList as_curl_string_list() const;
};
