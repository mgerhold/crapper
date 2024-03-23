#include <atomic>
#include <gtest/gtest.h>
#include <libcurl.hpp>
#include <thread>

TEST(LibcurlTests, LibraryInitializationSingleThreaded) {
    EXPECT_FALSE(Libcurl::is_initialized());
    {
        auto libcurl = Libcurl{};
        EXPECT_TRUE(Libcurl::is_initialized());
    }
    EXPECT_FALSE(Libcurl::is_initialized());

    {
        auto a = Libcurl{};
        EXPECT_TRUE(Libcurl::is_initialized());
        {
            auto b = Libcurl{};
            EXPECT_TRUE(Libcurl::is_initialized());
        }
        EXPECT_TRUE(Libcurl::is_initialized());
    }
    EXPECT_FALSE(Libcurl::is_initialized());

    {
        auto handles = std::vector<Libcurl>{};
        EXPECT_FALSE(Libcurl::is_initialized());
        for (auto i = 0; i < 10; ++i) {
            handles.emplace_back();
            EXPECT_TRUE(Libcurl::is_initialized());
        }
    }
    EXPECT_FALSE(Libcurl::is_initialized());
}

TEST(LibcurlTests, LibraryInitializationMultiThreaded) {
    auto start = std::atomic_bool{ false };
    auto threads = std::vector<std::jthread>{};
    EXPECT_FALSE(Libcurl::is_initialized());
    for (auto i = 0; i < 10000; ++i) {
        threads.emplace_back([&start] {
            while (not start) { }
            [[maybe_unused]] auto handle = Libcurl{};
            EXPECT_TRUE(Libcurl::is_initialized());
        });
    }
    start = true;
}
