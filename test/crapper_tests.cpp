#include "crapper_fixture.hpp"
#include <format>

TEST_F(CrapperFixture, SimpleGetRequest) {
    auto const response = Crapper{}.get(base_url).send();
    EXPECT_EQ(response.status(), HttpStatusCode::Ok);
    EXPECT_EQ(response.headers().get("Connection"), "close");
    EXPECT_EQ(response.headers().get("Content-Length"), "2");
    EXPECT_EQ(response.headers().get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.body(), "OK");

    auto const json = get_next_request();
    EXPECT_EQ(json["endpoint"], "");
    EXPECT_EQ(json["method"], "GET");
    EXPECT_EQ(json["headers"]["Host"], std::format("127.0.0.1:{}", http_port));
    EXPECT_EQ(json["headers"]["Accept"], "*/*");
    EXPECT_TRUE(json["body"].is_null());
}
