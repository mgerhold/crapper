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
)

target_include_directories(crapper PUBLIC include)

set(CURL_LIBRARY "libcurl_static")
if(BUILD_SHARED_LIBS)
    set(CURL_LIBRARY "libcurl_shared")
endif()

target_link_system_libraries(crapper PRIVATE ${CURL_LIBRARY})
