include("${PROJECT_SOURCE_DIR}/cmake/CPM.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/system_link.cmake")

function(crapper_setup_dependencies)
    set(LIBCURL_OPTIONS
            "BUILD_CURL_EXE OFF"
            "BUILD_LIBCURL_DOCS OFF"
            "ENABLE_CURL_MANUAL OFF"
            "CURL_DISABLE_TESTS ON"
    )
    if (WIN32)
        list(APPEND LIBCURL_OPTIONS "CURL_USE_SCHANNEL ON")
    endif ()
    CPMAddPackage(
            NAME LIBCURL
            GITHUB_REPOSITORY curl/curl
            GIT_TAG curl-8_6_0
            OPTIONS ${LIBCURL_OPTIONS}
    )
    CPMAddPackage(
            NAME NLOHMANN_JSON
            GITHUB_REPOSITORY nlohmann/json
            VERSION 3.11.3
    )
    add_compile_definitions(JSON_HAS_RANGES=0)
    if (${crapper_build_tests})
        CPMAddPackage(
                NAME GOOGLE_TEST
                GITHUB_REPOSITORY google/googletest
                VERSION 1.14.0
                OPTIONS
                "BUILD_GMOCK OFF"
                "INSTALL_GTEST OFF"
        )
        CPMAddPackage(
                NAME C2K_SOCKETS
                GITHUB_REPOSITORY mgerhold/sockets
                VERSION 0.4.0
        )
    endif ()
endfunction()
