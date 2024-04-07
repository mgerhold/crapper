#pragma once

#include "request.hpp"

class GetRequest final : public Request {
public:
    GetRequest(std::string url, std::string body, CurlHandle& curl_handle)
        : Request{ std::move(url), std::move(body), curl_handle } { }

protected:
    void prepare_curl(CurlHandle& curl_handle) const override;
};
