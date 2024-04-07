#pragma once

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
};
