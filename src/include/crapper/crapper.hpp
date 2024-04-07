#pragma once

#include "../../response.hpp"


#include <memory>
#include <nlohmann/json.hpp>
#include <string>


class Request;
class CurlHandle;

class Crapper final {
public:
    using Buffer = std::string;

private:
    enum class Method {
        Get,
        Post,
        Default = Get,
    };

    struct State final {
        std::string url;
        Method method = Method::Default;
        std::string body;

        [[nodiscard]] std::unique_ptr<Request> create_request(CurlHandle& curl_handle) const;
    };

    std::unique_ptr<CurlHandle> m_curl_handle; // pointer indirection to avoid exposing CurlHandle's definition
    State m_state;

public:
    Crapper();
    // Crapper(Crapper const&) = delete;
    Crapper(Crapper&&) = default;
    // Crapper& operator=(Crapper const&) = delete;
    Crapper& operator=(Crapper&&) = default;
    ~Crapper();

    Crapper& get(std::string url) &;
    [[nodiscard]] Crapper get(std::string url) &&;
    Crapper& post(std::string url) &;
    [[nodiscard]] Crapper post(std::string url) &&;

    /*Crapper& json(auto const& data) {
        auto const value = nlohmann::json{ data };
        auto const serialized = value.dump();

    }*/

    [[nodiscard]] Response send();
};
