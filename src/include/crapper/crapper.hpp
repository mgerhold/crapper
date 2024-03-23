#pragma once

#include <memory>
#include <string>

class CurlHandle;

class Crapper final {
private:
    std::unique_ptr<CurlHandle> m_curl_handle;

public:
    using Buffer = std::string;

    Crapper();
    // Crapper(Crapper const&) = delete;
    Crapper(Crapper&&) = default;
    // Crapper& operator=(Crapper const&) = delete;
    Crapper& operator=(Crapper&&) = default;
    ~Crapper();

    [[nodiscard]] Buffer get(char const* url);
};
