#include "curl_handle.hpp"
#include "utils.hpp"

#include <cassert>
#include <curl/curl.h>

static_assert(std::same_as<std::underlying_type_t<CURLoption>, CurlOption>);
static_assert(std::same_as<CurlOffset, curl_off_t>);
static_assert(std::same_as<CurlSocket, curl_socket_t>);
static_assert(std::same_as<std::underlying_type_t<curlsocktype>, CurlSocketType>);

void CurlHandle::Deleter::operator()(CURL* const handle) const {
    curl_easy_cleanup(handle);
}

CurlHandle::CurlHandle() {
    auto const curl = curl_easy_init();
    if (curl == nullptr) {
        throw CurlHandleError{ "failed to create curl handle" };
    }
    m_curl = decltype(m_curl){ curl };
}

void CurlHandle::writefunction(WriteFunction const function) {
    evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_WRITEFUNCTION, function));
}

void CurlHandle::writedata(void* const data) {
    evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_WRITEDATA, data));
}

void CurlHandle::readfunction(ReadFunction const function) {
    evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_READFUNCTION, function));
}

void CurlHandle::readdata(void* data) {
    evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_READDATA, data));
}

void CurlHandle::url(char const* url) {
    evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_URL, url));
}

void CurlHandle::get() {
    evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_HTTPGET, 1L));
}

void CurlHandle::post() {
    evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_POST, 1L));
}

void CurlHandle::postfields(std::string const& postdata) {
    evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_POSTFIELDS, postdata.c_str()));
}

void CurlHandle::customrequest(Method const method) {
    switch (method) {
        case Method::Get:
            evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_CUSTOMREQUEST, "GET"));
            break;
        case Method::Post:
            evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_CUSTOMREQUEST, "POST"));
            break;
        case Method::Delete:
            evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_CUSTOMREQUEST, "DELETE"));
            break;
    }
}

void CurlHandle::httpheader(CurlStringList const& list) {
    evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_HTTPHEADER, list.get()));
}

[[nodiscard]] std::unordered_multimap<std::string, std::string> CurlHandle::get_headers() {
    auto result = std::unordered_multimap<std::string, std::string>{};
    for (auto previous = static_cast<curl_header*>(nullptr);;) {
        auto const current = curl_easy_nextheader(m_curl.get(), CURLH_HEADER, -1, previous);
        if (current == nullptr) {
            break;
        }
        result.emplace(current->name, current->value);
        previous = current;
    }
    return result;
}

[[nodiscard]] HttpStatusCode CurlHandle::get_http_status_code() {
    long status_code;
    [[maybe_unused]] auto const result = curl_easy_getinfo(m_curl.get(), CURLINFO_RESPONSE_CODE, &status_code);
    assert(result == CURLE_OK);
    return HttpStatusCode{ static_cast<std::underlying_type_t<HttpStatusCode>>(status_code) };
}

void CurlHandle::perform() {
    if (auto const error_code = curl_easy_perform(m_curl.get()); error_code != 0) {
        throw CurlHandleError{ curl_error_string("failed to perform CURL request", error_code) };
    }
}

void CurlHandle::evaluate_setopt_return_code(CurlErrorCode const return_code) {
    if (return_code != 0) {
        throw CurlHandleError{ curl_error_string("failed to set CURL option", return_code) };
    }
}
