#pragma once

#include <optional>
#include <string>
#include <unordered_map>

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
};
