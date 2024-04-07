add_library(crapper
        include/crapper/crapper.hpp
        crapper.cpp
        libcurl.cpp
        libcurl.hpp
        curl_handle.cpp
        curl_handle.hpp
        utils.hpp
        utils.cpp
        types.hpp
        request.cpp
        request.hpp
        response.cpp
        response.hpp
        get_request.cpp
        get_request.hpp
        post_request.cpp
        post_request.hpp
        headers.cpp
        headers.hpp
        include/crapper/status_codes.hpp
)

target_include_directories(crapper PUBLIC include)

set(CURL_LIBRARY "libcurl_static")
if (BUILD_SHARED_LIBS)
    set(CURL_LIBRARY "libcurl_shared")
endif ()

target_link_system_libraries(crapper
        PUBLIC
        nlohmann_json
        PRIVATE
        ${CURL_LIBRARY}
)
