#include "delete_request.hpp"

void DeleteRequest::prepare_curl(CurlHandle& curl_handle) const {
    Request::prepare_curl(curl_handle);
    if (not body().empty()) {
        curl_handle.post();
        curl_handle.postfields(body());
    }
    curl_handle.customrequest(Method::Delete);
}
