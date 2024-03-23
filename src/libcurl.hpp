#pragma once

#include <cstdint>
#include <mutex>
#include <stdexcept>

class LibcurlError final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Libcurl final {
private:
    static inline std::mutex s_mutex;
    static inline std::size_t s_num_references;
    bool m_holds_reference{ true };

public:
    Libcurl();
    Libcurl(Libcurl const& other);
    Libcurl(Libcurl&& other) noexcept;
    Libcurl& operator=(Libcurl const& other);
    Libcurl& operator=(Libcurl&& other) noexcept;
    ~Libcurl();

    [[nodiscard]] static bool is_initialized();
};
