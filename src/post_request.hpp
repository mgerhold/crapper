#pragma once

#include "request.hpp"

class PostRequest final : public Request {
public:
    PostRequest(std::string url, std::string body, Headers headers, CurlHandle& curl_handle)
        : Request{ std::move(url), std::move(body), std::move(headers), curl_handle } { }

protected:
    void prepare_curl(CurlHandle& curl_handle) const override;
};
