#include "curl_handle.hpp"
#include "utils.hpp"
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

CurlHandle& CurlHandle::writefunction(WriteFunction const function) & {
    evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_WRITEFUNCTION, function));
    return *this;
}

CurlHandle CurlHandle::writefunction(WriteFunction const function) && {
    this->writefunction(function);
    return std::move(*this);
}

CurlHandle& CurlHandle::writedata(void* const data) & {
    evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_WRITEDATA, data));
    return *this;
}

CurlHandle CurlHandle::writedata(void* const data) && {
    this->writedata(data);
    return std::move(*this);
}

CurlHandle& CurlHandle::readfunction(ReadFunction const function) & {
    evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_READFUNCTION, function));
    return *this;
}

CurlHandle CurlHandle::readfunction(ReadFunction const function) && {
    this->readfunction(function);
    return std::move(*this);
}

CurlHandle& CurlHandle::readdata(void* const data) & {
    evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_READDATA, data));
    return *this;
}

CurlHandle CurlHandle::readdata(void* const data) && {
    this->readdata(data);
    return std::move(*this);
}

CurlHandle& CurlHandle::url(char const* const url) & {
    evaluate_setopt_return_code(curl_easy_setopt(m_curl.get(), CURLOPT_URL, url));
    return *this;
}

CurlHandle CurlHandle::url(char const* const url) && {
    this->url(url);
    return std::move(*this);
}

void CurlHandle::perform() {
    if (auto const error_code = curl_easy_perform(m_curl.get()); error_code != 0) {
        throw CurlHandleError{ curl_error_string("failed to perform CURL request", error_code) };
    }
}

void CurlHandle::evaluate_setopt_return_code(int const return_code) {
    if (return_code != 0) {
        throw CurlHandleError{ curl_error_string("failed to set CURL option", return_code) };
    }
}
