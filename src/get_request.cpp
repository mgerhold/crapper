#include "get_request.hpp"

void GetRequest::prepare_curl(CurlHandle& curl_handle) const {
    Request::prepare_curl(curl_handle);
    if (body().empty()) {
        curl_handle.get();
    } else {
        curl_handle.post();
        curl_handle.postfields(body());
        curl_handle.customrequest(Method::Get);
    }
}
