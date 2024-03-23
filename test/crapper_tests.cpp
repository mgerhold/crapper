#include <crapper/crapper.hpp>
#include <gtest/gtest.h>
#include <utility>

static constexpr auto url = "https://official-joke-api.appspot.com/random_joke";

TEST(CrapperTests, SimpleGetRequest) {
    std::ignore = Crapper{}.get(url);
}
