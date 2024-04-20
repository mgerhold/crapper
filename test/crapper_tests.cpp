#include "crapper_fixture.hpp"
#include <array>
#include <format>

struct Person {
    std::string name;
    int age = 0;

    Person() = default;
    Person(std::string name, int const age) : name{ std::move(name) }, age{ age } { }

    [[nodiscard]] friend bool operator==(Person const& lhs, Person const& rhs) {
        return std::tie(lhs.name, lhs.age) == std::tie(rhs.name, rhs.age);
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Person, name, age);

TEST_F(CrapperFixture, SimpleGetRequest) {
    auto const response = Crapper{}.get(base_url).send();
    EXPECT_EQ(response.status(), HttpStatusCode::Ok);
    EXPECT_EQ(response.headers().get("Connection"), "close");
    EXPECT_EQ(response.headers().get("Content-Length"), "2");
    EXPECT_EQ(response.headers().get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.body(), "OK");

    auto const request = get_next_request();
    EXPECT_EQ(request["endpoint"], "");
    EXPECT_EQ(request["method"], "GET");
    EXPECT_EQ(request["headers"]["Host"], std::format("127.0.0.1:{}", http_port));
    EXPECT_EQ(request["headers"]["Accept"], "*/*");
    EXPECT_EQ(request["body"], "");
}

TEST_F(CrapperFixture, ConsecutiveGetRequests) {
    static constexpr auto endpoints = std::array{
        "some_endpoint",
        "another_endpoint",
        "kebap-case",
        "nested/deeply/inside",
    };

    auto crapper = Crapper{};

    for (auto const endpoint : endpoints) {
        auto response = crapper.get(std::format("{}/{}", base_url, endpoint)).send();
        EXPECT_EQ(response.status(), HttpStatusCode::Ok);
        EXPECT_EQ(response.headers().get("Connection"), "close");
        EXPECT_EQ(response.headers().get("Content-Length"), "2");
        EXPECT_EQ(response.headers().get("Content-Type"), "text/html; charset=utf-8");
        EXPECT_EQ(response.body(), "OK");

        auto request = get_next_request();
        EXPECT_EQ(request["endpoint"], endpoint);
        EXPECT_EQ(request["method"], "GET");
        EXPECT_EQ(request["headers"]["Host"], std::format("127.0.0.1:{}", http_port));
        EXPECT_EQ(request["headers"]["Accept"], "*/*");
        EXPECT_EQ(request["body"], "");
    }
}

TEST_F(CrapperFixture, GetRequestWithBody) {
    auto const response = Crapper{}.get(base_url).body("test").send();
    EXPECT_EQ(response.status(), HttpStatusCode::Ok);
    EXPECT_EQ(response.headers().get("Connection"), "close");
    EXPECT_EQ(response.headers().get("Content-Length"), "2");
    EXPECT_EQ(response.headers().get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.body(), "OK");

    auto const request = get_next_request();
    EXPECT_EQ(request["endpoint"], "");
    EXPECT_EQ(request["method"], "GET");
    EXPECT_EQ(request["headers"]["Host"], std::format("127.0.0.1:{}", http_port));
    EXPECT_EQ(request["headers"]["Accept"], "*/*");
    EXPECT_EQ(request["headers"]["Content-Type"], "text/plain");
    EXPECT_EQ(request["headers"]["Content-Length"], "4");
    EXPECT_EQ(request["body"], "test");
}

TEST_F(CrapperFixture, GetRequestWithJsonBody) {
    auto const person = Person{ "Claus Kleber", 67 };
    auto const json_string = nlohmann::json(person).dump();
    auto const response = Crapper{}.get(base_url).json(person).send();
    EXPECT_EQ(response.status(), HttpStatusCode::Ok);
    EXPECT_EQ(response.headers().get("Connection"), "close");
    EXPECT_EQ(response.headers().get("Content-Length"), "2");
    EXPECT_EQ(response.headers().get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.body(), "OK");

    auto const request = get_next_request();
    EXPECT_EQ(request["endpoint"], "");
    EXPECT_EQ(request["method"], "GET");
    EXPECT_EQ(request["headers"]["Host"], std::format("127.0.0.1:{}", http_port));
    EXPECT_EQ(request["headers"]["Accept"], "*/*");
    EXPECT_EQ(request["headers"]["Content-Type"], "application/json");
    EXPECT_EQ(request["headers"]["Content-Length"], std::to_string(json_string.length()));
    auto const received_body_string = request["body"].get<std::string>();
    auto const deserialized = nlohmann::json::parse(received_body_string).get<Person>();
    EXPECT_EQ(deserialized, person);
}

TEST_F(CrapperFixture, PostRequestWithBody) {
    auto const response = Crapper{}.post(base_url).body("test").send();
    EXPECT_EQ(response.status(), HttpStatusCode::Ok);
    EXPECT_EQ(response.headers().get("Connection"), "close");
    EXPECT_EQ(response.headers().get("Content-Length"), "2");
    EXPECT_EQ(response.headers().get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.body(), "OK");

    auto const request = get_next_request();
    EXPECT_EQ(request["endpoint"], "");
    EXPECT_EQ(request["method"], "POST");
    EXPECT_EQ(request["headers"]["Host"], std::format("127.0.0.1:{}", http_port));
    EXPECT_EQ(request["headers"]["Accept"], "*/*");
    EXPECT_EQ(request["headers"]["Content-Type"], "text/plain");
    EXPECT_EQ(request["headers"]["Content-Length"], "4");
    EXPECT_EQ(request["body"], "test");
}

TEST_F(CrapperFixture, PostRequestWithJson) {
    auto const person = Person{ "Claus Kleber", 67 };
    auto const json_string = nlohmann::json(person).dump();
    auto const response = Crapper{}.post(base_url).json(person).send();
    EXPECT_EQ(response.status(), HttpStatusCode::Ok);
    EXPECT_EQ(response.headers().get("Connection"), "close");
    EXPECT_EQ(response.headers().get("Content-Length"), "2");
    EXPECT_EQ(response.headers().get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.body(), "OK");

    auto const request = get_next_request();
    EXPECT_EQ(request["endpoint"], "");
    EXPECT_EQ(request["method"], "POST");
    EXPECT_EQ(request["headers"]["Host"], std::format("127.0.0.1:{}", http_port));
    EXPECT_EQ(request["headers"]["Accept"], "*/*");
    EXPECT_EQ(request["headers"]["Content-Type"], "application/json");
    EXPECT_EQ(request["headers"]["Content-Length"], std::to_string(json_string.length()));
    EXPECT_EQ(request["body"], json_string);
    auto const received_body_string = request["body"].get<std::string>();
    auto const deserialized = nlohmann::json::parse(received_body_string).get<Person>();
    EXPECT_EQ(deserialized, person);
}

TEST_F(CrapperFixture, DeleteRequest) {
    auto const response = Crapper{}.delete_(base_url).send();
    EXPECT_EQ(response.status(), HttpStatusCode::Ok);
    EXPECT_EQ(response.headers().get("Connection"), "close");
    EXPECT_EQ(response.headers().get("Content-Length"), "2");
    EXPECT_EQ(response.headers().get("Content-Type"), "text/html; charset=utf-8");
    EXPECT_EQ(response.body(), "OK");

    auto request = get_next_request();
    EXPECT_EQ(request["endpoint"], "");
    EXPECT_EQ(request["method"], "DELETE");
    EXPECT_EQ(request["headers"]["Host"], std::format("127.0.0.1:{}", http_port));
    EXPECT_EQ(request["headers"]["Accept"], "*/*");
    EXPECT_EQ(request["body"], "");
}
