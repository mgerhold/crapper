include(GoogleTest)

add_executable(libcurl_tests
        libcurl_tests.cpp
        ../src/libcurl.hpp
        ../src/libcurl.cpp
        ../src/utils.hpp
        ../src/utils.cpp
)

set(CURL_LIBRARY "libcurl_static")
if (BUILD_SHARED_LIBS)
    set(CURL_LIBRARY "libcurl_shared")
endif ()

target_include_directories(libcurl_tests PRIVATE ../src)
target_link_libraries(libcurl_tests PRIVATE crapper_options)
target_link_system_libraries(libcurl_tests PRIVATE ${CURL_LIBRARY} GTest::gtest GTest::gtest_main)

add_executable(crapper_tests
        crapper_tests.cpp
)
target_compile_definitions(crapper_tests PRIVATE
        TEST_ROOT="${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(crapper_tests PRIVATE crapper crapper_options)
target_link_system_libraries(crapper_tests PRIVATE GTest::gtest GTest::gtest_main c2k_sockets)

gtest_discover_tests(libcurl_tests)
gtest_discover_tests(crapper_tests)
