#include "get_request.hpp"

void GetRequest::prepare_curl(CurlHandle& curl_handle) const {
    Request::prepare_curl(curl_handle);
    curl_handle.get();
    // todo: pass data to curl
}
