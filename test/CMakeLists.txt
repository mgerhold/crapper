include(GoogleTest)

add_executable(libcurl_tests
        libcurl_tests.cpp
        ../src/libcurl.hpp
        ../src/libcurl.cpp
        ../src/utils.hpp
        ../src/utils.cpp
)

target_include_directories(libcurl_tests PRIVATE ../src)
target_link_libraries(libcurl_tests PRIVATE crapper_options)
target_link_system_libraries(libcurl_tests PRIVATE libcurl_static GTest::gtest GTest::gtest_main)

add_executable(crapper_tests
        crapper_tests.cpp
)
target_link_libraries(crapper_tests PRIVATE crapper crapper_options)
target_link_system_libraries(crapper_tests PRIVATE GTest::gtest GTest::gtest_main)

gtest_discover_tests(libcurl_tests)
gtest_discover_tests(crapper_tests)
