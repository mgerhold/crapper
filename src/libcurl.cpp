#include "libcurl.hpp"
#include "utils.hpp"
#include <cassert>
#include <curl/curl.h>


Libcurl::Libcurl() {
    auto lock = std::scoped_lock{ s_mutex };
    if (s_num_references == 0) {
        if (auto const error_code = curl_global_init(CURL_GLOBAL_DEFAULT); error_code != 0) {
            throw LibcurlError{ curl_error_string("failed to initialize libcurl", error_code) };
        }
    }
    ++s_num_references;
}

Libcurl::Libcurl([[maybe_unused]] Libcurl const& other) {
    auto lock = std::scoped_lock{ s_mutex };
    assert(s_num_references > 0);
    ++s_num_references;
}

Libcurl::Libcurl(Libcurl&& other) noexcept {
    other.m_holds_reference = false;
}

Libcurl& Libcurl::operator=([[maybe_unused]] Libcurl const& other) {
    auto lock = std::scoped_lock{ s_mutex };
    assert(s_num_references > 0);
    ++s_num_references;
    return *this;
}

Libcurl& Libcurl::operator=(Libcurl&& other) noexcept {
    other.m_holds_reference = false;
    return *this;
}

Libcurl::~Libcurl() {
    if (not m_holds_reference) {
        // this object is in a moved-from state
        return;
    }

    auto lock = std::scoped_lock{ s_mutex };
    assert(s_num_references > 0);
    --s_num_references;
    if (s_num_references == 0) {
        curl_global_cleanup();
    }
}

[[nodiscard]] bool Libcurl::is_initialized() {
    auto lock = std::scoped_lock{ s_mutex };
    return s_num_references > 0;
}
