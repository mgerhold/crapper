#include "include/crapper/headers.hpp"
#include <curl/curl.h>
#include <format>


void CurlStringListDeleter::operator()(curl_slist* const pointer) const {
    curl_slist_free_all(pointer);
}

[[nodiscard]] CurlStringList Headers::as_curl_string_list() const {
    if (m_fields.empty()) {
        return nullptr;
    }
    auto list = static_cast<curl_slist*>(nullptr);
    for (auto const& [key, value] : m_fields) {
        list = curl_slist_append(list, std::format("{}: {}", key, value).c_str());
    }
    return CurlStringList{ list };
}
