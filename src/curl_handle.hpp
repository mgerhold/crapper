#pragma once

#include "libcurl.hpp"
#include "types.hpp"
#include <memory>
#include <stdexcept>

class CurlHandleError final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

typedef void CURL;

using CurlOffset = std::int64_t;

class CurlHandle final {
private:
    struct Deleter final {
        void operator()(CURL* handle) const;
    };

    Libcurl m_libcurl;
    std::unique_ptr<CURL, Deleter> m_curl;

public:
    using WriteFunction = std::size_t (*)(char*, std::size_t, std::size_t, void*);
    using ReadFunction = std::size_t (*)(char*, std::size_t, std::size_t, void*);
    using SeekFunction = int (*)(void*, CurlOffset, int origin);
    using SockoptFunction = int (*)(void*, CurlSocket, CurlSocketType);

    CurlHandle();

    CurlHandle& writefunction(WriteFunction function) &;
    CurlHandle writefunction(WriteFunction function) &&;
    CurlHandle& writedata(void* data) &;
    CurlHandle writedata(void* data) &&;
    CurlHandle& readfunction(ReadFunction function) &;
    CurlHandle readfunction(ReadFunction function) &&;
    CurlHandle& readdata(void* data) &;
    CurlHandle readdata(void* data) &&;
    CurlHandle& url(char const* url) &;
    CurlHandle url(char const* url) &&;
    void perform();

    // todo: add a lot of options, see https://curl.se/libcurl/c/curl_easy_setopt.html

private:
    static void evaluate_setopt_return_code(int return_code);
};
