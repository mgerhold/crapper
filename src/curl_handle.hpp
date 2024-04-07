#pragma once

#include "crapper/status_codes.hpp"
#include "libcurl.hpp"
#include "types.hpp"

#include <memory>
#include <stdexcept>
#include <unordered_map>

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
    using HeaderFunction = std::size_t (*)(char*, std::size_t, std::size_t, void*);
    using ReadFunction = std::size_t (*)(char*, std::size_t, std::size_t, void*);
    using SeekFunction = int (*)(void*, CurlOffset, int origin);
    using SockoptFunction = int (*)(void*, CurlSocket, CurlSocketType);

    CurlHandle();

    void writefunction(WriteFunction function);
    void writedata(void* data);
    void readfunction(ReadFunction function);
    void readdata(void* data);
    void url(char const* url);
    void get();
    void post();
    void postfields(std::string const& postdata);
    [[nodiscard]] std::unordered_multimap<std::string, std::string> get_headers();
    [[nodiscard]] HttpStatusCode get_http_status_code();

    void perform();

    // todo: add a lot of options, see https://curl.se/libcurl/c/curl_easy_setopt.html

private:
    static void evaluate_setopt_return_code(int return_code);
};
