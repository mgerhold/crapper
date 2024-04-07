#include "post_request.hpp"

void PostRequest::prepare_curl(CurlHandle& curl_handle) const {
    Request::prepare_curl(curl_handle);
    curl_handle.post();
    curl_handle.postfields(body());
}
